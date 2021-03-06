#include "flags.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

#include "impi.h"

// NOTE: This is the underlying MPI implementation!
#include "mpi.h"

#include "types.h"
#include "communicator.h"
#include "messaging.h"
#include "debugging.h"

#if 0 // GATHER_STATISTICS

static char *statistic_names[STATS_TOTAL] = {
   STATS_NAME_BARRIER,
   STATS_NAME_SEND,
   STATS_NAME_RECV,
   STATS_NAME_ISEND,
   STATS_NAME_IRECV,
   STATS_NAME_BCAST,
   STATS_NAME_SCATTER,
   STATS_NAME_GATHER,
   STATS_NAME_ALLGATHER,
   STATS_NAME_ALLTOALL,
   STATS_NAME_REDUCE,
   STATS_NAME_ALLREDUCE,
   STATS_NAME_SCAN };

#endif

static communicator *comms[MAX_COMMUNICATORS];

static int add_communicator(MPI_Comm comm, int number, int initial,
                           int local_rank, int local_size, int global_rank, int global_size,
                           int cluster_count, int *coordinators, int *cluster_sizes,
                           int flags, uint32_t *members,
                           communicator **out)
{
   int i; //, start, end, local, remote;

   if (number < 0 || number >= MAX_COMMUNICATORS) {
      ERROR(1, "Ran out of communicator storage (%d)!", number);
      return IMPI_ERR_INTERN;
   }

   if (initial == 0 && number < 3) {
      ERROR(1, "Attempting to overwrite reserved communicator (%d)!", number);
      return IMPI_ERR_INTERN;
   }

   if (comms[number] != NULL) {
      ERROR(1, "Attempting to overwrite existing communicator (%d)!", number);
      return IMPI_ERR_INTERN;
   }

   INFO(0, "add_communicator", "Creating communicator %d : local(%d %d) | global(%d %d)",
           number, local_rank, local_size, global_rank, global_size);

   communicator *c = malloc(sizeof(communicator));

   if (c == NULL) {
      ERROR(1, "Failed to allocate space for communicator (%d)!", number);
      return IMPI_ERR_INTERN;
   }

   c->number = number;
   c->flags = flags;
   c->comm = comm;
   c->local_rank = local_rank;
   c->local_size = local_size;
   c->global_rank = global_rank;
   c->global_size = global_size;
   c->cluster_count = cluster_count;
   c->queue_head = NULL;
   c->queue_tail = NULL;
   c->coordinators = coordinators;
   c->cluster_sizes = cluster_sizes;
   c->members = members;

   comms[number] = c;

#if GATHER_STATISTICS
   for (i=0;i<STATS_TOTAL;i++) {
      comms[number]->counters[i] = 0L;
   }
#endif

   if (out != NULL) {
      *out = c;
   }

   return IMPI_SUCCESS;
}

int init_communicators(int cluster_rank, int cluster_count,
                       int *cluster_sizes, int *cluster_offsets)
{
   // We create three special communicators here for
   // MPI_COMM_WORLD, MPI_COMM_SELF and MPI_COMM_NULL.
   int global_rank, global_count, tmp_process_rank, tmp_cluster_rank;
   int i, error, flags;

   uint32_t *members;
   int *coordinators;

   for (i=0;i<MAX_COMMUNICATORS;i++) {
      comms[i] = NULL;
   }

   // Create MPI_COMM_WORLD
   global_rank = cluster_offsets[cluster_rank]+local_rank;
   global_count = cluster_offsets[cluster_count];

   coordinators = malloc(cluster_count * sizeof(int));

   if (coordinators == NULL) {
      ERROR(1, "Failed to allocate space for communicator (coordinators)!");
      return IMPI_ERR_INTERN;
   }

   for (i=0;i<cluster_count;i++) {
      coordinators[i] = cluster_offsets[i];
   }

   members = malloc(global_count * sizeof(uint32_t));

   if (members == NULL) {
      ERROR(1, "Failed to allocate space for communicator (members)!");
      free(coordinators);
      return IMPI_ERR_INTERN;
   }

   tmp_process_rank = 0;
   tmp_cluster_rank = 0;

   for (i=0;i<global_count;i++) {

      if (i >= cluster_offsets[tmp_cluster_rank+1]) {
         tmp_process_rank = 0;
         tmp_cluster_rank++;
      }

      members[i] = SET_PID(tmp_cluster_rank, tmp_process_rank);
      tmp_process_rank++;
   }

   flags = COMM_FLAG_WORLD | COMM_FLAG_LOCAL;

   if (cluster_count > 1) {
      flags |= COMM_FLAG_REMOTE;
   }

   error = add_communicator(MPI_COMM_WORLD, IMPI_COMM_WORLD, 1,
                            local_rank, local_count, global_rank, global_count,
                            cluster_count, coordinators, cluster_sizes,
                            flags, members, NULL);

   if (error != IMPI_SUCCESS) {
      ERROR(1, "Failed to create MPI_COMM_WORLD!");
      return error;
   }

   // Create MPI_COMM_SELF
   members = malloc(sizeof(uint32_t));

   if (members == NULL) {
      ERROR(1, "Failed to allocate space for communicator (members -- self)!");
      return IMPI_ERR_INTERN;
   }

   members[0] = my_pid;

   coordinators = malloc(sizeof(int));
   cluster_sizes = malloc(sizeof(int));

   if (coordinators == NULL) {
      ERROR(1, "Failed to allocate space for communicator (coordinators -- self)!");     
      return IMPI_ERR_INTERN;
   }

   coordinators[0] = 0;
   cluster_sizes[0] = 1;

   flags = COMM_FLAG_SELF | COMM_FLAG_LOCAL;

   error = add_communicator(MPI_COMM_SELF, IMPI_COMM_SELF, 1,
                            0, 1, 0, 1, 1, coordinators, cluster_sizes, flags, members, NULL);

   if (error != IMPI_SUCCESS) {
      ERROR(1, "Failed to create MPI_COMM_SELF!");
      return error;
   }

   error = add_communicator(MPI_COMM_NULL, IMPI_COMM_NULL, 1,
                            0, 0, 0, 0, 1, NULL, NULL, 0, NULL, NULL);

   if (error != IMPI_SUCCESS) {
      ERROR(1, "Failed to create MPI_COMM_NULL!");
   }

   return error;
}

int create_communicator(MPI_Comm comm, int number,
         int local_rank, int local_size, int global_rank, int global_size,
         int cluster_count, int *coordinators, int *cluster_sizes,
         int flags, uint32_t *members, communicator **out)
{
   return add_communicator(comm, number, 0,
                    local_rank, local_size, global_rank, global_size,
                    cluster_count, coordinators, cluster_sizes,
                    flags, members, out);
}

int free_communicator(communicator * c)
{
   int error;

   MPI_Comm comm = c->comm;

   error = PMPI_Comm_free(&comm);

   comms[c->number] = NULL;

   free(c->coordinators);
   free(c->cluster_sizes);
   free(c->members);
   free(c);

   // TODO: decent error code translation ?
   if (error == MPI_SUCCESS) { 
      return IMPI_SUCCESS;
   } else { 
      return IMPI_ERR_INTERN;
   }
}

communicator* get_communicator(IMPI_Comm comm)
{
   return get_communicator_with_index(comm);
}

communicator *get_communicator_with_index(int index)
{
   if (index < 0 || index >= MAX_COMMUNICATORS) {
      ERROR(1, "get_communicator_with_index(index=%d) index out of bounds!", index);
      return NULL;
   }

   if (comms[index] == NULL) {
      ERROR(1, "get_communicator_with_index(index=%d) communicator not found!", index);
      return NULL;
   }

   return comms[index];
}

void set_communicator_ptr(IMPI_Comm *dst, communicator *src)
{
   *dst = src->number;
}

int rank_is_local(communicator *c, int rank, int *result)
{
   if (rank < 0 || rank >= c->global_size) {
      return IMPI_ERR_RANK;
   }

   *result = (GET_CLUSTER_RANK(c->members[rank]) == cluster_rank);

   return IMPI_SUCCESS;
}

int comm_is_world(communicator* c)
{
   return (c->flags & COMM_FLAG_WORLD) != 0;
}

int comm_is_self(communicator* c)
{
   return (c->flags & COMM_FLAG_SELF) != 0;
}

int comm_is_local(communicator* c)
{
   int result = ((c->flags & COMM_FLAG_LOCAL) != 0) && ((c->flags & COMM_FLAG_REMOTE) == 0);
//   fprintf(stderr, "   JASON: comm_is_local: comm: %d local: %d remote: %d -> result: %d \n", 
//	c->number, (c->flags & COMM_FLAG_LOCAL), (c->flags & COMM_FLAG_REMOTE), result);
   return result;
}

int comm_is_wa(communicator* c)
{
   return ((c->flags & COMM_FLAG_LOCAL) == 0) && ((c->flags & COMM_FLAG_REMOTE) != 0);
}

int comm_is_mixed(communicator* c)
{
   return ((c->flags & COMM_FLAG_LOCAL) != 0) && ((c->flags & COMM_FLAG_REMOTE) != 0);
}

void store_message(message_buffer *m)
{
   communicator* c = comms[m->header.comm];

   if (c == NULL) {
      ERROR(1, "Failed to find communicator %d in store_message!",
		m->header.comm);
      ERROR(1, "Dropping message!");
      return;
   }

   m->next = NULL;

   if (c->queue_head == NULL) {
      c->queue_head = c->queue_tail = m;
   } else { 
      c->queue_tail->next = m;
      c->queue_tail = m;
   }
}

int match_message(message_buffer *m, int comm, int source, int tag)
{
   int result = ((comm == m->header.comm) &&
                 (source == IMPI_ANY_SOURCE || source == m->header.source) &&
                 (tag == IMPI_ANY_TAG || tag == m->header.tag));

   DEBUG(5, "MATCH_MESSAGE: (comm=%d source=%d [any=%d] tag=%d [any=%d]) == (m.comm=%d m.source=%d m.tag=%d) => %d",
	comm, source, IMPI_ANY_SOURCE, tag, IMPI_ANY_TAG,
        m->header.comm, m->header.source, m->header.tag, result);

   return result;
}

message_buffer *find_pending_message(communicator *c, int source, int tag)
{
   message_buffer *curr, *prev;

   DEBUG(4, "FIND_PENDING_MESSAGE: Checking for pending messages in comm=%d from source=%d tag=%d", c->number, source, tag);

   if (c->queue_head == NULL) {
      DEBUG(4, "FIND_PENDING_MESSAGE: No pending messages");
      return NULL;
   }

   curr = c->queue_head;
   prev = NULL;

   while (curr != NULL) {

      if (match_message(curr, c->number, source, tag)) {
          if (curr == c->queue_head) {
              // delete head. check if list empty
              if (c->queue_head == c->queue_tail) {
                 c->queue_head = c->queue_tail = NULL;
              } else {
                 c->queue_head = c->queue_head->next;
              }

          } else if (curr == c->queue_tail) {
              // delete tail. set tail to prev
              c->queue_tail = prev;
              c->queue_tail->next = NULL;
          } else {
              // delete middle.
              prev->next = curr->next;
          }

          curr->next = NULL;
          DEBUG(4, "FIND_PENDING_MESSAGE: Found pending message from %d", curr->header.source);
          return curr;
      }

      prev = curr;
      curr = curr->next;
   }

   DEBUG(4, "FIND_PENDING_MESSAGE: No matching messages");

   return NULL;
}

int inc_communicator_statistics(IMPI_Comm comm, int field)
{
   return 0;
}

int print_communicator_statistics(IMPI_Comm comm)
{
   return 0;
}

int print_all_communicator_statistics()
{
   return 0;
}

