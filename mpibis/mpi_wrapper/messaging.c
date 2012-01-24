#include "flags.h"

#ifdef IBIS_INTERCEPT


#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#include "mpi.h"
#include "types.h"
#include "messaging.h"

#include "communicator.h"
#include "group.h"
#include "request.h"
#include "wa_sockets.h"

#define BYTE_ORDER_UNDEFINED 0
#define BYTE_ORDER_HOST      1
#define BYTE_ORDER_NETWORK   2

static message_buffer *receive_message(int blocking, int *error);

static void free_message(message_buffer *m)
{
   if (m == NULL) {
      return;
   }

   if (m->data_buffer != NULL) {
      free(m->data_buffer);
   }

   free(m);
}

static message_buffer *probe_wa(communicator *c, int source, int tag, int blocking, int *error)
{
   message_buffer *m;

fprintf(stderr, "Probing socket for incoming messages from %d %d (%d)\n", source, tag, blocking);

   do {
      m = receive_message(blocking, error);

      if (m == NULL) {
fprintf(stderr, "No message received\n");
         return NULL;
      }

fprintf(stderr, "Message received from %d %d\n", m->header.source, m->header.tag);

      if (match_message(m, c->number, source, tag)) {
         // we have a match!
fprintf(stderr, "Match! Returning message\n");
         return m;
      } else {
fprintf(stderr, "No match. Storing message\n");
         store_message(m);
      }

   } while (blocking);

fprintf(stderr, "No message received\n");

   return NULL;
}

/*
static int probe_mpi(request *r, MPI_Status *status)
{
   int flag = 0;

   int error = PMPI_Iprobe(r->source_or_dest, r->tag, r->c->comm, &flag, status);

   if (error != MPI_SUCCESS) {
      fprintf(stderr, "INTERNAL ERROR: MPI_Iprobe returned unexpected error %d\n", error);
      return error;
   }

   error = PMPI_Recv(r->buf, r->count, r->type, r->source_or_dest,
                   r->tag, r->c->comm, status);

   if (error != MPI_SUCCESS) {
      fprintf(stderr, "INTERNAL ERROR: MPI_Recv failed after MPI_Iprobe %d\n", error);
      return error;
   }

   return MPI_SUCCESS;
}
*/

static int unpack_message(void *buf, int count, MPI_Datatype type, MPI_Comm comm,
                           message_buffer *m, MPI_Status *status)
{
  int error = 0;
  int position = 0;

  if (m->header.count != count) {
      fprintf(stderr, "WARNING: message size mismatch! (expected %d got %d\n", 
                  count, m->header.count);

      if (m->header.count < count) {
         // we have received less data
         count = m->header.count;
      }
      // else we have received too much data!
   }

   error = PMPI_Unpack(m->data_buffer, m->header.bytes, &position, buf,
                        count, type, comm);

   if (status != MPI_STATUS_IGNORE && status != MPI_STATUSES_IGNORE) {
      status->MPI_SOURCE = m->header.source;
      status->MPI_TAG = m->header.tag;
      status->MPI_ERROR = error;
      PMPI_Status_set_elements(status, type, count);
   }

   free_message(m);
   return error;
}

static message_buffer *create_message_buffer()
{
   message_buffer *m = malloc(sizeof(message_buffer));

   if (m == NULL) {
       fprintf(stderr, "Failed to allocate message header\n");
       return NULL;
   }

   m->byte_order = BYTE_ORDER_UNDEFINED;
   m->data_buffer_size = 0;
   m->data_buffer = NULL;

   return m;
}

static int create_data_buffer(message_buffer *m, int max_size)
{
   m->data_buffer_size = max_size;
   m->data_buffer = malloc(max_size);

   if (m->data_buffer == NULL) {
       fprintf(stderr, "Failed to allocate message buffer of size %d\n", max_size);
       return CONNECT_ERROR_ALLOCATE;
   }

   return CONNECT_OK;
}

static message_buffer *create_message(int max_size)
{
   message_buffer *m = create_message_buffer();

   if (m == NULL) {
       return NULL;
   }

   if (create_data_buffer(m, max_size) != CONNECT_OK) {
       free_message(m);
       return NULL;
   }

   return m;
}

static void write_message_header(message_buffer *m, int opcode, int comm, int source, int dest, int tag, int count, int bytes)
{
   m->byte_order = BYTE_ORDER_HOST;

   m->header.opcode = opcode;
   m->header.comm = comm;
   m->header.source = source;
   m->header.dest = dest;
   m->header.tag = tag;
   m->header.count = count;
   m->header.bytes = bytes;
}

static int ensure_byte_order(message_buffer *m, int order)
{
   if (m->byte_order == order) {
      return CONNECT_OK;
   }

   if (order == BYTE_ORDER_HOST && m->byte_order == BYTE_ORDER_NETWORK) {
      m->byte_order = BYTE_ORDER_HOST;

      m->header.opcode = ntohl(m->header.opcode);
      m->header.comm = ntohl(m->header.comm);
      m->header.source = ntohl(m->header.source);
      m->header.dest = ntohl(m->header.dest);
      m->header.tag = ntohl(m->header.tag);
      m->header.count = ntohl(m->header.count);
      m->header.bytes = ntohl(m->header.bytes);

      return CONNECT_OK;
   }

   if (order == BYTE_ORDER_NETWORK && m->byte_order == BYTE_ORDER_HOST) {
      m->byte_order = BYTE_ORDER_NETWORK;

      m->header.opcode = htonl(m->header.opcode);
      m->header.comm = htonl(m->header.comm);
      m->header.source = htonl(m->header.source);
      m->header.dest = htonl(m->header.dest);
      m->header.tag = htonl(m->header.tag);
      m->header.count = htonl(m->header.count);
      m->header.bytes = htonl(m->header.bytes);

      return CONNECT_OK;
   }

   fprintf(stderr, "Failed to convert byte order %d to %d\n", order, m->byte_order);
   return CONNECT_ERROR_BYTE_ORDER;
}

static int send_message(message_buffer *m)
{
   int error;
   int size = m->header.bytes;

   if (m->byte_order == BYTE_ORDER_NETWORK) {
      size = ntohl(size);
   } else {
      ensure_byte_order(m, BYTE_ORDER_NETWORK);
   }

   error = wa_sendfully((unsigned char *) &(m->header), MSG_HEADER_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "Failed to send message header!\n");
      return MPI_ERR_INTERN;
   }

   error = wa_sendfully(m->data_buffer, size);

   if (error != CONNECT_OK) {
      fprintf(stderr, "Failed to send message body!\n");
      return MPI_ERR_INTERN;
   }

   return MPI_SUCCESS;
}



static int receive_opcode(int* opcode, int *error, int blocking)
{
fprintf(stderr, "Receiving opcode from socket (blocking=%d)\n", blocking);

   int result = wa_wait_for_data(blocking);

fprintf(stderr, "Result of receive: result=%d error=%d\n", result, *error);

   if (result == -1) {
      *error = MPI_ERR_INTERN;
      return 0;
   }

   if (result == 0) {
      if (blocking == 1) {
         *error = MPI_ERR_INTERN;
      } else {
         *error = MPI_SUCCESS;
      }
      return 0;
   }

   result = wa_receivefully((unsigned char *) opcode, 4);

   if (result != CONNECT_OK) {
      fprintf(stderr, "Failed to receive message opcode!\n");
      *error = MPI_ERR_INTERN;
      return 0;
   }

   *opcode = ntohl(*opcode);
   return 1;
}

static int receive_message_header(message_buffer *m)
{
   // Shift the buffer 4 bytes to skip the opcode.
   unsigned char *tmp = (unsigned char *) &(m->header);
   tmp += 4;

   int error = wa_receivefully(tmp, MSG_HEADER_SIZE-4);

   if (error != CONNECT_OK) {
      fprintf(stderr, "Failed to receive message header!\n");
      return error;
   }

   m->header.opcode = htonl(OPCODE_DATA);
   m->byte_order = BYTE_ORDER_NETWORK;
   return CONNECT_OK;
}

static int receive_message_data(message_buffer *m)
{
   int error = wa_receivefully(m->data_buffer, m->header.bytes);

   if (error != CONNECT_OK) {
      fprintf(stderr, "Failed to receive message data!\n");
      return error;
   }

   return CONNECT_OK;
}

static message_buffer *receive_data_message(int *error)
{
   message_buffer *m = create_message_buffer();

   if (m == NULL) {
      *error = MPI_ERR_INTERN;
      return NULL;
   }

   if (receive_message_header(m) != CONNECT_OK) {
      free_message(m);
      *error = MPI_ERR_INTERN;
      return NULL;
   }

   ensure_byte_order(m, BYTE_ORDER_HOST);

   if (create_data_buffer(m, m->header.bytes) != CONNECT_OK) {
      free_message(m);
      *error = MPI_ERR_INTERN;
      return NULL;
   }

   if (receive_message_data(m) != CONNECT_OK) {
      free_message(m);
      *error = MPI_ERR_INTERN;
      return NULL;
   }

   *error = MPI_SUCCESS;
   return m;
}

static message_buffer *receive_message(int blocking, int *error)
{
   int opcode;

fprintf(stderr, "Receiving message from socket (blocking=%d)\n", blocking);

   int result = receive_opcode(&opcode, error, blocking);

fprintf(stderr, "Result of receive: result=%d error=%d\n", result, *error);

   if (result == 0) {
      // Note: error will be set correctly is blocking was true
      return NULL;
   }

   if (opcode == OPCODE_DATA || opcode == OPCODE_COLLECTIVE_BCAST || opcode == OPCODE_COLLECTIVE_ALLREDUCE) {
      return receive_data_message(error);
   }

   fprintf(stderr, "INTERNAL ERROR: unexpected message opcode (RM) %d\n", opcode);
   *error = MPI_ERR_INTERN;
   return NULL;	
}

static int do_send(int opcode, void* buf, int count, MPI_Datatype datatype, int dest, int tag, communicator* c)
{
   // We have already checked the various parameters, so all we have to so is send the lot!
   int bytes, error;

   error = PMPI_Pack_size(count, datatype, c->comm, &bytes);

   if (error != MPI_SUCCESS) {
      return error;
   }

   message_buffer *m = create_message(bytes);

   if (m == NULL) {
      return MPI_ERR_INTERN;
   }

   bytes = 0;
   error = PMPI_Pack(buf, count, datatype, m->data_buffer, m->data_buffer_size, &bytes, c->comm);

   write_message_header(m, opcode, c->number, c->global_rank, dest, tag, count, bytes);

   if (error == MPI_SUCCESS) {
      error = send_message(m);
   }

   free_message(m);
   return error;
}

int messaging_send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, communicator* c)
{
   return do_send(OPCODE_DATA, buf, count, datatype, dest, tag, c);
}

int messaging_bcast(void* buf, int count, MPI_Datatype datatype, int root, communicator* c)
{
   return do_send(OPCODE_COLLECTIVE_BCAST, buf, count, datatype, root, BCAST_TAG, c);
}

int messaging_bcast_receive(void *buf, int count, MPI_Datatype datatype, int root, communicator* c)
{
   return messaging_receive(buf, count, datatype, root, BCAST_TAG, MPI_STATUS_IGNORE, c);
}

int messaging_allreduce(void* buf, int count, MPI_Datatype datatype, communicator* c)
{
   return do_send(OPCODE_COLLECTIVE_ALLREDUCE, buf, count, datatype, 0, ALLREDUCE_TAG, c);
}

int messaging_allreduce_receive(void *buf, int count, MPI_Datatype datatype, communicator* c)
{
   return messaging_receive(buf, count, datatype, 0, ALLREDUCE_TAG, MPI_STATUS_IGNORE, c);
}

/*
int messaging_probe_receive_request(request *r)
{
   int error = MPI_SUCCESS;
   message_buffer *m;

   // Check if the request was already completed.
   if (r->flags & REQUEST_FLAG_COMPLETED) {
      return 1;
   }

   // We now need to see if we can fullfill the waiting
   // receive request. There are 3 ways to do this:
   //
   // 1. Check to see if we have any pending messages which match.
   // 2. Check if the local MPI can fullfill the request.
   // 3. Check if the wide area connection can fullfill the request.
   //
   // NOTE: step 2 is only performed if the request refers to a MIXED
   //       operation, i.e., a receive from an communicator that contains
   //       both local and remote (wa) processes.
   // If the request is fullfilled, TRUE is returned, FALSE otherwise.

   m = find_pending_message(r->c, r->source_or_dest, r->tag);

   if (m == NULL) {
      m = probe_wa(r->c, r->source_or_dest, r->tag, 0, &error);
   }

   if (m != NULL) {
      r->flags |= REQUEST_FLAG_COMPLETED;
      r->message = m;
//      r->error = unpack_message(r->buf, r->count, r->type, r->c->comm, m, status);
      return 1;

   } else if (error != MPI_SUCCESS) {
      // Something happened in probe_wa!
      r->flags |= REQUEST_FLAG_COMPLETED;
      r->error = error;
      return 1;
   }

   return 0;
}

void messaging_complete_receive_request(request *r, MPI_Status *status)
{
   // Check if we still need to receive a message.
   if (!(r->flags & REQUEST_FLAG_COMPLETED)) {
      r->error = messaging_receive(r->buf, r->count, r->type, r->source_or_dest, r->tag, status, r->comm);
   } else {
      r->error = unpack_message(r->buf, r->count, r->type, r->c->comm, r->message, status);
   }
}
*/



int messaging_receive(void *buf, int count, MPI_Datatype datatype,
              int source, int tag, MPI_Status *status, communicator* c)
{
   int error = MPI_SUCCESS;
   message_buffer *m = find_pending_message(c, source, tag);

   while (m == NULL && error == MPI_SUCCESS) {
      m = probe_wa(c, source, tag, 1, &error);
   }

   if (error == MPI_SUCCESS) {
      error = unpack_message(buf, count, datatype, c->comm, m, status);
   }

   return error;
}

int messaging_probe_receive(request *r, int blocking)
{
   if ((r->flags & REQUEST_FLAG_COMPLETED)) {
      return MPI_SUCCESS;
   }

   r->message = find_pending_message(r->c, r->source_or_dest, r->tag);

   if (r->message != NULL) {
      r->flags |= REQUEST_FLAG_COMPLETED;
      return MPI_SUCCESS;
   }

   r->error = MPI_SUCCESS;

   do {
      r->message = probe_wa(r->c, r->source_or_dest, r->tag, blocking, &(r->error));
   } while (blocking && r->message == NULL && r->error == MPI_SUCCESS);

   if (r->message != NULL || r->error != MPI_SUCCESS) {
      r->flags |= REQUEST_FLAG_COMPLETED;
   }

   return MPI_SUCCESS;
}

int messaging_finalize_receive(request *r, MPI_Status *status)
{
   if (!(r->flags & REQUEST_FLAG_COMPLETED)) {
      return MPI_ERR_INTERN;
   }

   if (r->flags & REQUEST_FLAG_UNPACKED) {
      return MPI_SUCCESS;
   }

   r->error = unpack_message(r->buf, r->count, r->type, r->c->comm, r->message, status);
   r->flags |= REQUEST_FLAG_UNPACKED;

   return r->error;
}

static int queue_pending_messages(int *next_opcode)
{
   int opcode;
   int error = 0;
   int result = 0;

   message_buffer *m;

   while (result == 0) {

fprintf(stderr, "Queuing pending messages (blocking=1)\n");

      result = receive_opcode(&opcode, &error, 1);

      if (result == 0) {
         fprintf(stderr, "INTERNAL ERROR: Failed to receive opcode! (%d)\n", error);
         return error;
      }

fprintf(stderr, "*Result of receive result=%d error=%d\n", result, error);

      if (opcode == OPCODE_DATA || opcode == OPCODE_COLLECTIVE_BCAST || opcode == OPCODE_COLLECTIVE_ALLREDUCE) {
         // There is a message blocking the stream!
         m = receive_data_message(&error);

         if (m == NULL) {
            fprintf(stderr, "INTERNAL ERROR: Failed to receive message!\n");
            return error;
         }

fprintf(stderr, "*Message received from src: %d opcode: %d tag: %d\n", m->header.source, opcode, m->header.tag);

         store_message(m);
         result = 0;
      } else {
fprintf(stderr, "*Received non-message opcode %d\n", opcode);
         *next_opcode = opcode;
         result = 1;
      }
   }

   return MPI_SUCCESS;
}

int messaging_receive_comm_reply(comm_reply *reply)
{
   // Since operations on communicators are collective operations, we can
   // assume here that the reply has not be received yet. There is a chance,
   // however, that other messages are on the stream before our reply message.
   int opcode, i;

   int error = queue_pending_messages(&opcode);

   if (error != MPI_SUCCESS) {
      return error;
   }

   if (opcode != OPCODE_COMM_REPLY) {
      fprintf(stderr, "INTERNAL ERROR: unexpected message opcode (RC) %d\n", opcode);
      return MPI_ERR_INTERN;
   }

   error = wa_receivefully((unsigned char *) reply, COMM_REPLY_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "INTERNAL ERROR: Failed to receive comm reply!\n");
      return MPI_ERR_INTERN;
   }

   reply->comm = ntohl(reply->comm);
   reply->src = ntohl(reply->src);
   reply->newComm = ntohl(reply->newComm);
   reply->rank = ntohl(reply->rank);
   reply->size = ntohl(reply->size);
   reply->color = ntohl(reply->color);
   reply->key = ntohl(reply->key);
   reply->cluster_count = ntohl(reply->cluster_count);
   reply->flags = ntohl(reply->flags);

fprintf(stderr, "*Received comm reply (comm=%d src=%d newComm=%d rank=%d size=%d color=%d key=%d cluster_count=%d flag=%d)\n", reply->comm, reply->src, reply->newComm, reply->rank, reply->size, reply->color, reply->key, reply->cluster_count, reply->flags);

   reply->coordinators = malloc(reply->cluster_count * sizeof(int));

   if (reply->coordinators == NULL) {
      fprintf(stderr, "ERROR: Failed to allocate coordinators\n");
      return MPI_ERR_INTERN;
   }

   error = wa_receivefully((unsigned char *)(reply->coordinators), reply->cluster_count * sizeof(int));

   if (error != CONNECT_OK) {
      fprintf(stderr, "INTERNAL ERROR: Failed to receive comm reply coordinators!\n");
      return MPI_ERR_INTERN;
   }

fprintf(stderr, "*Received comm reply coordinators\n)");

   for (i=0;i<reply->cluster_count;i++) {
      reply->coordinators[i] = ntohl(reply->coordinators[i]);
      fprintf(stderr, "   *reply->coordinators[%d] = %d\n)", i, reply->coordinators[i]);
   }

   if (reply->size > 0) {
      reply->members = malloc(reply->size * sizeof(uint32_t));

      if (reply->members == NULL) {
         fprintf(stderr, "ERROR: Failed to allocate members\n");
         return MPI_ERR_INTERN;
      }

      error = wa_receivefully((unsigned char *)reply->members, reply->size * sizeof(uint32_t));

      if (error != CONNECT_OK) {
         fprintf(stderr, "INTERNAL ERROR: Failed to receive comm reply members!\n");
         return MPI_ERR_INTERN;
      }

      for (i=0;i<reply->size;i++) {
         reply->members[i] = ntohl(reply->members[i]);
      }
   }

   return MPI_SUCCESS;
}

int messaging_send_comm_request(communicator* c, int color, int key)
{
   comm_request req;

   req.opcode = htonl(OPCODE_COMM);
   req.comm = htonl(c->number);
   req.src = htonl(c->global_rank);
   req.color = htonl(color);
   req.key = htonl(key);

   int error = wa_sendfully((unsigned char *) &req, COMM_REQUEST_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "ERROR: Failed to send comm request!\n");
      return MPI_ERR_INTERN;
   }

   return MPI_SUCCESS;
}

int messaging_send_group_request(communicator* c, group *g)
{
   int i;
   group_request req;

   req.opcode = htonl(OPCODE_GROUP);
   req.comm = htonl(c->number);
   req.src = htonl(c->global_rank);
   req.size = htonl(g->size);

   int error = wa_sendfully((unsigned char *) &req, GROUP_REQUEST_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "ERROR: Failed to send comm request!\n");
      return MPI_ERR_INTERN;
   }

   for (i=0;i<g->size;i++) {
      g->members[i] = htonl(g->members[i]);
   }

   error = wa_sendfully((unsigned char *) g->members, g->size * sizeof(uint32_t));

   for (i=0;i<g->size;i++) {
      g->members[i] = ntohl(g->members[i]);
   }

   if (error != CONNECT_OK) {
      fprintf(stderr, "ERROR: Failed to send comm request!\n");
      return MPI_ERR_INTERN;
   }

   return MPI_SUCCESS;
}

int messaging_receive_group_reply(group_reply *reply)
{
   // Since operations on communicators are collective operations, we can
   // assume here that the reply has not be received yet. There is a chance,
   // however, that other messages are on the stream before our reply message.
   int opcode, i;

   int error = queue_pending_messages(&opcode);

   if (error != MPI_SUCCESS) {
      return error;
   }

   if (opcode != OPCODE_GROUP_REPLY) {
      fprintf(stderr, "INTERNAL ERROR: unexpected message opcode (RG) %d\n", opcode);
      return MPI_ERR_INTERN;
   }

fprintf(stderr, "*Receiving group reply %lu %lu %lu %lu\n", sizeof(group_reply), sizeof(unsigned char *), sizeof(group_reply)-sizeof(unsigned char *), GROUP_REPLY_SIZE);

   error = wa_receivefully((unsigned char *) reply, GROUP_REPLY_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "INTERNAL ERROR: Failed to receive group reply!\n");
      return MPI_ERR_INTERN;
   }

   reply->comm = ntohl(reply->comm);
   reply->src = ntohl(reply->src);
   reply->newComm = ntohl(reply->newComm);
   reply->rank = ntohl(reply->rank);
   reply->size = ntohl(reply->size);
   reply->type = ntohl(reply->type);
   reply->cluster_count = ntohl(reply->cluster_count);
   reply->flags = ntohl(reply->flags);

fprintf(stderr, "*Received group reply (comm=%d src=%d newComm=%d rank=%d size=%d type=%d cluster_count=%d flags=%d)\n", reply->comm, reply->src, reply->newComm, reply->rank, reply->size, reply->type, reply->cluster_count, reply->flags);

   if (reply->type == GROUP_TYPE_ACTIVE) {

fprintf(stderr, "*Receiving cluster coordinators (%ld bytes)\n", reply->cluster_count * sizeof(int));

      reply->coordinators = malloc(reply->cluster_count * sizeof(int));

      if (reply->coordinators == NULL) {
         fprintf(stderr, "ERROR: Failed to allocate coordinators\n");
         return MPI_ERR_INTERN;
      }

      error = wa_receivefully((unsigned char *)(reply->coordinators), reply->cluster_count * sizeof(int));

      if (error != CONNECT_OK) {
         fprintf(stderr, "INTERNAL ERROR: Failed to receive comm reply coordinators!\n");
         return MPI_ERR_INTERN;
      }

fprintf(stderr, "*Received comm reply coordinators\n)");

      for (i=0;i<reply->cluster_count;i++) {
         reply->coordinators[i] = ntohl(reply->coordinators[i]);
         fprintf(stderr, "   *reply->coordinators[%d] = %d\n)", i, reply->coordinators[i]);
      }

fprintf(stderr, "*Receiving group members (%ld bytes)\n", reply->size * sizeof(uint32_t));

      reply->members = malloc(reply->size * sizeof(uint32_t));

      if (reply->members == NULL) {
         fprintf(stderr, "ERROR: Failed to allocate member array!\n");
         return MPI_ERR_INTERN;
      }

      error = wa_receivefully((unsigned char *)reply->members, reply->size * sizeof(uint32_t));

      if (error != CONNECT_OK) {
         fprintf(stderr, "INTERNAL ERROR: Failed to receive comm reply members!\n");
         return MPI_ERR_INTERN;
      }

      for (i=0;i<reply->size;i++) {
         reply->members[i] = ntohl(reply->members[i]);
      }
   }

   return MPI_SUCCESS;
}

int messaging_send_dup_request(communicator* c)
{
   dup_request req;

   req.opcode = htonl(OPCODE_DUP);
   req.comm = htonl(c->number);
   req.src = htonl(c->global_rank);

   int error = wa_sendfully((unsigned char *) &req, DUP_REQUEST_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "ERROR: Failed to send dup request!\n");
      return MPI_ERR_INTERN;
   }

   return MPI_SUCCESS;
}

int messaging_receive_dup_reply(dup_reply *reply)
{
   // Since operations on communicators are collective operations, we can
   // assume here that the reply has not be received yet. There is a chance,
   // however, that other messages are on the stream before our reply message.
   int opcode;

   int error = queue_pending_messages(&opcode);

   if (error != MPI_SUCCESS) {
      return error;
   }

   if (opcode != OPCODE_DUP_REPLY) {
      fprintf(stderr, "INTERNAL ERROR: unexpected message opcode (RD) %d\n", opcode);
      return MPI_ERR_INTERN;
   }

   error = wa_receivefully((unsigned char *) reply, DUP_REPLY_SIZE);

   if (error != CONNECT_OK) {
      fprintf(stderr, "INTERNAL ERROR: Failed to receive group reply!\n");
      return MPI_ERR_INTERN;
   }

   reply->comm = ntohl(reply->comm);
   reply->src = ntohl(reply->src);
   reply->newComm = ntohl(reply->newComm);

fprintf(stderr, "*Received dup reply (comm=%d src=%d newComm=%d)\n", reply->comm, reply->src, reply->newComm);

   return MPI_SUCCESS;
}


#endif // IBIS_INTERCEPT
