
#ifndef _IMPI_H_
#define _IMPI_H_


/************************************************************************************ 
 * This file defines the various constants, types and functions offered by MPI 1.3. *
 *                                                                                  *
 * As this MPI implementation is designed to run on top of an existing MPI, we have *                       
 * to ensure we don't have a clash between our identifiers and the identifiers used * 
 * in the underlying MPI. We therefore use the prefix "IMPI_" in all identifiers.   *
 ************************************************************************************/

/* Start by defining the MPI version */

#define IMPI_VERSION    (1)
#define IMPI_SUBVERSION (3)


/* Next, define the various MPI opaque types */

typedef int IMPI_Comm;
typedef int IMPI_Request;
typedef int IMPI_Group;
typedef int IMPI_Datatype;
typedef int IMPI_Op;
typedef int IMPI_Errhandler;
typedef int IMPI_Fint;

typedef unsigned long int IMPI_Aint;


/************************************************************************************
 * Next, we define IMPI_Status, the only non-opaque type of MPI 1.3. This struct    *
 * must be compatible with the mpif.h values IMPI_STATUS_SIZE, IMPI_SOURCE,         *
 * IMPI_TAG, and IMPI_ERROR. The type used for IMPI_Status_int must be chosen to    *
 * match Fortran INTEGER.                                                           *
 ************************************************************************************/

typedef int IMPI_Status_int;

typedef struct                  /* Fortran: INTEGER status(IMPI_STATUS_SIZE) */
{
  IMPI_Status_int MPI_SOURCE;    /* Fortran: status(IMPI_SOURCE) */
  IMPI_Status_int MPI_TAG;       /* Fortran: status(IMPI_TAG) */
  IMPI_Status_int MPI_ERROR;     /* Fortran: status(IMPI_ERROR) */

} IMPI_Status;


/************************************************************************************
 * Next, we define the various constants used by MPI 1.3. Note that their values    *
 * may differ in value and type from the underlying MPI implementation              *
 ************************************************************************************/

/* Return codes (used in C and Fortran) */

#define IMPI_SUCCESS        (0)
#define IMPI_ERR_BUFFER     (1)
#define IMPI_ERR_COUNT      (2)
#define IMPI_ERR_TYPE       (3)
#define IMPI_ERR_TAG        (4)
#define IMPI_ERR_COMM       (5)
#define IMPI_ERR_RANK       (6)
#define IMPI_ERR_REQUEST    (7)
#define IMPI_ERR_ROOT       (8)
#define IMPI_ERR_GROUP      (9)
#define IMPI_ERR_OP         (10)
#define IMPI_ERR_TOPOLOGY   (11)
#define IMPI_ERR_DIMS       (12)
#define IMPI_ERR_ARG        (13)
#define IMPI_ERR_UNKNOWN    (14)
#define IMPI_ERR_TRUNCATE   (15)
#define IMPI_ERR_OTHER      (16)
#define IMPI_ERR_INTERN     (17)
#define IMPI_ERR_PENDING    (18)
#define IMPI_ERR_IN_STATUS  (19)
#define IMPI_ERR_LASTCODE   (20)


/* Assorted constants (used in C and Fortran) */

#define IMPI_BOTTOM         (0)
#define IMPI_PROC_NULL      (-2)
#define IMPI_ANY_SOURCE     (-1)
#define IMPI_ANY_TAG        (-1)
#define IMPI_UNDEFINED      (-1)
#define IMPI_BSEND_OVERHEAD (0)
#define IMPI_KEYVAL_INVALID (-1)


/* Status size and reserved index values (Fortran) */

#define IMPI_STATUS_SIZE    (sizeof(IMPI_Status) / sizeof(int))
#define IMPI_SOURCE         (1)
#define IMPI_TAG            (2)
#define IMPI_ERROR          (3)


/* Error-handling specifiers (C and Fortran) */

#define IMPI_ERRORS_ARE_FATAL (0)
#define IMPI_ERRORS_RETURN    (1)


/* Maximum sizes for strings */

#define IMPI_MAX_ERROR_STRING (128)
#define IMPI_MAX_PROCESSOR_NAME (128)


/* Elementary datatypes (C) */

#define IMPI_DATATYPE_NULL     (0)
#define IMPI_CHAR              (1)
#define IMPI_SHORT             (2)
#define IMPI_INT               (3)
#define IMPI_LONG              (4)
#define IMPI_UNSIGNED_CHAR     (5)
#define IMPI_UNSIGNED_SHORT    (6)
#define IMPI_UNSIGNED          (7)
#define IMPI_UNSIGNED_LONG     (8)
#define IMPI_FLOAT             (9)
#define IMPI_DOUBLE            (10)
#define IMPI_LONG_DOUBLE       (11)
#define IMPI_BYTE              (12)
#define IMPI_PACKED            (13)

/* Elementary datatypes (Fortran) */

#define IMPI_INTEGER           (14)
#define IMPI_REAL              (15)
#define IMPI_DOUBLE_PRECISION  (16)
#define IMPI_COMPLEX           (17)
#define IMPI_LOGICAL           (18)
#define IMPI_CHARACTER         (19)

/* Datatypes for reduction functions (C) */

#define IMPI_FLOAT_INT         (20) 
#define IMPI_DOUBLE_INT        (21) 
#define IMPI_LONG_INT          (22)
#define IMPI_2INT              (23)
#define IMPI_SHORT_INT         (24)
#define IMPI_LONG_DOUBLE_INT   (25)

/* Datatypes for reduction functions (Fortran) */

#define IMPI_2REAL             (26)
#define IMPI_2DOUBLE_PRECISION (27)
#define IMPI_2INTEGER          (28)

/* Optional datatypes (C) */

#define IMPI_LONG_LONG_INT     (29)
#define IMPI_LONG_LONG         (30)

/* Optional datatypes (Fortran) */

#define IMPI_DOUBLE_COMPLEX    (31)

#define IMPI_INTEGER1          (32)
#define IMPI_INTEGER2          (33)
#define IMPI_INTEGER4          (34)
#define IMPI_INTEGER8          (35)

#define IMPI_REAL2             (36)
#define IMPI_REAL4             (37)
#define IMPI_REAL8             (38)


#define IMPI_MAX_USER_DATATYPES (32)  
#define IMPI_LAST_DATATYPE      (39+IMPI_MAX_USER_DATATYPES)


/* Special datatypes for constructing derived datatypes */

#define IMPI_UB (-1)
#define IMPI_LB (-2)


/* Reserved communicators */

#define IMPI_COMM_NULL  (0)
#define IMPI_COMM_WORLD (1)
#define IMPI_COMM_SELF  (2)


/* Results of communicator and group comparisons */

#define IMPI_IDENT      (0)
#define IMPI_CONGRUENT  (1)
#define IMPI_SIMILAR    (2) 
#define IMPI_UNEQUAL    (3)


/* Environmental inquiry keys (C and Fortran) */

#define IMPI_TAG_UB          (32768)
#define IMPI_IO              (IMPI_ANY_SOURCE)
#define IMPI_HOST            (IMPI_PROC_NULL)
#define IMPI_WTIME_IS_GLOBAL (0)


/* Collective operations (C and Fortran) */

#define IMPI_OP_NULL (0)
#define IMPI_MAX     (1)
#define IMPI_MIN     (2)
#define IMPI_SUM     (3)
#define IMPI_PROD    (4)
#define IMPI_LAND    (5)
#define IMPI_BAND    (6)
#define IMPI_LOR     (7)
#define IMPI_BOR     (8)
#define IMPI_LXOR    (9)
#define IMPI_BXOR    (10)
#define IMPI_MAXLOC  (11)
#define IMPI_MINLOC  (12)


/* Reserved groups */

#define IMPI_GROUP_NULL  (0)
#define IMPI_GROUP_EMPTY (-1)  


/* Reserved requests */

#define IMPI_REQUEST_NULL (0)


/* Topologies (C and Fortran) -- Must not clash with IMPI_UNDEFINED */

#define IMPI_GRAPH  (0)
#define IMPI_CART   (1)


/* Prototypes for user-defined functions */

// NOT SUPPORTED due to typecasting issues
//
// typedef void IMPI_Handler_function(IMPI_Comm *, int *, ...);
// typedef int IMPI_Copy_function(IMPI_Comm oldcomm, int keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
// typedef int IMPI_Delete_function(IMPI_Comm comm, int keyval, void *attribute_val, void *extra_state);
// typedef void IMPI_User_function(void *invec, void *inoutvec, int *len, IMPI_Datatype *datatype);

/* Predefined functions in C and Fortran */

extern int IMPI_DUP_FN(IMPI_Comm oldcomm, int keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
extern int IMPI_NULL_COPY_FN(IMPI_Comm oldcomm, int keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
extern int IMPI_NULL_DELETE_FN(IMPI_Comm comm, int keyval, void *attribute_val, void *extra_state);


/*************** The MPI function prototypes ****************/


/* Initialization and termination */

extern int IMPI_Init(int *argc, char ***argv);
extern int IMPI_Finalize(void);
extern int IMPI_Initialized(int *flag);
extern int IMPI_Abort(IMPI_Comm comm, int errorcode);


/* Point to point communication */

extern int IMPI_Send(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm);
extern int IMPI_Recv(void* buf, int count, IMPI_Datatype datatype, int source, int tag, IMPI_Comm comm, IMPI_Status *status);
extern int IMPI_Bsend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm);
extern int IMPI_Ssend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm);
extern int IMPI_Rsend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm);
extern int IMPI_Buffer_attach( void* buffer, int size);
extern int IMPI_Buffer_detach( void* buffer, int* size);
extern int IMPI_Isend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Ibsend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Issend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Irsend(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Irecv(void* buf, int count, IMPI_Datatype datatype, int source, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Send_init(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Bsend_init(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Ssend_init(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Rsend_init(void* buf, int count, IMPI_Datatype datatype, int dest, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Recv_init(void* buf, int count, IMPI_Datatype datatype, int source, int tag, IMPI_Comm comm, IMPI_Request *request);
extern int IMPI_Sendrecv(void *sendbuf, int sendcount, IMPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, IMPI_Datatype recvtype, int source, int recvtag, IMPI_Comm comm, IMPI_Status *status);
extern int IMPI_Sendrecv_replace(void* buf, int count, IMPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, IMPI_Comm comm, IMPI_Status *status);


/* Asynchronous communication support */

extern int IMPI_Get_count(IMPI_Status *status, IMPI_Datatype datatype, int *count);
extern int IMPI_Get_elements(IMPI_Status *status, IMPI_Datatype datatype, int *count);
extern int IMPI_Wait(IMPI_Request *request, IMPI_Status *status);
extern int IMPI_Test(IMPI_Request *request, int *flag, IMPI_Status *status);
extern int IMPI_Request_free(IMPI_Request *request);
extern int IMPI_Waitany(int count, IMPI_Request *array_of_requests, int *index, IMPI_Status *status);
extern int IMPI_Testany(int count, IMPI_Request *array_of_requests, int *index, int *flag, IMPI_Status *status);
extern int IMPI_Waitall(int count, IMPI_Request *array_of_requests, IMPI_Status *array_of_statuses);
extern int IMPI_Testall(int count, IMPI_Request *array_of_requests, int *flag, IMPI_Status *array_of_statuses);
extern int IMPI_Waitsome(int incount, IMPI_Request *array_of_requests, int *outcount, int *array_of_indices, IMPI_Status *array_of_statuses);
extern int IMPI_Testsome(int incount, IMPI_Request *array_of_requests, int *outcount, int *array_of_indices, IMPI_Status *array_of_statuses);
extern int IMPI_Iprobe(int source, int tag, IMPI_Comm comm, int *flag, IMPI_Status *status);
extern int IMPI_Probe(int source, int tag, IMPI_Comm comm, IMPI_Status *status);
extern int IMPI_Cancel(IMPI_Request *request);
extern int IMPI_Test_cancelled(IMPI_Status *status, int *flag);
extern int IMPI_Start(IMPI_Request *request);
extern int IMPI_Startall(int count, IMPI_Request *array_of_requests);


/* Type definition */

extern int IMPI_Type_contiguous(int count, IMPI_Datatype oldtype, IMPI_Datatype *newtype);
extern int IMPI_Type_vector(int count, int blocklength, int stride, IMPI_Datatype oldtype, IMPI_Datatype *newtype);
extern int IMPI_Type_hvector(int count, int blocklength, IMPI_Aint stride, IMPI_Datatype oldtype, IMPI_Datatype *newtype);
extern int IMPI_Type_indexed(int count, int *array_of_blocklengths, int *array_of_displacements, IMPI_Datatype oldtype, IMPI_Datatype *newtype);
extern int IMPI_Type_hindexed(int count, int *array_of_blocklengths, IMPI_Aint *array_of_displacements, IMPI_Datatype oldtype, IMPI_Datatype *newtype);
extern int IMPI_Type_struct(int count, int *array_of_blocklengths, IMPI_Aint *array_of_displacements, IMPI_Datatype *array_of_types, IMPI_Datatype *newtype);
extern int IMPI_Type_extent(IMPI_Datatype datatype, IMPI_Aint *extent);
extern int IMPI_Type_size(IMPI_Datatype datatype, int *size);
extern int IMPI_Type_lb(IMPI_Datatype datatype, IMPI_Aint* displacement);
extern int IMPI_Type_ub(IMPI_Datatype datatype, IMPI_Aint* displacement);
extern int IMPI_Type_commit(IMPI_Datatype *datatype);
extern int IMPI_Type_free(IMPI_Datatype *datatype);

/* Collectives */

extern int IMPI_Barrier(IMPI_Comm comm);
extern int IMPI_Bcast(void* buffer, int count, IMPI_Datatype datatype, int root, IMPI_Comm comm);
extern int IMPI_Gather(void* sendbuf, int sendcount, IMPI_Datatype sendtype, void* recvbuf, int recvcount, IMPI_Datatype recvtype, int root, IMPI_Comm comm);
extern int IMPI_Gatherv(void* sendbuf, int sendcount, IMPI_Datatype sendtype, void* recvbuf, int *recvcounts, int *displs, IMPI_Datatype recvtype, int root, IMPI_Comm comm);
extern int IMPI_Scatter(void* sendbuf, int sendcount, IMPI_Datatype sendtype, void* recvbuf, int recvcount, IMPI_Datatype recvtype, int root, IMPI_Comm comm);
extern int IMPI_Scatterv(void* sendbuf, int *sendcounts, int *displs, IMPI_Datatype sendtype, void* recvbuf, int recvcount, IMPI_Datatype recvtype, int root, IMPI_Comm comm);
extern int IMPI_Allgather(void* sendbuf, int sendcount, IMPI_Datatype sendtype, void* recvbuf, int recvcount, IMPI_Datatype recvtype, IMPI_Comm comm);
extern int IMPI_Allgatherv(void* sendbuf, int sendcount, IMPI_Datatype sendtype, void* recvbuf, int *recvcounts, int *displs, IMPI_Datatype recvtype, IMPI_Comm comm);
extern int IMPI_Alltoall(void* sendbuf, int sendcount, IMPI_Datatype sendtype, void* recvbuf, int recvcount, IMPI_Datatype recvtype, IMPI_Comm comm);
extern int IMPI_Alltoallv(void* sendbuf, int *sendcounts, int *sdispls, IMPI_Datatype sendtype, void* recvbuf, int *recvcounts, int *rdispls, IMPI_Datatype recvtype, IMPI_Comm comm);
extern int IMPI_Reduce(void* sendbuf, void* recvbuf, int count, IMPI_Datatype datatype, IMPI_Op op, int root, IMPI_Comm comm);
extern int IMPI_Allreduce(void* sendbuf, void* recvbuf, int count, IMPI_Datatype datatype, IMPI_Op op, IMPI_Comm comm);
extern int IMPI_Reduce_scatter(void* sendbuf, void* recvbuf, int *recvcounts, IMPI_Datatype datatype, IMPI_Op op, IMPI_Comm comm);
extern int IMPI_Scan(void* sendbuf, void* recvbuf, int count, IMPI_Datatype datatype, IMPI_Op op, IMPI_Comm comm);

// NOT SUPPORTED due to type casting issues !
// extern int IMPI_Op_create(IMPI_User_function *function, int commute, IMPI_Op *op);
// extern int IMPI_Op_free( IMPI_Op *op);

/* Communicators and groups */

extern int IMPI_Group_size(IMPI_Group group, int *size);
extern int IMPI_Group_rank(IMPI_Group group, int *rank);
extern int IMPI_Group_translate_ranks(IMPI_Group group1, int n, int *ranks1, IMPI_Group group2, int *ranks2);
extern int IMPI_Group_compare(IMPI_Group group1,IMPI_Group group2, int *result);
extern int IMPI_Comm_group(IMPI_Comm comm, IMPI_Group *group);
extern int IMPI_Group_union(IMPI_Group group1, IMPI_Group group2, IMPI_Group *newgroup);
extern int IMPI_Group_intersection(IMPI_Group group1, IMPI_Group group2, IMPI_Group *newgroup);
extern int IMPI_Group_difference(IMPI_Group group1, IMPI_Group group2, IMPI_Group *newgroup);
extern int IMPI_Group_incl(IMPI_Group group, int n, int *ranks, IMPI_Group *newgroup);
extern int IMPI_Group_excl(IMPI_Group group, int n, int *ranks, IMPI_Group *newgroup); 
extern int IMPI_Group_range_incl(IMPI_Group group, int n, int ranges[][3], IMPI_Group *newgroup);
extern int IMPI_Group_range_excl(IMPI_Group group, int n, int ranges[][3], IMPI_Group *newgroup);
extern int IMPI_Group_free(IMPI_Group *group);
extern int IMPI_Comm_size(IMPI_Comm comm, int *size);
extern int IMPI_Comm_rank(IMPI_Comm comm, int *rank);
extern int IMPI_Comm_compare(IMPI_Comm comm1,IMPI_Comm comm2, int *result);
extern int IMPI_Comm_dup(IMPI_Comm comm, IMPI_Comm *newcomm);
extern int IMPI_Comm_create(IMPI_Comm comm, IMPI_Group group, IMPI_Comm *newcomm);
extern int IMPI_Comm_split(IMPI_Comm comm, int color, int key, IMPI_Comm *newcomm);
extern int IMPI_Comm_free(IMPI_Comm *comm);
extern int IMPI_Comm_test_inter(IMPI_Comm comm, int *flag);
extern int IMPI_Comm_remote_size(IMPI_Comm comm, int *size);
extern int IMPI_Comm_remote_group(IMPI_Comm comm, IMPI_Group *group);
extern int IMPI_Intercomm_create(IMPI_Comm local_comm, int local_leader, IMPI_Comm peer_comm, int remote_leader, int tag, IMPI_Comm *newintercomm);
extern int IMPI_Intercomm_merge(IMPI_Comm intercomm, int high, IMPI_Comm *newintracomm);

extern int IMPI_Attr_put(IMPI_Comm comm, int keyval, void* attribute_val);
extern int IMPI_Attr_get(IMPI_Comm comm, int keyval, void* attribute_val, int *flag);
extern int IMPI_Attr_delete(IMPI_Comm comm, int keyval);

// NOT SUPPORTED 
// extern int IMPI_Keyval_create(IMPI_Copy_function *copy_fn, IMPI_Delete_function *delete_fn, int *keyval, void* extra_state);
// extern int IMPI_Keyval_free(int *keyval);

/* Process topology */

extern int IMPI_Cart_create(IMPI_Comm comm_old, int ndims, int *dims, int *periods, int reorder, IMPI_Comm *comm_cart);
extern int IMPI_Dims_create(int nnodes, int ndims, int *dims);
extern int IMPI_Graph_create(IMPI_Comm comm_old, int nnodes, int *index, int *edges, int reorder, IMPI_Comm *comm_graph);
extern int IMPI_Topo_test(IMPI_Comm comm, int *status);
extern int IMPI_Graphdims_get(IMPI_Comm comm, int *nnodes, int *nedges);
extern int IMPI_Graph_get(IMPI_Comm comm, int maxindex, int maxedges, int *index, int *edges);
extern int IMPI_Cartdim_get(IMPI_Comm comm, int *ndims);
extern int IMPI_Cart_get(IMPI_Comm comm, int maxdims, int *dims, int *periods, int *coords);
extern int IMPI_Cart_rank(IMPI_Comm comm, int *coords, int *rank);
extern int IMPI_Cart_coords(IMPI_Comm comm, int rank, int maxdims, int *coords);
extern int IMPI_Graph_neighbors_count(IMPI_Comm comm, int rank, int *nneighbors);
extern int IMPI_Graph_neighbors(IMPI_Comm comm, int rank, int maxneighbors, int *neighbors);
extern int IMPI_Cart_shift(IMPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
extern int IMPI_Cart_sub(IMPI_Comm comm, int *remain_dims, IMPI_Comm *newcomm);
extern int IMPI_Cart_map(IMPI_Comm comm, int ndims, int *dims, int *periods, int *newrank);
extern int IMPI_Graph_map(IMPI_Comm comm, int nnodes, int *index, int *edges, int *newrank);


/* Environmental quiry */

extern int IMPI_Get_version(int *version, int *subversion);
extern int IMPI_Get_processor_name(char *name, int *resultlen);


/* Error handling */

// NOT SUPPORTED due to type casting issues!
//
// extern int IMPI_Errhandler_create(IMPI_Handler_function *function, IMPI_Errhandler *errhandler);
// extern int IMPI_Errhandler_set(IMPI_Comm comm, IMPI_Errhandler errhandler);
// extern int IMPI_Errhandler_get(IMPI_Comm comm, IMPI_Errhandler *errhandler);
// extern int IMPI_Errhandler_free(IMPI_Errhandler *errhandler);
extern int IMPI_Error_string(int errorcode, char *string, int *resultlen);
extern int IMPI_Error_class(int errorcode, int *errorclass);

/* Various utilities */

extern int IMPI_Address(void* location, IMPI_Aint *address);
extern int IMPI_Pack(void* inbuf, int incount, IMPI_Datatype datatype, void *outbuf, int outsize, int *position, IMPI_Comm comm);
extern int IMPI_Unpack(void* inbuf, int insize, int *position, void *outbuf, int outcount, IMPI_Datatype datatype, IMPI_Comm comm);
extern int IMPI_Pack_size(int incount, IMPI_Datatype datatype, IMPI_Comm comm, int *size);

extern double IMPI_Wtime(void);
extern double IMPI_Wtick(void);

extern IMPI_Comm IMPI_Comm_f2c(IMPI_Fint comm);
extern IMPI_Fint IMPI_Comm_c2f(IMPI_Comm comm);

extern IMPI_Group IMPI_Group_f2c(IMPI_Fint g);
extern IMPI_Fint IMPI_Group_c2f(IMPI_Group g);

extern IMPI_Request IMPI_Request_f2c(IMPI_Fint request);
extern IMPI_Fint IMPI_Request_c2f(IMPI_Request request);

extern IMPI_Op IMPI_Op_f2c(IMPI_Fint op);
extern IMPI_Fint IMPI_Op_c2f(IMPI_Op op);

extern IMPI_Errhandler IMPI_Errhandler_f2c(IMPI_Fint Errhandler);
extern IMPI_Fint IMPI_Errhandler_c2f(IMPI_Errhandler Errhandler);

extern IMPI_Datatype IMPI_Type_f2c(IMPI_Fint Type);
extern IMPI_Fint IMPI_Type_c2f(IMPI_Datatype Type);

/* Profiling */

//extern int IMPI_Pcontrol(const int level, ...);

#endif
