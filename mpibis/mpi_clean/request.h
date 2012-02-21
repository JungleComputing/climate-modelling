#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "flags.h"

#include "impi.h"
#include "types.h"

// NOTE: This is the underlying MPI implementation!
#include "mpi.h"

#define REQUEST_FLAG_ACTIVE     (1 << 0)
#define REQUEST_FLAG_PERSISTENT (1 << 1)
#define REQUEST_FLAG_COMPLETED  (1 << 2)
#define REQUEST_FLAG_LOCAL      (1 << 3)
#define REQUEST_FLAG_SEND       (1 << 4)
#define REQUEST_FLAG_RECEIVE    (1 << 5)
#define REQUEST_FLAG_UNPACKED   (1 << 6)

struct s_request {
     // Status flags (see below)
     int flags;

     // These contain the call
     // parameters and return value
     void *buf;
     IMPI_Datatype type;
     int count;
     int source_or_dest;
     int tag;
     communicator *c;
     int error;

     int index;

     // This is the underlying MPI_Request
     MPI_Request req;

     // This is the matching message
     message_buffer *message;
};

int init_request();

request *create_request(int flags, void *buf, int count, IMPI_Datatype datatype, int dest, int tag, communicator *c);

void free_request(request *r);

request *get_request(IMPI_Request request);
request *get_request_with_index(int index);
void set_request_ptr(IMPI_Request *dst, request *src);

int request_active(request *r);
int request_local(request *r);
int request_persistent(request *r);
int request_send(request *r);
int request_receive(request *r);
int request_completed(request *r);

#endif // _REQUEST_H_
