#include "flags.h"

#ifdef IBIS_INTERCEPT

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <execinfo.h>

#include "generated_header.h"

#include "types.h"
#include "request.h"
#include "communicator.h"
#include "group.h"
#include "messaging.h"
#include "wa_sockets.h"
#include "util.h"
#include "debugging.h"
#include "operation.h"

#include "impi.h"

// NOTE: This is the underlying MPI!
#include "mpi.h"

// The number of clusters and the rank of our cluster in this set.
uint32_t cluster_count;
uint32_t cluster_rank;

// The number of processes in our cluster and our process' rank in this set.
int local_count;
int local_rank;

// The pid of my machine.
uint32_t my_pid;

// The location of the server (hostname/ip and port)
static char *server;
static unsigned short port;

// The name of this cluster (must be unique).
static char *cluster_name;

// The size of each cluster, and the offset of each cluster in the
// total set of machines.
static int *cluster_sizes;
static int *cluster_offsets;

// Value of various Fortan MPI constants.
/*
int FORTRAN_MPI_COMM_NULL;
int FORTRAN_MPI_GROUP_NULL;
int FORTRAN_MPI_REQUEST_NULL;
int FORTRAN_MPI_GROUP_EMPTY;
int FORTRAN_MPI_COMM_WORLD;
int FORTRAN_MPI_COMM_SELF;
int FORTRAN_MPI_OP_NULL;
*/

int FORTRAN_FALSE;
int FORTRAN_TRUE;

/*****************************************************************************/
/*                      Initialization / Finalization                        */
/*****************************************************************************/

static int read_config_file()
{
   int  error;
   char *file;
   FILE *config;
   int retval = 1;
   char buffer[1024];

   file = getenv("MPIBIS_CONFIG");

   if (file == NULL) {
      WARN(0, "MPIBIS_CONFIG not set");
      file = "/home/jason/mpibis.config";
   } else {
      retval = 0;
   }

   INFO(0, "MPIBIS", "looking for config file %s", file);

   config = fopen(file, "r");

   if (config == NULL) {
      ERROR(1, "MPIBIS failed to open config file %s", file);
      return retval;
   }

   INFO(0, "MPIBIS", "config file %s opened.", file);

   error = fscanf(config, "%s", buffer);

   if (error == EOF || error == 0) {
      ERROR(1, "Failed to read server adres from %s", file);
      fclose(config);
      return 0;
   }

   server = malloc(strlen(buffer+1));

   if (server == NULL) {
      ERROR(1, "Failed to allocate space for server adres %s", buffer);
      fclose(config);
      return 0;
   }

   strcpy(server, buffer);

   error = fscanf(config, "%hu", &port);

   if (error == EOF || error == 0) {
      ERROR(1, "Failed to read server port from %s", file);
      fclose(config);
      return 0;
   }

   error = fscanf(config, "%s", buffer);

   if (error == EOF || error == 0) {
      ERROR(1, "Failed to read cluster name from %s", file);
      fclose(config);
      return 0;
   }

   cluster_name = malloc(strlen(buffer)+1);

   if (cluster_name == NULL) {
      ERROR(1, "Failed to allocate space for cluster name %s", buffer);
      fclose(config);
      return 0;
   }

   strcpy(cluster_name, buffer);

   error = fscanf(config, "%d", &cluster_rank);

   if (error == EOF || error == 0) {
      ERROR(1, "Failed to read cluster rank from %s", file);
      fclose(config);
      return 0;
   }

   error = fscanf(config, "%d", &cluster_count);

   if (error == EOF || error == 0) {
      ERROR(1, "Failed to read cluster rank from %s", file);
      fclose(config);
      return 0;
   }

   fclose(config);
   return 1;
}

static int init_mpibis(int *argc, char ***argv)
{
   int i, changed, cnt, len;

   server = NULL;
   port = -1;

   cluster_name = NULL;
   cluster_count = 0;
   cluster_rank = -1;

   // First try to read the configuration from an input file whose location is set in the environment.
   // This is needed since Fortran does not pass the command line arguments to the MPI library like C does.
   if (read_config_file() != 1) {
      return 0;
   }

   // Next, parse the command line (possibly overwriting the config).
   i = 1;

   while ( i < (*argc) ){
      changed = 0;

      if ( strcmp((*argv)[i],"--wa-server") == 0 ) {
         if ( i+1 < *argc ){
            server = malloc(strlen((*argv)[i+1])+1);
            strcpy(server, (*argv)[i+1]);
            DELETE_ARG;
         } else {
            ERROR(1, "Missing option for --wa-server");
         }
         DELETE_ARG;

      } else if ( strcmp((*argv)[i],"--wa-server-port") == 0 ) {
         if ( i+1 < *argc ){
            port = (unsigned short) atoi((*argv)[i+1]);
            DELETE_ARG;
         } else {
            ERROR(1, "Missing option for --wa-server-port");
         }
         DELETE_ARG;

      } else if ( strcmp((*argv)[i],"--wa-cluster-name") == 0 ) {
         if ( i+1 < *argc ){

            len = strlen((*argv)[i+1]);

            if (len >= MAX_LENGTH_CLUSTER_NAME) {
               ERROR(1, "Cluster name too long (%d)", len);
            } else {
               cluster_name = malloc(len+1);
               strcpy(cluster_name, (*argv)[i+1]);
            }
            DELETE_ARG;
         } else {
            ERROR(1, "Missing option for --wa-cluster-name");
         }
         DELETE_ARG;

      } else if ( strcmp((*argv)[i],"--wa-cluster-rank") == 0 ) {
         if ( i+1 < *argc ){
            cluster_rank = atoi((*argv)[i+1]);
            DELETE_ARG;
         } else {
            ERROR(1, "Missing option for --wa-cluster-rank");
         }
         DELETE_ARG;

      } else if ( strcmp((*argv)[i],"--wa-cluster-count") == 0 ) {
         if ( i+1 < *argc ){
            cluster_count = atoi((*argv)[i+1]);
            DELETE_ARG;
         } else {
            ERROR(1, "Missing option for --wa-cluster-rank");
         }
         DELETE_ARG;
      }

      if ( !changed ) i++;
   }

   if (server == NULL || port <= 0) {
      ERROR(1, "WA server not (correctly) set (%s %d)!", server, port);
      return 0;
   }

   INFO(1, "", "WA: server %s %d", server, port);

   if (local_rank < 0 || local_count <= 0 || local_rank >= local_count) {
      ERROR(1, "WA: local cluster info not set correctly (%d, %d)!", local_rank, local_count);
      return 0;
   }

   if (cluster_name == NULL || cluster_rank < 0 || cluster_count <= 0 || cluster_rank >= cluster_count) {
      ERROR(1, "WA: cluster info not set correctly (%s, %d, %d)!", cluster_name, cluster_rank, cluster_count);
      return 0;
   }

   INFO(1, "", "WA: I am %d of %d in cluster %s", local_rank, local_count, cluster_name);
   INFO(1, "", "WA: cluster %s is %d of %d clusters", cluster_name, cluster_rank, cluster_count);

   cluster_sizes = malloc(cluster_count * sizeof(int));
   cluster_offsets = malloc((cluster_count+1) * sizeof(int));

   my_pid = SET_PID(cluster_rank, local_rank);

   return 1;
}

static void init_constants()
{
   init_fortran_logical_(&FORTRAN_TRUE, &FORTRAN_FALSE);
   INFO(1, "init_constants", "FORTRAN_TRUE     = %d", FORTRAN_TRUE);
   INFO(1, "init_constants", "FORTRAN_FALSE    = %d", FORTRAN_FALSE);
}

#define __IMPI_Init
int IMPI_Init(int *argc, char **argv[])
{
   int i=0;

   int status = PMPI_Init(argc, argv);

   INFO(0, "IMPI_Init", "(...)");

   if (status == MPI_SUCCESS) {
      PMPI_Comm_rank(MPI_COMM_WORLD, &local_rank);
      PMPI_Comm_size(MPI_COMM_WORLD, &local_count);

      INFO(1, "IMPI_Init", "START MPI WRAPPER on %d of %d", local_rank, local_count);

      for (i=0;i<*argc;i++) {
         INFO(4, "IMPI_Init", "argv[%d] = %s", i, (*argv)[i]);
      }

      init_constants();

      init_convert();

      status = init_groups();

      if (status != IMPI_SUCCESS) {
         IERROR(1, "Failed to initialize groups!");
         PMPI_Finalize();
         return status;
      }

      status = init_request();

      if (status != IMPI_SUCCESS) {
         IERROR(1, "Failed to initialize requests!");
         PMPI_Finalize();
         return status;
      }

      status = init_operations();

      if (status != IMPI_SUCCESS) {
         IERROR(1, "Failed to initialize operations!");
         PMPI_Finalize();
         return status;
      }

      status = init_mpibis(argc, argv);

      if (status == 0) {
         IERROR(1,"Failed to initialize mpibis!");
         PMPI_Finalize();
         return IMPI_ERR_INTERN;
      }

      status = wa_init(server, port, local_rank, local_count,
                   cluster_name, cluster_rank, cluster_count,
                   cluster_sizes, cluster_offsets);

      if (status == 0) {
         IERROR(1, "Failed to initialize wide area communication!");
         PMPI_Finalize();
         return IMPI_ERR_INTERN;
      }

      status = init_communicators(cluster_rank, cluster_count,
                                  cluster_sizes, cluster_offsets);

      if (status != IMPI_SUCCESS) {
         IERROR(1, "Failed to initialize communicators!");
         wa_finalize();
         PMPI_Finalize();
         return status;
      }
   }

   return IMPI_SUCCESS;
}

#define __IMPI_Finalize
int IMPI_Finalize(void)
{
   int error;

   print_all_communicator_statistics();

   // We tell the system to shut down by terminating MPI_COMM_WORLD.
   error = messaging_send_terminate_request(get_communicator(IMPI_COMM_WORLD));

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to terminate MPI_COMM_WORLD! (error=%d)", error);
   }

   wa_finalize();

   return PMPI_Finalize();
}

#define __IMPI_Abort
int IMPI_Abort(MPI_Comm comm, int errorcode)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return IMPI_ERR_COMM;
   }

   print_all_communicator_statistics();

   return PMPI_Abort(c->comm, errorcode);
}

/*****************************************************************************/
/*                             Send / Receive                                */
/*****************************************************************************/

#define __IMPI_Send
int IMPI_Send(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm)
{
   int local = 0;

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return IMPI_ERR_COMM;
   }

   if (count < 0) {
      ERROR(1, "Invalid count! (%d)", count);
      return IMPI_ERR_COUNT;
   }

   if (rank_is_local(c, dest, &local) != MPI_SUCCESS) {
      ERROR(1, "Illegal destination rank! (%d)", dest);
      return IMPI_ERR_RANK;
   }

   inc_communicator_statistics(comm, STATS_SEND);

   
HIERO

   if (local == 1) {
      return PMPI_Send(buf, count, datatype, dest, tag, c->comm);
   } else {
      return messaging_send(buf, count, datatype, dest, tag, c);
   }
}

#define __IMPI_Rsend
int IMPI_Rsend(void* buf, int count, MPI_Datatype datatype,
                     int dest, int tag, MPI_Comm comm)
{
   // In rsend, it is the users responsibility to ensure that the
   // matching receive has already been posted -before- this rsend
   // is invoked. This allows MPI to assume all is well and proceed
   // in sending the message without additional handshakes. If the
   // receive has not been posted, however, the program is erroneous
   // and may crash.
   //
   // In this implementation we simply replace rsend by send, which
   // offers a stricter (less efficient) contract.
   return IMPI_Send(buf, count, datatype, dest, tag, comm);
}

#define __IMPI_Isend
int IMPI_Isend(void *buf, int count, MPI_Datatype datatype,
                     int dest, int tag, MPI_Comm comm, MPI_Request *req)
{
   int error, local, flags = REQUEST_FLAG_SEND;
   request *r;

   if (count < 0) {
      ERROR(1, "Invalid count!");
      return MPI_ERR_COUNT;
   }
/*
   if (tag < 0 || tag > MPI_TAG_UB) {
      if (tag != MPI_ANY_TAG) {
         ERROR(1, "Illegal tag! (2) %d %d", tag, MPI_TAG_UB);
         return MPI_ERR_TAG;
      }
   }
*/
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found! (%p)", (void*) comm);
      return MPI_ERR_COMM;
   }

   if (rank_is_local(c, dest, &local) != MPI_SUCCESS) {
      ERROR(1, "Illegal destination! (%d)", dest);
      return MPI_ERR_RANK;
   }

   if (local) {
      flags |= REQUEST_FLAG_LOCAL;
   }

   r = create_request(flags, buf, count, datatype, dest, tag, c);

   if (r == NULL) {
      IERROR(1, "Failed to create request!");
      return MPI_ERR_INTERN;
   }

   inc_communicator_statistics(comm, STATS_ISEND);

   if (local == 1) {
//      DEBUG(1, "Performing local isend!");
      error = PMPI_Isend(buf, count, datatype, dest, tag, c->comm, &(r->req));
//      DEBUG(1, "Local isend done %d %p!", error, (void *) r->req);
   } else {
//      DEBUG(1, "Performing WA isend !");
      error = messaging_send(buf, count, datatype, dest, tag, c);
//      DEBUG(1, "WA isend done %d %p!", error, (void *) r->req);
   }

   if (error != MPI_SUCCESS) {
      ERROR(1, "Isend failed! (%d)", error);
      free_request(r);
      *req = MPI_REQUEST_NULL;
   } else {
      // We stuff our own data into the users request pointer here...
      set_request_ptr(req, r);
   }

   return error;
}

#define __IMPI_Irsend
int IMPI_Irsend(void *buf, int count, MPI_Datatype datatype,
                     int dest, int tag, MPI_Comm comm, MPI_Request *req)
{
   INFO(0, "MPI_Irsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)",
             buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), req);

   // As with rsend, we can simply replace this call with a normal isend.
   return IMPI_Isend(buf, count, datatype, dest, tag, comm, req);
}

#define __IMPI_Irecv
int IMPI_Irecv(void *buf, int count, MPI_Datatype datatype,
                     int source, int tag, MPI_Comm comm, MPI_Request *req)
{
   int error, local, flag, flags = REQUEST_FLAG_RECEIVE;
   request *r;

   inc_communicator_statistics(comm, STATS_IRECV);

   if (count < 0) {
      ERROR(1, "Invalid count! (%d)", count);
      return MPI_ERR_COUNT;
   }

   // We first unpack the communicator.
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found! (%p)", (void *)comm);
      return MPI_ERR_COMM;
   }

   // Next, we check if the source is local or not.
   if (source == MPI_ANY_SOURCE) {
      // if source is any, the local flag is determined by the distribution of the communicator.
      local = comm_is_local(c);
   } else {
      error = rank_is_local(c, source, &local);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Illegal source! (%d)", source);
         return MPI_ERR_RANK;
      }
   }

   // Next, we create the request struct.
   if (local) {
      flags |= REQUEST_FLAG_LOCAL;
   }

   r = create_request(flags, buf, count, datatype, source, tag, c);

   if (r == NULL) {
      IERROR(1, "Failed to create request!");
      return MPI_ERR_INTERN;
   }

   // Post the ireceive.
   if (local == 1) {
      // If the source is guarenteed to be local, we directly use MPI.
      error = PMPI_Irecv(buf, count, datatype, source, tag, c->comm, &(r->req));

   } else if (source != MPI_ANY_SOURCE) {
      // If the source is guarenteed to be remote, we directly use the WA link.
      error = messaging_probe_receive(r, 0);

   } else { // local == 0 && source == MPI_ANY_SOURCE

      // If the source may be local or remote, we must poll both. Start with local MPI.
      // FIXME: fixed order may cause starvation ?
      error = PMPI_Iprobe(MPI_ANY_SOURCE, tag, c->comm, &flag, MPI_STATUS_IGNORE);

      if (error != MPI_SUCCESS) {
         IERROR(1, "IProbe from MPI_ANY_SOURCE failed!");
         return MPI_ERR_INTERN;
      }

      if (flag) {
         // A message is available locally, so receiving it!
         r->error = PMPI_Recv(buf, count, datatype, MPI_ANY_SOURCE, tag, c->comm, MPI_STATUS_IGNORE);
         r->flags |= REQUEST_FLAG_COMPLETED;
      } else {
         // No local message was found (yet), so try the WA link.
         error = messaging_probe_receive(r, 0);
      }
   }

   if (error != MPI_SUCCESS) {
      ERROR(1, "IRecv failed!");
      free_request(r);
      *req = MPI_REQUEST_NULL;
   } else {
      // We stuff our own data into the users request pointer here...
      set_request_ptr(req, r);
   }

   return error;
}

#define __IMPI_Recv
int IMPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
   int error;
   int local = 0;

   inc_communicator_statistics(comm, STATS_RECV);

   if (count < 0) {
      ERROR(1, "Invalid count!");
      return MPI_ERR_COUNT;
   }

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found! (%p)", (void *)comm);
      return MPI_ERR_COMM;
   }

   if (source != MPI_ANY_SOURCE && (source < 0 || source >= c->global_size)) {
      ERROR(1, "Illegal source! (%d)", source);
      return MPI_ERR_RANK;
   }
/*
   if (tag < 0 || tag > MPI_TAG_UB) {
      if (tag != MPI_ANY_TAG) {
         ERROR(1, "Illegal tag! (4) %d %d", tag, MPI_TAG_UB);
         return MPI_ERR_TAG;
      }
   }
*/

   if (source == MPI_ANY_SOURCE) {
      // if source is any, the local flag is determined by the distribution of the communicator.
      local = comm_is_local(c);
   } else {
      error = rank_is_local(c, source, &local);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Failed to determine location of source! (%d)", source);
         return MPI_ERR_RANK;
      }
   }

   if (local == 1) {
      return PMPI_Recv(buf, count, datatype, source, tag, c->comm, status);
   } else {
      if (source == MPI_ANY_SOURCE) {
         IERROR(0, "MPI_Recv from MIXED MPI_ANY_SOURCE not implemented!");
         return MPI_ERR_RANK;
      }

      return messaging_receive(buf, count, datatype, source, tag, status, c);
   }
}

/*****************************************************************************/
/*                             Waits / Polling                               */
/*****************************************************************************/

static void clear_status(MPI_Status *status)
{
   if (status != MPI_STATUS_IGNORE) {
      status->MPI_SOURCE = MPI_ANY_SOURCE;
      status->MPI_TAG = MPI_ANY_TAG;
      status->MPI_ERROR = 0;

      // FIXME: What datatype must we use in an empty status ?
      // The standard doesn't say!
      MPI_Status_set_elements(status, MPI_BYTE, 0);
   }
}

static int probe_request(MPI_Request *req, int blocking, int *flag, MPI_Status *status)
{
   int error = MPI_SUCCESS;

   request *r = get_request(*req);

   if (r == NULL) {

      DEBUG(1, "PROBE_REQUEST: request=NULL, blocking=%d", blocking);

      clear_status(status);
      *flag = 1;
      return MPI_SUCCESS;
   }

   DEBUG(1, "PROBE_REQUEST: request=(index=%d, flags=%d, srcdest=%d, count=%d, tag=%d) blocking=%d", r->index, r->flags, r->source_or_dest, r->count, r->tag, blocking);

   // We don't support persistent request yet!
   if (request_persistent(r)) {
      IERROR(0, "persistent requests not supported yet! (MPI_Test)");
      clear_status(status);
      free_request(r);
      *flag = 1;
      *req = MPI_REQUEST_NULL;
      return MPI_ERR_REQUEST;

   } else if (request_local(r)) {
      // Pure local request, so we ask MPI.

      DEBUG(2, "PROBE_REQUEST: request=LOCAL blocking=%d", blocking);

      if (blocking) {
         r->error = PMPI_Wait(&(r->req), status);
         *flag = 1;
      } else {
         r->error = PMPI_Test(&(r->req), flag, status);
      }

   } else if (request_send(r)) {

      DEBUG(2, "PROBE_REQUEST: request=WA_SEND blocking=%d", blocking);

      // Non-persistent WA send should already have finished.
      status->MPI_SOURCE = r->source_or_dest;
      status->MPI_TAG = r->tag;
      status->MPI_ERROR = MPI_SUCCESS;
      r->error = MPI_SUCCESS;
      *flag = 1;

   } else if (r->source_or_dest != MPI_ANY_SOURCE) {

      DEBUG(2, "PROBE_REQUEST: request=WA_RECEIVE blocking=%d", blocking);

      // It was a non-persistent remote receive request, so probe the
      // WA link (will do nothing if the request was already completed).
      messaging_probe_receive(r, blocking);

      if (request_completed(r)) {
         *flag = 1;
         if (r->error == MPI_SUCCESS) {
            r->error = messaging_finalize_receive(r, status);
         } else {
            status->MPI_SOURCE = r->source_or_dest;
            status->MPI_TAG = r->tag;
            status->MPI_ERROR = r->error;
         }
      }

   } else {
      // It was a non-persistent mixed receive request, so we must probe
      // the local network and the WA link.

      DEBUG(2, "PROBE_REQUEST: request=WA_RECEIVE_ANY blocking=%d", blocking);

      if (request_completed(r)) { 
         DEBUG(3, "PROBE_REQUEST: request=WA_RECEIVE_ANY already completed by source=%d tag=%d count=%d", r->source_or_dest, r->tag, r->count);
         *flag = 1;
      } else { 
         do {
            // Probe locally first
            DEBUG(3, "PROBE_REQUEST: request=WA_RECEIVE_ANY performing LOCAL probe for ANY, %d", r->tag);

            r->error = PMPI_Iprobe(MPI_ANY_SOURCE, r->tag, r->c->comm, flag, MPI_STATUS_IGNORE);

            if (r->error != MPI_SUCCESS) {
               IERROR(1, "IProbe from MPI_ANY_SOURCE failed! ()");
               return MPI_ERR_INTERN;
            }

            if (*flag) {

               DEBUG(3, "PROBE_REQUEST: request=WA_RECEIVE_ANY performing LOCAL receive");
 
               // A message is available locally, so receiving it!
               r->error = PMPI_Recv(r->buf, r->count, r->type, MPI_ANY_SOURCE, r->tag, r->c->comm, status);
               r->flags |= REQUEST_FLAG_COMPLETED;

            } else {

               DEBUG(3, "PROBE_REQUEST: request=WA_RECEIVE_ANY performing WA probe");
 
               // No local message was found (yet), so try the WA link.
               // NOTE: we should poll here, so blocking is set to 0,
               // ignoring the value of the parameter.
               r->error = messaging_probe_receive(r, 0 /*blocking*/);

               if (request_completed(r)) {

                  DEBUG(3, "PROBE_REQUEST: request=WA_RECEIVE_ANY perfored WA receive");

                  *flag = 1;
                  status->MPI_SOURCE = r->source_or_dest;
                  status->MPI_TAG = r->tag;
                  status->MPI_ERROR = r->error;
               }
            }

         } while (blocking && (*flag == 0) && (r->error == MPI_SUCCESS));
      }
   }

   if (*flag == 1) {
      error = r->error;
      free_request(r);
      *req = MPI_REQUEST_NULL;
   }

   DEBUG(1, "PROBE_REQUEST: done error=%d", error);

   return error;
}

#define __IMPI_Test
int IMPI_Test(MPI_Request *req, int *flag, MPI_Status *status)
{
   int error = probe_request(req, 0, flag, status);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Probe request failed!");
   }

   return error;
}

#define __IMPI_Wait
int IMPI_Wait(MPI_Request *req, MPI_Status *status)
{
   int flag = 0;

   int error = probe_request(req, 1, &flag, status);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Probe request failed!");
   }

   return error;
}

#define __IMPI_Waitany
int IMPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status)
{
   int i;
   int error;
   int undef;
   int flag = 0;

   while (1) {

      undef = 0;

      for (i=0;i<count;i++) {
         if (array_of_requests[i] != MPI_REQUEST_NULL) {
            error = IMPI_Test(&array_of_requests[i], &flag, status);

            if (error != MPI_SUCCESS) {
               *index = i;
               return MPI_ERR_IN_STATUS;
            }

            if (flag) {
               *index = i;
               return MPI_SUCCESS;
            }

         } else {
            undef++;
         }
      }

      if (undef == count) {
         // All requests where MPI_REQUEST_NULL
         *index = MPI_UNDEFINED;
         clear_status(status);
         return MPI_SUCCESS;
      }
   }

   // unreachable!
   return MPI_ERR_INTERN;
}

#define __IMPI_Waitall
int IMPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)
{
   int i, error;
//   size_t size;
//   void *array[5];

   int errors = 0;   

   DEBUG(0, "MPI_WAITALL: %d", count); 

//   size = backtrace(array, 5);
//   backtrace_symbols_fd(array, size, 2);

   for (i=0;i<count;i++) {      
      if (array_of_requests[i] != MPI_REQUEST_NULL) {

         DEBUG(1, "MPI_WAITALL: %d of %d request=%s", i, count, request_to_string(array_of_requests[i])); 
 
         error = IMPI_Wait(&array_of_requests[i], &array_of_statuses[i]);

         DEBUG(1, "MPI_WAITALL: %d of %d request=%s -> error=%d", i, count, request_to_string(array_of_requests[i]), error); 

         if (error != MPI_SUCCESS) {
            errors++;
         }
      }
   }

   DEBUG(0, "MPI_WAITALL: %d error=%d", count, errors); 

   if (errors != 0) {
      return MPI_ERR_IN_STATUS;
   }

   return MPI_SUCCESS;;
}

#define __IMPI_Request_free
int IMPI_Request_free(MPI_Request *r)
{
   int error = MPI_SUCCESS;
   request *req = NULL;

   if (r == NULL || *r == MPI_REQUEST_NULL) {
      return MPI_SUCCESS;
   }

   req = get_request(*r);

   if (req == NULL) {
      IERROR(0, "Request not found!\n");
      return MPI_ERR_REQUEST;
   }

   if (req->req != MPI_REQUEST_NULL) {
      error = PMPI_Request_free(&(req->req));
   }

   free_request(req);
   *r = MPI_REQUEST_NULL;

   return error;
}

#define __IMPI_Request_get_status
int IMPI_Request_get_status(MPI_Request req, int *flag, MPI_Status *status )
{
   int error = MPI_SUCCESS;
   request *r = get_request(req);

   if (r == NULL) {
      clear_status(status);
      *flag = 1;
      return MPI_SUCCESS;
   }

   // We don't support persistent request yet!
   if (request_persistent(r)) {
      IERROR(0, "persistent requests not supported yet! (MPI_Test)");
      clear_status(status);
      *flag = 1;
      return MPI_ERR_REQUEST;
   }

   if (request_local(r)) {
      // Pure local request, so we ask MPI.
      return PMPI_Request_get_status(r->req, flag, status);
   }

   // It was a WA or mixed request.
   // Non-persistent send should already have finished.
   if (request_send(r)) {
      status->MPI_SOURCE = r->source_or_dest;
      status->MPI_TAG = r->tag;
      status->MPI_ERROR = MPI_SUCCESS;
      *flag = 1;
      return MPI_SUCCESS;
   }

   // Non-persistent receive may not have completed yet.
   *flag = request_completed(r);

   if (!(*flag)) {
      messaging_probe_receive(r, 0);
   }

   if (request_completed(r)) {
      *flag = 1;
      if (r->error == MPI_SUCCESS) {
         r->error = messaging_finalize_receive(r, status);
      } else {
         status->MPI_SOURCE = r->source_or_dest;
         status->MPI_TAG = r->tag;
         status->MPI_ERROR = r->error;
      }
   }

   if (*flag) {
      error = r->error;
   }

   return error;
}

/*****************************************************************************/
/*                              Collectives                                  */
/*****************************************************************************/

#define __IMPI_Barrier
int IMPI_Barrier(MPI_Comm comm)
{
   int error, i;
   char buffer = 42;
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform a barrier in local cluster
     inc_communicator_statistics(comm, STATS_BARRIER);
     return PMPI_Barrier(c->comm);
   }

   // We need to perform a WA barrier. This consists of three steps:
   //
   // 1) local barrier, to ensure all local processes have entered the call
   // 2) WA barrier (implemented using flat tree on coordinators) to ensure all
   //    clusters have entered the call
   // 3) local barrier, to ensure all local processes wait until their coordinator
   //    has finished the WA barrier.

INFO(1, "IMPI_Barrier WA BARRIER", "grank=%d lrank=%d coord[0]=%d", c->global_rank, c->local_rank, c->coordinators[0]);

   // Perform the first local barrier

INFO(1, "IMPI_Barrier WA BARRIER", "LOCAL BARRIER(1)");

   error = PMPI_Barrier(c->comm);

   if (error != MPI_SUCCESS) {
      ERROR(1, "IMPI_Barrier: local barrier failed! (1)");
      return MPI_ERR_INTERN;
   }

   // Perform the WA barrier (coordinators only)
   if (c->global_rank == c->coordinators[0]) {

INFO(1, "IMPI_Barrier WA BARRIER", "I am coord[0]");

      // Coordinator 0 first receives from all others....
      for (i=1;i<c->cluster_count;i++) {

INFO(1, "IMPI_Barrier WA BARRIER", "Receiving from coord[i]=%d", c->coordinators[i]);

         error = messaging_receive(&buffer, 1, MPI_BYTE, c->coordinators[i], BARRIER_TAG, MPI_STATUS_IGNORE, c);

         if (error != MPI_SUCCESS) {
            ERROR(1, "IMPI_Barrier: WA receive failed!");
            return MPI_ERR_INTERN;
         }
      }

INFO(1, "IMPI_Barrier WA BARRIER", "Bcast result from coord[0]");

      // ... then bcasts reply.
      error = messaging_bcast(&buffer, 1, MPI_BYTE, c->coordinators[0], c);

      if (error != MPI_SUCCESS) {
         ERROR(1, "IMPI_Barrier: WA bcast failed!");
         return MPI_ERR_INTERN;
      }

   } else {

      for (i=1;i<c->cluster_count;i++) {
         if (c->global_rank == c->coordinators[i]) {

INFO(1, "IMPI_Barrier WA BARRIER", "I am coord[%d]=%d", i, c->coordinators[i]);

            // All other coordinators first send to coordinator 0...

INFO(1, "IMPI_Barrier WA BARRIER", "Sending to coord[0]");

            error = messaging_send(&buffer, 1, MPI_BYTE, c->coordinators[0], BARRIER_TAG, c);

            if (error != MPI_SUCCESS) {
               ERROR(1, "IMPI_Barrier: WA send failed!");
               return MPI_ERR_INTERN;
            }

INFO(1, "IMPI_Barrier WA BARRIER", "Receiving BCAST");

            // Then wait for reply.
            error = messaging_bcast_receive(&buffer, 1, MPI_BYTE, c->coordinators[0], c);

            if (error != MPI_SUCCESS) {
               ERROR(1, "IMPI_Barrier: WA bcast receive failed (error=%d)!", error);
               return MPI_ERR_INTERN;
            }
         }
      }
   }

INFO(1, "IMPI_Barrier WA BARRIER", "LOCAL BARRIER(2)");

   // Perform the second local barrier
   error = PMPI_Barrier(c->comm);

   if (error != MPI_SUCCESS) {
      ERROR(1, "IMPI_Barrier: local barrier failed! (2)");
      return MPI_ERR_INTERN;
   }

INFO(1, "IMPI_Barrier WA BARRIER", "DONE!");


   return MPI_SUCCESS;
}

#define __IMPI_Bcast
int IMPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
   int error, root_cluster, local_cluster;

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform a bcast in local cluster
     inc_communicator_statistics(comm, STATS_BCAST);
     return PMPI_Bcast(buffer, count, datatype, root, c->comm);
   }

   // We need to perform a WA BCAST.

   // If we are root we first send the data to the server and then bcast locally.
   if (c->global_rank == root) {
      error = messaging_bcast(buffer, count, datatype, root, c);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Root %d failed to broadcast in communicator %d!\n", root, c->number);
         return error;
      }

      return PMPI_Bcast(buffer, count, datatype, c->local_rank, c->comm);
   }

   // Retrieve the cluster of the bcast root and the local process.
   root_cluster = GET_CLUSTER_RANK(c->members[root]);
   local_cluster = GET_CLUSTER_RANK(c->members[c->global_rank]);

   // Check if we are in the same cluster as the root. If so, just receive it's bcast.
   if (local_cluster == root_cluster) {
      return PMPI_Bcast(buffer, count, datatype, GET_PROCESS_RANK(c->members[root]), c->comm);
   }

   // If we are not in the same cluster AND we are the root of the local communicator
   // we first receive the WA message.

   // If we are in a different cluster from the root and we are process 0 of the local
   // communicator, we first receive the WA bcast and then forward this bcast locally
   if (c->local_rank == 0) {
      error = messaging_bcast_receive(buffer, count, datatype, root, c);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Local root failed to receive broadcast in communicator %d!\n", c->number);
         return error;
      }
   }

   return PMPI_Bcast(buffer, count, datatype, 0, c->comm);
}

#define __IMPI_Gather
int IMPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf,
                      int recvcount, MPI_Datatype recvtype,
                      int root, MPI_Comm comm)
{
   int error, root_cluster, local_cluster, i, tmp_rank, tmp_cluster;
   MPI_Aint extent;

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // We simply perform a gather in local cluster
     inc_communicator_statistics(comm, STATS_GATHER);
     return PMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, c->comm);
   }

   // We need to do a WA gather.

   // Retrieve the cluster of the gather root and the local process.
   root_cluster = GET_CLUSTER_RANK(c->members[root]);
   local_cluster = GET_CLUSTER_RANK(c->members[c->global_rank]);

   // FIXME: Slow ?
   if (c->global_rank == root) {

      // If we are the root, we simple receive the data in-order from all other processes.

      // Retrieve the data size.
      error = MPI_Type_extent(sendtype, &extent);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Failed to retrieve data size for gather (in communicator %d)!\n", c->number);
         return error;
      }

      // Receive all data.
      for (i=0;i<c->global_size;i++) {

         // Test is the data is send locally or over the WA link
         tmp_cluster = GET_CLUSTER_RANK(c->members[i]);

         if (tmp_cluster == root_cluster) {
            // The data is send locally.
            if (i == c->global_rank) {
               // Our own data is simply copied.
               memcpy(recvbuf + (i * recvcount * extent), sendbuf, recvcount*extent);
               error = MPI_SUCCESS;
            } else {
               // All other local data directly used MPI
               tmp_rank = GET_PROCESS_RANK(c->members[i]);
               error = PMPI_Recv(recvbuf + (i * recvcount * extent), recvcount, recvtype, tmp_rank, 999 /*FIXME*/, c->comm, MPI_STATUS_IGNORE);
            }
         } else {
            // The data is send remotely
            error = messaging_receive(recvbuf + (i * recvcount * extent), recvcount, recvtype, i, GATHER_TAG, MPI_STATUS_IGNORE, c);
         }

         if (error != MPI_SUCCESS) {
            ERROR(1, "Failed to receive data from %d for gather (in communicator %d)!\n", i, c->number);
            return error;
         }
      }

   } else {

      // We need to send some data
      if (local_cluster == root_cluster) {
         // Send within the cluster
         tmp_rank = GET_PROCESS_RANK(c->members[root]);
         error = PMPI_Send(sendbuf, sendcount, sendtype, tmp_rank, 999 /*FIXME*/, c->comm);
      } else {
         // Send between clusters
         error = messaging_send(sendbuf, sendcount, sendtype, root, GATHER_TAG, c);
      }

      if (error != MPI_SUCCESS) {
         ERROR(1, "Failed to send data (in communicator %d)!\n", c->number);
         return error;
      }
   }

   return MPI_SUCCESS;
}

/*
   // We start by performing a local gather. The root for this local gather will be the overall root (for
   // the cluster that contains this process) or the local cluster coordinator (for all other clusters).
   if (root_cluster == local_cluster) {
      local_root = GET_PROCESS_RANK(c->members[root]));
   } else {
      local_root = 0;
   }

   // Create a receive buffer on each of the local root processes.
   if (local_root == c->local_rank) {

      error = MPI_Type_extent(sendtype, &extent);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Failed to retrieve data size for gather (in communicator %d)!\n", c->number);
         return error;
      }

      buffer = malloc(c->local_size * sendcount * extent);

      if (buffer == NULL) {
         ERROR(1, "Failed to allocate buffer for local gather (in communicator %d)!\n", c->number);
         return MPI_ERR_INTERN;
      }
   }

   // NOTE: we only use the send side parameters here. The receive side are only valid on the overall root!
   error = PMPI_Gather(sendbuf, sendcount, sendtype, buffer, sendcount, sendtype, local_root, c->comm);

   if (error != MPI_SUCCESS) {
      ERROR(1, "Failed to perform local gather in communicator %d!\n", c->number);
      return error;
   }

   // Next, all cluster coordinators will send their results to the overall root.
   if (local_root == c->local_rank) {

      if (root_cluster == local_cluster) {
         // I am the overall root, so I create a buffer to receive all data.
         bigbuffer = malloc(c->global_size * sendcount * extent);

         if (bigbuffer == NULL) {
            ERROR(1, "Failed to allocate buffer for global gather (in communicator %d)!\n", c->number);
            return MPI_ERR_INTERN;
         }

         // Next, receive the data from all cluster coordinators. Since the cluster sizes may vary,
         // we need to calculate the appropriate receive offset.
         offset = 0;

         for (i=0;i<c->cluster_count;i++) {

            if (c->coordinator[i] != c->global_rank) {
               // If the coordinator is not me, I'll do a receive.
               error = messaging_receive(bigbuffer + (offset * sendcount * extent), sendcount,
                                   sendtype, c->coordinator[i], GATHER_TAG, MPI_STATUS_IGNORE, c);

               if (error != MPI_SUCCESS) {
                  ERROR(1, "Failed to recievce data from %d in global gather (in communicator %d)!\n", c->coordinator[i], c->number);
                  free(bigbuffer);
                  free(buffer);
                  return MPI_ERR_INTERN;
               }
            } else {
               // Otherwise a copy is enough.
               memcpy(bigbuffer + (offset * sendcount * extent), buffer, sendcount * extent);
            }

            offset += c->cluster_sizes[i];
         }

         // Once all data has been received, we need to reorder it (as the clusters may be interleaved).
         for (i=0;i<c->size;i++) {
            tmp_cluster = GET_CLUSTER_RANK(c->members[i]);
            tmp_rank = GET_PROCESS_RANK(c->members[i]);

     EEP!!! WHAT NOW?



         }

        



      } else {
         // I am a local root.
         error = messaging_send(buffer, sendcount*c->local_size, sendtype, root, GATHER_TAG, c);

         if (error != MPI_SUCCESS) {
            ERROR(1, "Failed to perform local gather in communicator %d!\n", c->number);
            return error;
         }
      }

      free(buffer);
   }

*/

/*
   if (c->global_rank == root) {

   } else {
      PMPI_Send(sendbuf, sendcount, sendtype, root, comm);
   }

   IERROR(0, "WA MPI_Gather not implemented yet!");
   return MPI_ERR_INTERN;
}
*/


#define __IMPI_Gatherv
int IMPI_Gatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                void *recvbuf, int *recvcounts, int *displs,
                                MPI_Datatype recvtype, int root, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform a gatherv in local cluster
     inc_communicator_statistics(comm, STATS_GATHER);
     return PMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, c->comm);
   }

   IERROR(0, "WA MPI_Gatherv not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Allgather
int IMPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                         void* recvbuf, int recvcount, MPI_Datatype recvtype,
                         MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform an allgather in local cluster
     inc_communicator_statistics(comm, STATS_ALLGATHER);
     return PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, c->comm);
   }

   IERROR(0, "WA MPI_Allgather not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Allgatherv
int IMPI_Allgatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                         void *recvbuf, int *recvcounts,
                         int *displs, MPI_Datatype recvtype, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform an allgatherv in local cluster
     inc_communicator_statistics(comm, STATS_ALLGATHER);
     return PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, c->comm);
   }

   IERROR(0, "WA MPI_Allgatherv not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Scatter
int IMPI_Scatter( void* sendbuf, int sendcount, MPI_Datatype sendtype,
                        void* recvbuf, int recvcount, MPI_Datatype recvtype,
                        int root, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform a scatter in local cluster
     inc_communicator_statistics(comm, STATS_SCATTER);
     return PMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, c->comm);
   }

   IERROR(0, "WA MPI_Scatter not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Scatterv
int IMPI_Scatterv(void* sendbuf, int *sendcounts, int *displs,
                        MPI_Datatype sendtype, void* recvbuf, int recvcount,
                        MPI_Datatype recvtype, int root, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform a scatterv in local cluster
     inc_communicator_statistics(comm, STATS_SCATTER);
     return PMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, c->comm);
   }

   IERROR(0, "WA MPI_Scatterv not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Reduce
int IMPI_Reduce(void* sendbuf, void* recvbuf, int count,
                      MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      return MPI_ERR_COMM;
   }

   operation *o = get_operation(op);

   if (o == NULL) {
      ERROR(1, "Operation not found!");
      return MPI_ERR_OP;
   }

   if (comm_is_local(c)) {
     // simply perform a reduce in local cluster
     inc_communicator_statistics(comm, STATS_REDUCE);
     return PMPI_Reduce(sendbuf, recvbuf, count, datatype, o->op, root, c->comm);
   }

   IERROR(0, "WA MPI_Reduce not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Accumulate
int IMPI_Accumulate (void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
                     int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, 
                     MPI_Op op, MPI_Win win)
{
   operation *o = get_operation(op);

   if (o == NULL) {
      ERROR(1, "Operation not found!");
      return MPI_ERR_OP;
   }

   return PMPI_Accumulate(origin_addr, origin_count, origin_datatype, 
                          target_rank, target_disp, target_count, target_datatype, 
                          o->op, win);
}

#define __IMPI_Allreduce
int IMPI_Allreduce(void* sendbuf, void* recvbuf, int count,
                         MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
   // FIXME: Assumes operation is commutative!
   int error, i;
   MPI_Aint extent;
   char *buffer;

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   operation *o = get_operation(op);

   if (o == NULL) {
      ERROR(1, "Operation not found!");
      return MPI_ERR_OP;
   }

   if (comm_is_local(c)) {
     // simply perform an allreduce in local cluster
     inc_communicator_statistics(comm, STATS_ALLREDUCE);
     return PMPI_Allreduce(sendbuf, recvbuf, count, datatype, o->op, c->comm);
   }

   // We need to perform a WA Allreduce. We do this by performing a reduce
   // to our local cluster coordinator. This result is then broadcast to the
   // other cluster coordinators, which merge the results locally. The result
   // of this local merge is then broadcast in each local cluster.
   // NOTE: this does assume the operation is commutative!

   INFO(1, "JASON ALLREDUCE WA", "START LOCAL REDUCE grank=%d lrank=%d count=%d sbuf[0]=%d rbuf[0]=%d\n",
                       c->global_rank, c->local_rank, count, ((int *)sendbuf)[0], ((int *)recvbuf)[0]);

   error = PMPI_Reduce(sendbuf, recvbuf, count, datatype, o->op, 0, c->comm);

   if (error != MPI_SUCCESS) {
      ERROR(1, "Failed to perform local allreduce in communicator %d!\n", c->number);
      return error;
   }

//  INFO(1, "JASON ALLREDUCE WA", "RESULT LOCAL REDUCE grank=%d lrank=%d count=%d sbuf[0]=%d rbuf[0]=%d\n",
//                       c->global_rank, c->local_rank, count, ((int *)sendbuf)[0], ((int *)recvbuf)[0]);

   // The local root shares the result with all other cluster coordinators.
   if (c->local_rank == 0) {

//  INFO(1, "JASON ALLREDUCE WA", "LOCAL ROOT!\n");

      error = PMPI_Type_extent(datatype, &extent);

      if (error != MPI_SUCCESS) {
         ERROR(1, "Failed to retrieve data size for allreduce (in communicator %d)!\n", c->number);
         return error;
      }

      buffer = malloc(count * extent);

      if (buffer == NULL) {
         ERROR(1, "Failed to allocate space for WA Allreduce (in communicator %d)!\n", c->number);
         return MPI_ERR_INTERN;
      }

//  INFO(1, "JASON ALLREDUCE WA", "FIXME: WA BCAST with CRAP performance!!\n");

      // FIXME: This is a synchronous implementation, which is correct but has crap performance!
      for (i=0;i<c->cluster_count;i++) {
         if (c->coordinators[i] == c->global_rank) {

//  INFO(1, "JASON ALLREDUCE WA", "WA BAST SEND i=%d grank=%d lrank=%d count=%d buf[0]=%d\n", i, c->global_rank, c->local_rank, count, ((int*)recvbuf)[0]);
            error = messaging_bcast(recvbuf, count, datatype, c->global_rank, c);

            if (error != MPI_SUCCESS) {
               ERROR(1, "Local root %d failed to bcast local allreduce result in communicator %d error=%d!\n", c->global_rank, c->number, error);
               return error;
            }
         } else {

//  INFO(1, "JASON ALLREDUCE WA", "WA BAST RECV i=%d grank=%d lrank=%d count=%d from=%d\n", i, c->global_rank, c->local_rank, count, c->coordinators[i]);

            error = messaging_bcast_receive(buffer, count, datatype, c->coordinators[i], c);

//  INFO(1, "JASON ALLREDUCE WA", "WA BAST RECEIVED %d\n", buffer[i]);

            if (error != MPI_SUCCESS) {
               ERROR(1, "Local root %d failed to bcast local allreduce result in communicator %d!\n", c->global_rank, c->number);
               return error;
            }

//  INFO(1, "JASON ALLREDUCE WA", "CALLING REDUCE OP buf[0]=%d revcbuf[0]=%d count=%d\n", ((int *)buffer)[0], ((int *)recvbuf)[0], count);

            (*(o->function))((void*)buffer, recvbuf, &count, &datatype);

//  INFO(1, "JASON ALLREDUCE WA", "RESULT REDUCE OP buf[0]=%d revcbuf[0]=%d count=%d\n", ((int *)buffer)[0], ((int *)recvbuf)[0], count);
         }
      }
   }

//  INFO(1, "JASON ALLREDUCE WA", "LOCAL BAST grank=%d lrank=%d count=%d buf[0]=%d\n", c->global_rank, c->local_rank, count, ((int*)recvbuf)[0]);

   return PMPI_Bcast(recvbuf, count, datatype, 0, c->comm);
}

#define __IMPI_Scan
int IMPI_Scan( void* sendbuf, void* recvbuf, int count,
                     MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   operation *o = get_operation(op);

   if (o == NULL) {
      ERROR(1, "Operation not found!");
      return MPI_ERR_OP;
   }

   if (comm_is_local(c)) {
     // simply perform a scan in local cluster
     inc_communicator_statistics(comm, STATS_SCAN);
     return PMPI_Scan(sendbuf, recvbuf, count, datatype, o->op, c->comm);
   }

   IERROR(0, "WA MPI_Scan not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Alltoall
int IMPI_Alltoall(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                        void *recvbuf, int recvcount, MPI_Datatype recvtype,
                        MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform an all-to-all in local cluster
     inc_communicator_statistics(comm, STATS_ALLTOALL);
     return PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, c->comm);
   }

   IERROR(0, "WA MPI_Scan not implemented yet!");
   return MPI_ERR_INTERN;
}

#define __IMPI_Alltoallv
int IMPI_Alltoallv(void *sendbuf, int *sendcounts, int *sdispls,
                        MPI_Datatype sendtype, void *recvbuf, int *recvcounts,
                        int *rdispls, MPI_Datatype recvtype, MPI_Comm comm)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   if (comm_is_local(c)) {
     // simply perform an all-to-all in local cluster
     inc_communicator_statistics(comm, STATS_ALLTOALL);
     return PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, c->comm);
   }

   IERROR(0, "WA MPI_Alltoallv not implemented yet!");
   return MPI_ERR_INTERN;
}

/*****************************************************************************/
/*                         Communicators and Groups                          */
/*****************************************************************************/

#define __IMPI_Comm_size
int IMPI_Comm_size(MPI_Comm comm, int *size)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

//   INFO(1, "MPI_Comm_size", "Retrieve size from %d: local(%d %d) | global(%d %d)", c->number, c->local_rank, c->local_size, c->global_rank, c->global_size);

   *size = c->global_size;

   return MPI_SUCCESS;
}

#define __IMPI_Comm_rank
int IMPI_Comm_rank(MPI_Comm comm, int *rank)
{
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

//   INFO(1, "MPI_Comm_rank", "Retrieve rank from %d: local(%d %d) | global(%d %d)", c->number, c->local_rank, c->local_size, c->global_rank, c->global_size);

   *rank = c->global_rank;

   return MPI_SUCCESS;
}

static int *copy_int_array(int *src, int size)
{
   int *tmp = malloc(size * sizeof(int));

   if (tmp == NULL) {
      return NULL;
   }

   return memcpy(tmp, src, size * sizeof(int));
}

#define __IMPI_Comm_dup
int IMPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm)
{
   int error;
   dup_reply reply;
   MPI_Comm tmp_com;
   uint32_t *members;
   int *coordinators;
   int *cluster_sizes;
   communicator *dup;

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(0, "Commununicator not found!");
      return MPI_ERR_COMM;
   }

   error = messaging_send_dup_request(c);

   if (error != MPI_SUCCESS) {
      IERROR(1, "MPI_Comm_dup send failed!");
      return error;
   }

   error = messaging_receive_dup_reply(&reply);

   if (error != MPI_SUCCESS) {
      IERROR(1, "MPI_Comm_dup receive failed!");
      return error;
   }

   error = PMPI_Comm_dup(c->comm, &tmp_com);

   if (error != MPI_SUCCESS) {
      IERROR(1, "MPI_Comm_dup local dup failed!");
      return error;
   }

   members = (uint32_t *) copy_int_array((int *)c->members, c->global_size);

   if (members == NULL) {
      IERROR(1, "MPI_Comm_dup member copy failed!");
      return error;
   }

   coordinators = copy_int_array(c->coordinators, c->cluster_count);

   if (coordinators == NULL) {
      IERROR(1, "MPI_Comm_dup coordinator copy failed!");
      free(members);
      return error;
   }

   cluster_sizes = copy_int_array(c->cluster_sizes, c->cluster_count);

   if (coordinators == NULL) {
      IERROR(1, "MPI_Comm_dup cluster_sizes copy failed!");
      free(members);
      free(coordinators);
      return error;
   }

   error = create_communicator(tmp_com, reply.newComm,
                 c->local_rank, c->local_size,
                 c->global_rank, c->global_size,
                 c->cluster_count, coordinators, cluster_sizes,
                 c->flags, members, &dup);

   if (error != MPI_SUCCESS) {
      IERROR(1, "MPI_Comm_dup create communicator failed!");
      return error;
   }

   set_communicator_ptr(newcomm, dup);

   return MPI_SUCCESS;
}

static int local_comm_create(communicator *c, group *g, MPI_Comm *newcomm)
{
   int i, error, local_count;
   MPI_Group orig_group;
   MPI_Group new_group;
   int *local_members;
   communicator *world;

   // We first need to split the local part of the group from our local communicator.

   // Collect the local members.
   local_count = 0;
   local_members = malloc(g->size * sizeof(int));

   if (local_members == NULL) {
      IERROR(1, "Failed to allocate memory for local group members!");
      return MPI_ERR_INTERN;
   }

   for (i=0;i<g->size;i++) {
      if (GET_CLUSTER_RANK(g->members[i]) == cluster_rank) {
         local_members[local_count++] = (int) GET_PROCESS_RANK(g->members[i]);
      }
   }

   // If local_count == 0 then we do not need to split the local communicator.
   if (local_count == 0) {
      free(local_members);
      *newcomm = MPI_COMM_NULL;
      return MPI_SUCCESS;
   }

   // If local_count > 0 we need to create a new (local) communicator.
   // Since we known the local ranks relative to WORLD, we'll use that 
   // communicator as a basis.
 
   world = get_communicator_with_index(FORTRAN_MPI_COMM_WORLD);

   error = PMPI_Comm_group(world->comm, &orig_group);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to split local group!");
      free(local_members);
      return error;
   }

   error = PMPI_Group_incl(orig_group, local_count, local_members, &new_group);

   free(local_members);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to perform local group include!");
      return error;
   }

   // Check if we are part of the new communicator.
   if (new_group == MPI_GROUP_NULL) {
      *newcomm = MPI_COMM_NULL;
      return MPI_SUCCESS;
   }

   // HACK: we use the local communicator to create the new one here ? 
   error = PMPI_Comm_create(c->comm, new_group, newcomm);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to create local communicator!");
      return error;
   }

   return MPI_SUCCESS;
}

#define __IMPI_Comm_create
int IMPI_Comm_create(MPI_Comm mc, MPI_Group mg, MPI_Comm *newcomm)
{
   int error, local_rank, local_size;
   group_reply reply;
   MPI_Comm tmp_comm;
   communicator *result;

   // Retrieve our communicator.
   communicator *c = get_communicator(mc);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   // Retrieve our group.
   group *g = get_group(mg);

   if (g == NULL) {
      ERROR(1, "Group not found!");
      return MPI_ERR_GROUP;
   }

   // Request the create at the server
   error = messaging_send_group_request(c, g);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to send comm_create!");
      return error;
   }

   // Wait for the reply.
   error = messaging_receive_group_reply(&reply);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to receive comm_create!");
      return error;
   }

   // If the type of the new communicator is GROUP_TYPE_IDLE we can simply return MPI_COMM_NULL.
   if (reply.type == GROUP_TYPE_IDLE) {
      *newcomm = MPI_COMM_NULL;
      return MPI_SUCCESS;
   }

   // Otherwise, we may have to create a local communicator.
   error = local_comm_create(c, g, &tmp_comm);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to create local communicator!");
      return error;
   }

   // Create the communicator if necessary.
   if (tmp_comm != MPI_COMM_NULL) {
      PMPI_Comm_rank(tmp_comm, &local_rank);
      PMPI_Comm_size(tmp_comm, &local_size);

      error = create_communicator(tmp_comm, reply.newComm,
                 local_rank, local_size, reply.rank, reply.size,
                 reply.cluster_count, reply.coordinators, reply.cluster_sizes,
                 reply.flags, reply.members,
                 &result);

      if (error != MPI_SUCCESS) {
         IERROR(1, "Failed to create new communicator!");
         return error;
      }

      set_communicator_ptr(newcomm, result);

   } else {
      *newcomm = MPI_COMM_NULL;
   }

   return MPI_SUCCESS;
}

#define __IMPI_Comm_split
int IMPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
{
   int error, local_rank, local_size;
   MPI_Comm tmp;
   comm_reply reply;
   communicator *result;

   // We first forward the split request to the server to split the
   // virtual communicator.
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   // Translate the color from MPI_UNDEFINED to -1 if needed.
   if (color == MPI_UNDEFINED) {
      color = -1;
      key = -1;
   }

   error = messaging_send_comm_request(c, color, key);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to send comm_split!");
      return error;
   }

   error = messaging_receive_comm_reply(&reply);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to receive comm_split!");
      return error;
   }

   // The reply from the server will tell us how to split the
   // local communicator. Note that we may get a -1 as a color
   // which we need to translate to an MPI_UNDEFINED first.
   if (reply.color < 0) {
      reply.color = MPI_UNDEFINED;
      reply.key = 0;
   }

   tmp = MPI_COMM_NULL;

   error = PMPI_Comm_split(c->comm, reply.color, reply.key, &tmp);

   if (error != MPI_SUCCESS) {
      IERROR(1, "Failed to perform local comm_split!");
      return error;
   }

   // If a new communicator was returned (that is, color != -1) we
   // have to register the new virtual communicator locally.
   if (tmp != MPI_COMM_NULL) {
      PMPI_Comm_rank(tmp, &local_rank);
      PMPI_Comm_size(tmp, &local_size);

      error = create_communicator(tmp, reply.newComm,
                 local_rank, local_size, reply.rank, reply.size,
                 reply.cluster_count, reply.coordinators, reply.cluster_sizes,
                 reply.flags, reply.members,
                 &result);

      if (error != MPI_SUCCESS) {
         IERROR(1, "Failed to create new communicator!");
         return error;
      }

      set_communicator_ptr(newcomm, result);
   } else {
      *newcomm = MPI_COMM_NULL;
   }

   return MPI_SUCCESS;
}

#define __IMPI_Comm_group
int IMPI_Comm_group(MPI_Comm comm, MPI_Group *g)
{
   int error;
   group *res;

   communicator *c = get_communicator(comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

   error = group_comm_group(c, &res);

   if (error == MPI_SUCCESS) {
      set_group_ptr(g, res);
   } else {
      ERROR(1, "Failed to create group!\n");
   }

   return error;
}

#define __IMPI_Comm_free
int IMPI_Comm_free ( MPI_Comm *comm )
{
   if (*comm == MPI_COMM_WORLD) { 
      // ignored
      return MPI_SUCCESS;
   }
 
   if (*comm == MPI_COMM_NULL) { 
      ERROR(1, "Communicator is MPI_COMM_NULL!");
      return MPI_ERR_COMM;
   }
    
   communicator *c = get_communicator(*comm);

   if (c == NULL) {
      ERROR(1, "Communicator not found!");
      return MPI_ERR_COMM;
   }

/* 
   Ignored for now, as the spec implies that this is an asynchronous operation! 
   
   error = free_communicator(c);
   comm = MPI_COMM_NULL;
   return error;
*/

   WARN(1, "Ignoring MPI_Comm_free on communicator %d!\n", c->number);

   *comm = MPI_COMM_NULL;

   return MPI_SUCCESS;
}

#define __IMPI_Group_rank
int IMPI_Group_rank(MPI_Group g, int *rank)
{
   return group_rank(get_group(g), rank);
}

#define __IMPI_Group_size
int IMPI_Group_size(MPI_Group g, int *size)
{
   return group_size(get_group(g), size);
}

#define __IMPI_Group_incl
int IMPI_Group_incl(MPI_Group g, int n, int *ranks, MPI_Group *newgroup)
{
   group *res;

   int error = group_incl(get_group(g), n, ranks, &res);

   if (error == MPI_SUCCESS) {
      set_group_ptr(newgroup, res);
   } else {
      ERROR(1, "Failed to include group!");
   }

   return error;
}

#define __IMPI_Group_range_incl
int IMPI_Group_range_incl(MPI_Group g, int n, int ranges[][3], MPI_Group *newgroup)
{
   group *res;

   int error = group_range_incl(get_group(g), n, ranges, &res);

   if (error == MPI_SUCCESS) {
      set_group_ptr(newgroup, res);
   } else {
      ERROR(1, "Failed to include group range!");
   }

   return error;
}

#define __IMPI_Group_union
int IMPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup)
{
   int error;
   group *in1;
   group *in2;
   group *out;

   in1 = get_group(group1);
   in2 = get_group(group2);

   if (in1 == NULL || in2 == NULL) {
      ERROR(1, "Failed to find group!");
      return MPI_ERR_GROUP;
   }

   error = group_union(in1, in2, &out);

   if (error == MPI_SUCCESS) {
      set_group_ptr(newgroup, out);
   } else {
      ERROR(1, "Failed to perform group union!");
   }

   return error;
}

#define __IMPI_Group_translate_ranks
int IMPI_Group_translate_ranks(MPI_Group group1, int n, int *ranks1,
                                     MPI_Group group2, int *ranks2)
{
   int i, j, pid, rank;
   group *in1;
   group *in2;

   in1 = get_group(group1);
   in2 = get_group(group2);

INFO(1, "IMPI_Group_translate_ranks DEBUG", "group1 %d group2 %d", in1->size, in2->size);

   if (in1 == NULL || in2 == NULL) {
      ERROR(1, "Group not found!");
      return MPI_ERR_GROUP;
   }

   for (i=0;i<n;i++) {

      rank = ranks1[i];

      if (rank < 0 || rank >= in1->size) {
         ERROR(1, "Incorrect rank!");
         return MPI_ERR_RANK;
      }

      pid = in1->members[rank];

INFO(1, "IMPI_Group_translate_ranks DEBUG", "rank %d pid %d", rank, pid);

      ranks2[i] = MPI_UNDEFINED;

      for (j=0;i<in2->size;j++) {
         if (in2->members[j] == pid) {

INFO(1, "IMPI_Group_translate_ranks DEBUG", "found pid %d at %d", pid, j);

            ranks2[i] = j;
            break;
         }
      }
   }

   return MPI_SUCCESS;
}

/*****************************************************************************/
/*                                Utilities                                  */
/*****************************************************************************/

/* ========== C <-> Fortran conversions ============ */

#define __IMPI_Comm_f2c
MPI_Comm IMPI_Comm_f2c(MPI_Fint comm)
{
   MPI_Comm res;

   if (comm == FORTRAN_MPI_COMM_WORLD) {
      return MPI_COMM_WORLD;
   }

   if (comm == FORTRAN_MPI_COMM_SELF) {
      return MPI_COMM_SELF;
   }

   if (comm == FORTRAN_MPI_COMM_NULL) {
      return MPI_COMM_NULL;
   }

   communicator *tmp = get_communicator_with_index(comm);

   if (tmp == NULL) {
      ERROR(1, "MPI_Comm_f2c(comm=%d) communicator not found!", comm);
      return MPI_COMM_NULL;
   }

   set_communicator_ptr(&res, tmp);
   return res;
}

#define __IMPI_Comm_c2f
MPI_Fint IMPI_Comm_c2f(MPI_Comm comm)
{
   if (comm == MPI_COMM_WORLD) {
      return FORTRAN_MPI_COMM_WORLD;
   }

   if (comm == MPI_COMM_SELF) {
      return FORTRAN_MPI_COMM_SELF;
   }

   if (comm == MPI_COMM_NULL) {
      return FORTRAN_MPI_COMM_NULL;
   }

   communicator *tmp = get_communicator(comm);

   if (tmp == NULL) {
      ERROR(1, "MPI_Comm_c2f(comm=%p) communicator not found!", comm);
      return FORTRAN_MPI_COMM_NULL;
   }

   return tmp->number;
}

#define __IMPI_Group_f2c
MPI_Group IMPI_Group_f2c(MPI_Fint g)
{
   MPI_Group res;

   if (g == FORTRAN_MPI_GROUP_NULL) {
      return MPI_GROUP_NULL;
   }

   if (g == FORTRAN_MPI_GROUP_EMPTY) {
      return MPI_GROUP_EMPTY;
   }

   group *tmp = get_group_with_index(g);

   if (tmp == NULL) {
      ERROR(1, "MPI_Group_f2c(group=%d) group not found!", g);
      return MPI_GROUP_NULL;
   }

   set_group_ptr(&res, tmp);
   return res;
}

#define __IMPI_Group_c2f
MPI_Fint IMPI_Group_c2f(MPI_Group g)
{
   if (g == MPI_GROUP_NULL) {
      return FORTRAN_MPI_GROUP_NULL;
   }

   if (g == MPI_GROUP_EMPTY) {
      return FORTRAN_MPI_GROUP_EMPTY;
   }

   group *tmp = get_group(g);

   if (tmp == NULL) {
      ERROR(1, "MPI_Group_c2f(group=%p) group not found!", (void *)g);
      return FORTRAN_MPI_GROUP_NULL;
   }

   return tmp->index;
}

#define __IMPI_Request_f2c
MPI_Request IMPI_Request_f2c(MPI_Fint req)
{
   MPI_Request r;

   if (req == FORTRAN_MPI_REQUEST_NULL) {
      return MPI_REQUEST_NULL;
   }

   request *tmp = get_request_with_index(req);

   if (tmp == NULL) {
      ERROR(1, "MPI_Request_f2c(req=%d) request not found!", req);
      return MPI_REQUEST_NULL;
   }

   set_request_ptr(&r, tmp);
   return r;
}

#define __IMPI_Request_c2f
MPI_Fint IMPI_Request_c2f(MPI_Request req)
{
   if (req == MPI_REQUEST_NULL) {
      return FORTRAN_MPI_REQUEST_NULL;
   }

   request *tmp = get_request(req);

   if (tmp == NULL) {
      ERROR(1, "MPI_Request_c2f(req=%p) request not found!", req);
      return FORTRAN_MPI_REQUEST_NULL;
   }

   return tmp->index;
}

#define __IMPI_Op_f2c
MPI_Op IMPI_Op_f2c(MPI_Fint op)
{
   MPI_Op o;

   if (op == FORTRAN_MPI_OP_NULL) {
      return MPI_OP_NULL;
   }

   operation *tmp = get_operation_with_index(op);

   if (tmp == NULL) {
      ERROR(1, "MPI_Op_f2c(req=%d) operation not found!", op);
      return MPI_OP_NULL;
   }

   set_operation_ptr(&o, tmp);
   return o;
}


#define __IMPI_Op_c2f
MPI_Fint IMPI_Op_c2f(MPI_Op op) 
{
   operation *tmp = get_operation(op);

   if (tmp == NULL) {
      ERROR(1, "MPI_Op_c2f(op=%p) op not found!", op);
      return FORTRAN_MPI_OP_NULL;
   }

   return tmp->index;
}

// Include the generated impi implementation which contains
// default implementations for all MPI calls.
#include "generated_impi.c"

#endif // IBIS_INTERCEPT




