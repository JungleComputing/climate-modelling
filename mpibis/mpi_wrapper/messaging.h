#ifndef _MESSAGING_H_
#define _MESSAGING_H_

#include "flags.h"

#ifdef IBIS_INTERCEPT


#include "mpi.h"
#include "types.h"

#define COMM_REQUEST_SIZE (5*sizeof(int))
#define COMM_REPLY_SIZE (8*sizeof(int))

#define GROUP_REQUEST_SIZE (4*sizeof(int))
#define GROUP_REPLY_SIZE (7*sizeof(int))

#define DUP_REQUEST_SIZE (3*sizeof(int))
#define DUP_REPLY_SIZE (3*sizeof(int))

#define MSG_HEADER_SIZE (7*sizeof(int))

// requests

typedef struct s_comm_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
    int color;   // target new communicator
    int key;     // prefered rank in target communicator
} comm_request;

typedef struct s_group_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
    int size;    // number of ranks in group
    // int *ranks // ranks for group
} group_request;

typedef struct s_dup_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
} dup_request;

// replies

typedef struct s_comm_reply {
    int comm;          // communicator used
    int src;           // source rank (unused)
    int newComm;       // communicator created
    int rank;          // rank in new communicator
    int size;          // size of new communicator
    int color;         // color for local split
    int key;           // key for local split
    int cluster_count; // number of cluster in communicator
    int flags;         // flags of new communicator
    uint32_t *members; // communicator members
} comm_reply;

// TODO: merge with s_comm_reply ?
typedef struct s_group_reply {
    int comm;          // communicator used
    int src;           // source rank (unused)
    int newComm;       // communicator created
    int rank;          // rank in new communicator
    int size;          // size of new communicator
    int type;          // type of groupreply
    int cluster_count; // number of cluster in communicator
    int flags;         // flags of new communicator
    uint32_t *members; // communicator members
} group_reply;

// TODO: merge with s_comm_reply ?
typedef struct s_dup_reply {
    int comm;    // communicator used
    int src;     // source rank (unused)
    int newComm; // communicator created
} dup_reply;

// mesg

typedef struct s_msg_header {
    int opcode;  // type of message
    int comm;    // communicator used
    int source;  // source rank
    int dest;    // destination rank
    int tag;     // message tag
    int count;   // data size in elements
    int bytes;   // data size in bytes
} message_header;

struct s_msg_buffer {
    message_header header;      // Message header
    int byte_order;             // Current byte-order of header
    int data_buffer_size;       // Message buffer size
    unsigned char *data_buffer; // Message buffer

    message_buffer *next;       // Next message in queue
};

int messaging_send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, communicator* c);
int messaging_receive(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Status *status, communicator* c);

int messaging_bcast(void* buf, int count, MPI_Datatype datatype, int root, communicator* c);
int messaging_bcast_receive(void *buf, int count, MPI_Datatype datatype, int root, communicator* c);

int messaging_allreduce(void* buf, int count, MPI_Datatype datatype, communicator* c);
int messaging_allreduce_receive(void *buf, int count, MPI_Datatype datatype, communicator* c);

int messaging_probe_receive(request *r, int blocking);
int messaging_finalize_receive(request *r, MPI_Status *status);

int messaging_send_comm_request(communicator* c, int color, int key);
int messaging_receive_comm_reply(comm_reply *reply);


int messaging_send_group_request(communicator* c, group *g);
int messaging_receive_group_reply(group_reply *reply);


int messaging_send_dup_request(communicator* c);
int messaging_receive_dup_reply(dup_reply *reply);



//int  messaging_probe_receive_request(request *r);
//void messaging_complete_receive_request(request *r, MPI_Status *status);

#endif // IBIS_INTERCEPT

#endif // _MESSAGING_H_
