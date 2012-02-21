
#ifndef _MPI_H_
#define _MPI_H_

/* Start by defining the various MPI opaque types */

typedef int MPI_Comm;
typedef int MPI_Request;
typedef int MPI_Group;
typedef int MPI_Datatype;
typedef int MPI_Op;
typedef int MPI_Errhandler;
typedef int MPI_Fint;

typedef unsigned long int MPI_Aint;

/* 
 * Next, we define MPI_Status, the only non-opaque type 
 * Must be compatible with the mpif.h values MPI_STATUS_SIZE, MPI_SOURCE, MPI_TAG, and MPI_ERROR. 
 * The type used for MPI_Status_int must be chosen to match Fortran INTEGER. 
 */

typedef int MPI_Status_int;

typedef struct                  /* Fortran: INTEGER status(MPI_STATUS_SIZE) */
{
  MPI_Status_int MPI_SOURCE;    /* Fortran: status(MPI_SOURCE) */
  MPI_Status_int MPI_TAG;       /* Fortran: status(MPI_TAG) */
  MPI_Status_int MPI_ERROR;     /* Fortran: status(MPI_ERROR) */

} MPI_Status;

/* Reserved communicators */

#define MPI_COMM_NULL  (0)      /* handle 0 maps to NULL */
#define MPI_COMM_WORLD (1)
#define MPI_COMM_SELF  (2)

/* Reserved groups -- MPI_GROUP_EMPTY and MPI_GROUP_NULL must not conflict with MPI_GROUP_ONE */

#define MPI_GROUP_NULL  (0)
#define MPI_GROUP_EMPTY (-1)  

/* Reserved requests */

#define MPI_REQUEST_NULL (0)

/* Error codes */

#define MPI_SUCCESS        (0)
#define MPI_ERR_BUFFER     (1)
#define MPI_ERR_COUNT      (2)
#define MPI_ERR_TYPE       (3)
#define MPI_ERR_TAG        (4)
#define MPI_ERR_COMM       (5)
#define MPI_ERR_RANK       (6)
#define MPI_ERR_REQUEST    (7)
#define MPI_ERR_ROOT       (8)
#define MPI_ERR_GROUP      (9)
#define MPI_ERR_OP         (10)
#define MPI_ERR_TOPOLOGY   (11)
#define MPI_ERR_DIMS       (12)
#define MPI_ERR_ARG        (13)
#define MPI_ERR_UNKNOWN    (14)
#define MPI_ERR_TRUNCATE   (15)
#define MPI_ERR_OTHER      (16)
#define MPI_ERR_INTERN     (17)
#define MPI_PENDING        (18)
#define MPI_ERR_IN_STATUS  (19)
#define MPI_ERR_LASTCODE   (20)

/* Various constants */

#define MPI_BOTTOM     (0)
#define MPI_PROC_NULL  (-2)
#define MPI_ANY_SOURCE (-1)
#define MPI_ANY_TAG    (-1)
#define MPI_UNDEFINED (-1)

#define MPI_MAX_ERROR_STRING (128)
#define MPI_MAX_PROCESSOR_NAME (128)

/* The type's value is its size in bytes */

#define MPI_DATATYPE_NULL  (0)
#define MPI_BYTE           (sizeof(unsigned char))
#define MPI_CHAR           (sizeof(char))
#define MPI_UNSIGNED_CHAR  (sizeof(unsigned char))
#define MPI_SHORT          (sizeof(short))
#define MPI_UNSIGNED_SHORT (sizeof(unsigned short))
#define MPI_INT            (sizeof(int))
#define MPI_UNSIGNED       (sizeof(unsigned))
#define MPI_LONG           (sizeof(long))
#define MPI_UNSIGNED_LONG  (sizeof(unsigned long))
#define MPI_FLOAT          (sizeof(float))
#define MPI_DOUBLE         (sizeof(double))
#define MPI_LONG_DOUBLE    (sizeof(long double))
#define MPI_PACKED         (sizeof(char))

/* types for MINLOC and MAXLOC */

#define MPI_FLOAT_INT        (sizeof(struct{float a; int b;}))
#define MPI_DOUBLE_INT       (sizeof(struct{double a; int b;}))
#define MPI_LONG_INT         (sizeof(struct{long a; int b;}))
#define MPI_2INT             (sizeof(struct{int a; int b;}))
#define MPI_SHORT_INT        (sizeof (struct{short a; int b;}))
#define MPI_LONG_DOUBLE_INT  (sizeof (struct{long double a; int b;}))

/* Fortran specific types */

#define MPI_INTEGER           MPI_INT
#define MPI_REAL              MPI_FLOAT
#define MPI_DOUBLE_PRECISION  MPI_DOUBLE

#define MPI_INTEGER1       (1)
#define MPI_INTEGER2       (2)
#define MPI_INTEGER4       (4)
#define MPI_INTEGER8       (8)
#define MPI_INTEGER16      (16)

#define MPI_REAL4          (4)
#define MPI_REAL8          (8)
#define MPI_REAL16         (16)

#define MPI_STATUS_SIZE       (sizeof(MPI_Status) / sizeof(int))

/* Reduction operations */

#define MPI_OP_NULL (0)
#define MPI_MAX     (1)
#define MPI_MIN     (2)
#define MPI_SUM     (3)
#define MPI_PROD    (4)
#define MPI_LAND    (5)
#define MPI_BAND    (6)
#define MPI_LOR     (7)
#define MPI_BOR     (8)
#define MPI_LXOR    (9)
#define MPI_BXOR    (10)
#define MPI_MAXLOC  (11)
#define MPI_MINLOC  (12)

/* Prototypes for user-defined functions */

typedef int MPI_Copy_function(MPI_Comm oldcomm, int keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
typedef int MPI_Delete_function(MPI_Comm comm, int keyval, void *attribute_val, void *extra_state);
typedef void MPI_Handler_function(MPI_Comm *, int *, ...);
typedef void MPI_User_function(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype);


/*************** The MPI function prototypes ****************/

/* Initialization and termination */

extern int MPI_Init(int *argc, char ***argv);
extern int MPI_Finalize(void);
extern int MPI_Initialized(int *flag);
extern int MPI_Abort(MPI_Comm comm, int errorcode);


/* Point to point communication */

extern int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
extern int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
extern int MPI_Bsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
extern int MPI_Ssend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
extern int MPI_Rsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
extern int MPI_Buffer_attach( void* buffer, int size);
extern int MPI_Buffer_detach( void* buffer, int* size);
extern int MPI_Isend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Ibsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Issend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Irsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Irecv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Send_init(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Bsend_init(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Ssend_init(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Rsend_init(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Recv_init(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
extern int MPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
extern int MPI_Sendrecv_replace(void* buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status);


/* Asynchronous communication support */

extern int MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);
extern int MPI_Get_elements(MPI_Status *status, MPI_Datatype datatype, int *count);
extern int MPI_Wait(MPI_Request *request, MPI_Status *status);
extern int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status);
extern int MPI_Request_free(MPI_Request *request);
extern int MPI_Waitany(int count, MPI_Request *array of requests, int *index, MPI_Status *status);
extern int MPI_Testany(int count, MPI_Request *array of requests, int *index, int *flag, MPI_Status *status);
extern int MPI_Waitall(int count, MPI_Request *array of requests, MPI_Status *array of statuses);
extern int MPI_Testall(int count, MPI_Request *array of requests, int *flag, MPI_Status *array of statuses);
extern int MPI_Waitsome(int incount, MPI_Request *array of requests, int *outcount, int *array of indices, MPI_Status *array of statuses);
extern int MPI_Testsome(int incount, MPI_Request *array of requests, int *outcount, int *array of indices, MPI_Status *array of statuses);
extern int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);
extern int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
extern int MPI_Cancel(MPI_Request *request);
extern int MPI_Test_cancelled(MPI_Status *status, int *flag);
extern int MPI_Start(MPI_Request *request);
extern int MPI_Startall(int count, MPI_Request *array of requests);


/* Type definition */

extern int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_indexed(int count, int *array_of_blocklengths, int *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_hindexed(int count, int *array_of_blocklengths, MPI_Aint *array_of_ displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_struct(int count, int *array_of_blocklengths, MPI_Aint *array_of_displacements, MPI_Datatype *array_of_types, MPI_Datatype *newtype);
extern int MPI_Type_extent(MPI_Datatype datatype, MPI_Aint *extent);
extern int MPI_Type_size(MPI_Datatype datatype, int *size);
extern int MPI_Type_lb(MPI_Datatype datatype, MPI_Aint* displacement);
extern int MPI_Type_ub(MPI_Datatype datatype, MPI_Aint* displacement);
extern int MPI_Type_commit(MPI_Datatype *datatype);
extern int MPI_Type_free(MPI_Datatype *datatype);


/* Collectives */

extern int MPI_Barrier(MPI_Comm comm);
extern int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
extern int MPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
extern int MPI_Gatherv(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm);
extern int MPI_Scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
extern int MPI_Scatterv(void* sendbuf, int *sendcounts, int *displs, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
extern int MPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
extern int MPI_Allgatherv(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm);
extern int MPI_Alltoall(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
extern int MPI_Alltoallv(void* sendbuf, int *sendcounts, int *sdispls, MPI_Datatype sendtype, void* recvbuf, int *recvcounts, int *rdispls, MPI_Datatype recvtype, MPI_Comm comm);
extern int MPI_Reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
extern int MPI_Op_create(MPI_User function *function, int commute, MPI_Op *op);
extern int MPI_Op_free( MPI_Op *op);
extern int MPI_Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
extern int MPI_Reduce_scatter(void* sendbuf, void* recvbuf, int *recvcounts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
extern int MPI_Scan(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);


/* Communicators and groups */

extern int MPI_Group_size(MPI_Group group, int *size);
extern int MPI_Group_rank(MPI_Group group, int *rank);
extern int MPI_Group_translate_ranks(MPI_Group group1, int n, int *ranks1, MPI_Group group2, int *ranks2);
extern int MPI_Group_compare(MPI_Group group1,MPI_Group group2, int *result);
extern int MPI_Comm_group(MPI_Comm comm, MPI_Group *group);
extern int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
extern int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
extern int MPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
extern int MPI_Group_incl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);
extern int MPI_Group_excl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup); 
extern int MPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
extern int MPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
extern int MPI_Group_free(MPI_Group *group);
extern int MPI_Comm_size(MPI_Comm comm, int *size);
extern int MPI_Comm_rank(MPI_Comm comm, int *rank);
extern int MPI_Comm_compare(MPI_Comm comm1,MPI_Comm comm2, int *result);
extern int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);
extern int MPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);
extern int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
extern int MPI_Comm_free(MPI_Comm *comm);
extern int MPI_Comm_test inter(MPI_Comm comm, int *flag);
extern int MPI_Comm_remote_size(MPI_Comm comm, int *size);
extern int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);
extern int MPI_Intercomm_create(MPI_Comm local comm, int local leader, MPI_Comm peer comm, int remote leader, int tag, MPI_Comm *newintercomm);
extern int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintracomm);
extern int MPI_Keyval_create(MPI_Copy function *copy fn, MPI_Delete function *delete fn, int *keyval, void* extra state);
extern int MPI_Keyval_free(int *keyval);
extern int MPI_Attr_put(MPI_Comm comm, int keyval, void* attribute val);
extern int MPI_Attr_get(MPI_Comm comm, int keyval, void* attribute val, int *flag);
extern int MPI_Attr_delete(MPI_Comm comm, int keyval);


/* Process topology */

extern int MPI_Cart_create(MPI_Comm comm old, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm cart);
extern int MPI_Dims_create(int nnodes, int ndims, int *dims);
extern int MPI_Graph_create(MPI_Comm comm old, int nnodes, int *index, int *edges, int reorder, MPI_Comm *comm graph);
extern int MPI_Topo_test(MPI_Comm comm, int *status);
extern int MPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);
extern int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges);
extern int MPI_Cartdim_get(MPI_Comm comm, int *ndims);
extern int MPI_Cart_get(MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords);
extern int MPI_Cart_rank(MPI_Comm comm, int *coords, int *rank);
extern int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int *coords);
extern int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);
extern int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int *neighbors);
extern int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank source, int *rank dest);
extern int MPI_Cart_sub(MPI_Comm comm, int *remain dims, MPI_Comm *newcomm);
extern int MPI_Cart_map(MPI_Comm comm, int ndims, int *dims, int *periods, int *newrank);
extern int MPI_Graph_map(MPI_Comm comm, int nnodes, int *index, int *edges, int *newrank);


/* Environmental quiry */

extern int MPI_Get_version(int *version, int *subversion)
extern int MPI_Get_processor_name(char *name, int *resultlen)


/* Error handling */

extern int MPI_Errhandler_create(MPI_Handler function *function, MPI_Errhandler *errhandler);
extern int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);
extern int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);
extern int MPI_Errhandler_free(MPI_Errhandler *errhandler);
extern int MPI_Error_string(int errorcode, char *string, int *resultlen);
extern int MPI_Error_class(int errorcode, int *errorclass);


/* Various utilities */

extern int MPI_Address(void* location, MPI_Aint *address);
extern int MPI_Pack(void* inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm);
extern int MPI_Unpack(void* inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm);
extern int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size);

extern double MPI_Wtime(void);
extern double MPI_Wtick(void);

extern MPI_Comm MPI_Comm_f2c(MPI_Fint comm);
extern MPI_Fint MPI_Comm_c2f(MPI_Comm comm);

extern MPI_Group MPI_Group_f2c(MPI_Fint g);
extern MPI_Fint MPI_Group_c2f(MPI_Group g);

extern MPI_Request MPI_Request_f2c(MPI_Fint request);
extern MPI_Fint MPI_Request_c2f(MPI_Request request);

extern MPI_Op MPI_Op_f2c(MPI_Fint op);
extern MPI_Fint MPI_Op_c2f(MPI_Op op);

extern MPI_Errhandler MPI_Errhandler_f2c(MPI_Fint Errhandler);
extern MPI_Fint MPI_Errhandler_c2f(MPI_Errhandler Errhandler);

extern MPI_Datatype MPI_Type_f2c(MPI_Fint Type);
extern MPI_Fint MPI_Type_c2f(MPI_Datatype Type);

/* Profiling */

extern int MPI_Pcontrol(const int level, ...);

#endif
