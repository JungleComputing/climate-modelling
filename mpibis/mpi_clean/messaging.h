#ifndef _MESSAGING_H_
#define _MESSAGING_H_

#include "flags.h"

#include "impi.h"
#include "types.h"

// requests

#define COMM_REQUEST_SIZE (5*sizeof(int))

typedef struct s_comm_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
    int color;   // target new communicator
    int key;     // prefered rank in target communicator
} comm_request;


#define GROUP_REQUEST_SIZE (4*sizeof(int))

typedef struct s_group_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
    int size;    // number of ranks in group
    // int *ranks // ranks for group
} group_request;


#define DUP_REQUEST_SIZE (3*sizeof(int))

typedef struct s_dup_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
} dup_request;

#define TERMINATE_REQUEST_SIZE (3*sizeof(int))

typedef struct s_terminate_request {
    int opcode;  // type of message
    int comm;    // communicator used
    int src;     // rank in current communicator
} terminate_request;

// replies


#define COMM_REPLY_SIZE (9*sizeof(int))

typedef struct s_comm_reply {
    int comm;           // communicator used
    int src;            // source rank (unused)
    int newComm;        // communicator created
    int rank;           // rank in new communicator
    int size;           // size of new communicator
    int color;          // color for local split
    int key;            // key for local split
    int cluster_count;  // number of cluster in communicator
    int flags;          // flags of new communicator
    int *coordinators;  // cluster coordinators
    int *cluster_sizes; // cluster sizes
    uint32_t *members;  // communicator members
} comm_reply;


#define GROUP_REPLY_SIZE (8*sizeof(int))

// TODO: merge with s_comm_reply ?
typedef struct s_group_reply {
    int comm;           // communicator used
    int src;            // source rank (unused)
    int newComm;        // communicator created
    int rank;           // rank in new communicator
    int size;           // size of new communicator
    int type;           // type of groupreply
    int cluster_count;  // number of clusters in communicator
    int flags;          // flags of new communicator
    int *coordinators;  // cluster coordinators
    int *cluster_sizes; // cluster sizes
    uint32_t *members;  // communicator members
} group_reply;


#define DUP_REPLY_SIZE (3*sizeof(int))

// TODO: merge with s_comm_reply ?
typedef struct s_dup_reply {
    int comm;    // communicator used
    int src;     // source rank (unused)
    int newComm; // communicator created
} dup_reply;

// mesg

#define MSG_HEADER_SIZE (7*sizeof(int))

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

// Send and receive messages (to any remote participant).
int messaging_send(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, communicator* c);
int messaging_receive(void *buf, int count, IMPI_Datatype datatype, int source, int tag, IMPI_Status *status, communicator* c);

// Broadcast messages (to all cluster coordinators).
int messaging_bcast(void* buf, int count, IMPI_Datatype datatype, int root, communicator* c);
int messaging_bcast_receive(void *buf, int count, IMPI_Datatype datatype, int root, communicator* c);

// Probe if a message is available.
int messaging_probe_receive(request *r, int blocking);

// Finalize a pending receive request.
int messaging_finalize_receive(request *r, IMPI_Status *status);

// Send and receive functions used to implement an MPI_COMM_SPLIT
int messaging_send_comm_request(communicator* c, int color, int key);
int messaging_receive_comm_reply(comm_reply *reply);

// Send and receive functions used to implement an MPI_COMM_CREATE
int messaging_send_group_request(communicator* c, group *g);
int messaging_receive_group_reply(group_reply *reply);

// Send and receive functions used to implement an MPI_COMM_DUP
int messaging_send_dup_request(communicator* c);
int messaging_receive_dup_reply(dup_reply *reply);

// Send and receive functions used to implement an MPI_COMM_FREE and MPI_FINALIZE
int messaging_send_terminate_request(communicator* c);

#endif // _MESSAGING_H_