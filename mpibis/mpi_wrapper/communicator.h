#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

#include "flags.h"

#ifdef IBIS_INTERCEPT

#include "mpi.h"
#include "types.h"

#define GATHER_STATISTICS 1

#define STATS_BARRIER   0
#define STATS_SEND      1
#define STATS_RECV      2
#define STATS_ISEND     3
#define STATS_IRECV     4
#define STATS_BCAST     5
#define STATS_SCATTER   6
#define STATS_GATHER    7
#define STATS_ALLGATHER 8
#define STATS_ALLTOALL  9
#define STATS_REDUCE    10
#define STATS_ALLREDUCE 11
#define STATS_SCAN      12
#define STATS_TOTAL     13

#define STATS_NAME_BARRIER   "barrier"
#define STATS_NAME_SEND      "send"
#define STATS_NAME_RECV      "receive"
#define STATS_NAME_ISEND     "isend"
#define STATS_NAME_IRECV     "ireceive"
#define STATS_NAME_BCAST     "bcast"
#define STATS_NAME_SCATTER   "scatter"
#define STATS_NAME_GATHER    "gather"
#define STATS_NAME_ALLGATHER "allgather"
#define STATS_NAME_ALLTOALL  "alltoall"
#define STATS_NAME_REDUCE    "reduce"
#define STATS_NAME_ALLREDUCE "allreduce"
#define STATS_NAME_SCAN      "scan"

#define STATS_OK        0
#define STATS_ERROR     1
#define STATS_NOT_FOUND 2

#define COMM_FLAG_LOCAL  (1 << 0)
#define COMM_FLAG_REMOTE (1 << 1)
#define COMM_FLAG_WORLD  (1 << 2)
#define COMM_FLAG_SELF   (1 << 3)

#define COMM_OK                  0
#define COMM_ERROR_ALLOCATE      1
#define COMM_ERROR_COMMUNICATORS 2

struct s_communicator {

   int number;
   int flags;

   MPI_Comm comm;

   int local_rank;
   int local_size;

   int global_rank;
   int global_size;

   uint32_t *members;

//   unsigned char *bitmap;

   message_buffer *queue_head;
   message_buffer *queue_tail;

#if GATHER_STATISTICS
   unsigned long counters[STATS_TOTAL];
#endif

};

// Communicator mapping

int init_communicators(int cluster_rank, int cluster_count,
                       int* cluster_sizes, int *cluster_offsets);

int create_communicator(MPI_Comm comm, int number, int local_rank, int local_size,
         int global_rank, int global_size, int flags, uint32_t *members, communicator **out);

int free_communicator(communicator * c);

communicator *get_communicator(MPI_Comm comm);
communicator *get_communicator_with_index(int index);

void set_communicator_ptr(MPI_Comm *dst, communicator *src);

int comm_dup(communicator *in, communicator **out);

int free_communicator(communicator *c);
int rank_is_local(communicator *c, int rank, int *result);
int comm_is_world(communicator* c);
int comm_is_self(communicator* c);
int comm_is_local(communicator* c);
int comm_is_wa(communicator* c);
int comm_is_mixed(communicator* c);
void store_message(message_buffer *m);
message_buffer *find_pending_message(communicator *c, int source, int tag);
int match_message(message_buffer *m, int comm, int source, int tag);

int inc_communicator_statistics(MPI_Comm comm, int field);
int print_communicator_statistics(MPI_Comm comm);
int print_all_communicator_statistics();

#endif // IBIS_INTERCEPT

#endif // _COMMUNICATOR_H_
