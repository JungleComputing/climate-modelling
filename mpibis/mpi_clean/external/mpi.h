
#ifndef _MPI_H_
#define _MPI_H_

#include "../impi.h"

/************************************************************************************ 
 * This file provides a wrapper for "impi.h" which defines the various constants,   *
 * types and functions offered by MPI 1.3.                                          *
 *                                                                                  *
 * This indirection is required because this MPI implementation is designed to run  *
 * on top of an existing MPI. To ensure we don't have a identifer clash between     *
 * these two MPI implementations, we internally use the prefix "IMPI_" in all       *
 * identifiers. This file ensures that the application can use the regular "MPI_"   *
 * prefixes instead.                                                                *
 ************************************************************************************/

/* Start by defining the MPI version */

#define MPI_VERSION (IMPI_VERSION)    
#define MPI_SUBVERSION (IMPI_SUBVERSION)


/* Next, define the various MPI types */

#define MPI_Comm       IMPI_Comm
#define MPI_Request    IMPI_Request
#define MPI_Group      IMPI_Group
#define MPI_Datatype   IMPI_Datatype
#define MPI_Op         IMPI_Op
#define MPI_Errhandler IMPI_Errhandler
#define MPI_Fint       IMPI_Fint
#define MPI_Aint       IMPI_Aint
#define MPI_Status_int IMPI_Status_int  
#define MPI_Status     IMPI_Status

/************************************************************************************
 * Next, we define the various constants used by MPI 1.3. Note that their values    *
 * may differ in value and type from the underlying MPI implementation              *
 ************************************************************************************/

/* Return codes (used in C and Fortran) */

#define MPI_SUCCESS        IMPI_SUCCESS
#define MPI_ERR_BUFFER     IMPI_ERR_BUFFER
#define MPI_ERR_COUNT      IMPI_ERR_COUNT
#define MPI_ERR_TYPE       IMPI_ERR_TYPE
#define MPI_ERR_TAG        IMPI_ERR_TAG
#define MPI_ERR_COMM       IMPI_ERR_COMM
#define MPI_ERR_RANK       IMPI_ERR_RANK
#define MPI_ERR_REQUEST    IMPI_ERR_REQUEST
#define MPI_ERR_ROOT       IMPI_ERR_ROOT
#define MPI_ERR_GROUP      IMPI_ERR_GROUP
#define MPI_ERR_OP         IMPI_ERR_OP
#define MPI_ERR_TOPOLOGY   IMPI_ERR_TOPOLOGY 
#define MPI_ERR_DIMS       IMPI_ERR_DIMS
#define MPI_ERR_ARG        IMPI_ERR_ARG
#define MPI_ERR_UNKNOWN    IMPI_ERR_UNKNOWN
#define MPI_ERR_TRUNCATE   IMPI_ERR_TRUNCATE
#define MPI_ERR_OTHER      IMPI_ERR_OTHER
#define MPI_ERR_INTERN     IMPI_ERR_INTERN
#define MPI_PENDING        IMPI_PENDING
#define MPI_ERR_IN_STATUS  IMPI_ERR_IN_STATUS
#define MPI_ERR_LASTCODE   IMPI_ERR_LASTCODE


/* Assorted constants (used in C and Fortran) */

#define MPI_BOTTOM         IMPI_BOTTOM
#define MPI_PROC_NULL      IMPI_PROC_NULL
#define MPI_ANY_SOURCE     IMPI_ANY_SOURCE
#define MPI_ANY_TAG        IMPI_ANY_TAG
#define MPI_UNDEFINED      IMPI_UNDEFINED
#define MPI_BSEND_OVERHEAD IMPI_BSEND_OVERHEAD
#define MPI_KEYVAL_INVALID IMPI_KEYVAL_INVALID


/* Status size and reserved index values (Fortran) */

#define MPI_STATUS_SIZE   IMPI_STATUS_SIZE
#define MPI_SOURCE        IMPI_SOURCE
#define MPI_TAG           IMPI_TAG
#define MPI_ERROR         IMPI_ERROR


/* Error-handling specifiers (C and Fortran) */

#define MPI_ERRORS_ARE_FATAL IMPI_ERRORS_ARE_FATAL
#define MPI_ERRORS_RETURN    IMPI_ERRORS_RETURN


/* Maximum sizes for strings */

#define MPI_MAX_ERROR_STRING   IMPI_MAX_ERROR_STRING
#define MPI_MAX_PROCESSOR_NAME IMPI_MAX_PROCESSOR_NAME


/* Elementary datatypes (C) */

#define MPI_DATATYPE_NULL  IMPI_DATATYPE_NULL
#define MPI_CHAR           IMPI_CHAR
#define MPI_SHORT          IMPI_SHORT
#define MPI_INT            IMPI_INT
#define MPI_LONG           IMPI_LONG
#define MPI_UNSIGNED_CHAR  IMPI_UNSIGNED_CHAR
#define MPI_UNSIGNED_SHORT IMPI_UNSIGNED_SHORT
#define MPI_UNSIGNED       IMPI_UNSIGNED
#define MPI_UNSIGNED_LONG  IMPI_UNSIGNED_LONG
#define MPI_FLOAT          IMPI_FLOAT
#define MPI_DOUBLE         IMPI_DOUBLE
#define MPI_LONG_DOUBLE    IMPI_LONG_DOUBLE
#define MPI_BYTE           IMPI_BYTE
#define MPI_PACKED         IMPI_PACKED


/* Elementary datatypes (Fortran) */

#define MPI_INTEGER          IMPI_INTEGER
#define MPI_REAL             IMPI_REAL
#define MPI_DOUBLE_PRECISION IMPI_DOUBLE_PRECISION
#define MPI_COMPLEX          IMPI_COMPLEX
#define MPI_LOGICAL          IMPI_LOGICAL
#define MPI_CHARACTER        IMPI_CHARACTER


/* Datatypes for reduction functions (C) */

#define MPI_FLOAT_INT        IMPI_FLOAT_INT
#define MPI_DOUBLE_INT       IMPI_DOUBLE_INT
#define MPI_LONG_INT         IMPI_LONG_INT
#define MPI_2INT             IMPI_2INT
#define MPI_SHORT_INT        IMPI_SHORT_INT
#define MPI_LONG_DOUBLE_INT  IMPI_LONG_DOUBLE_INT


/* Datatypes for reduction functions (Fortran) */

#define MPI_2REAL             IMPI_2REAL
#define MPI_2DOUBLE_PRECISION IMPI_2DOUBLE_PRECISION
#define MPI_2INTEGER          IMPI_2INTEGER


/* Optional datatypes (C) */

#define MPI_LONG_LONG_INT    IMPI_LONG_LONG_INT
#define MPI_LONG_LONG        IMPI_LONG_LONG


/* Optional datatypes (Fortran) */

#define MPI_DOUBLE_COMPLEX  IMPI_DOUBLE_COMPLEX

#define MPI_INTEGER1        IMPI_INTEGER1
#define MPI_INTEGER2        IMPI_INTEGER2
#define MPI_INTEGER4        IMPI_INTEGER4
#define MPI_INTEGER8        IMPI_INTEGER8

#define MPI_REAL2           IMPI_REAL2
#define MPI_REAL4           IMPI_REAL4
#define MPI_REAL8           IMPI_REAL8


/* Special datatypes for constructing derived datatypes */

#define MPI_UB IMPI_UB
#define MPI_LB IMPI_LB


/* Reserved communicators */

#define MPI_COMM_NULL  IMPI_COMM_NULL
#define MPI_COMM_WORLD IMPI_COMM_WORLD
#define MPI_COMM_SELF  IMPI_COMM_SELF


/* Results of communicator and group comparisons */

#define MPI_IDENT      IMPI_IDENT
#define MPI_CONGRUENT  IMPI_CONGRUENT
#define MPI_SIMILAR    IMPI_SIMILAR
#define MPI_UNEQUAL    IMPI_UNEQUAL


/* Environmental inquiry keys (C and Fortran) */

#define MPI_TAG_UB          IMPI_TAG_UB
#define MPI_IO              IMPI_IO
#define MPI_HOST            IMPI_HOST
#define MPI_WTIME_IS_GLOBAL IMPI_WTIME_IS_GLOBAL


/* Collective operations (C and Fortran) */

#define MPI_OP_NULL IMPI_OP_NULL
#define MPI_MAX     IMPI_MAX
#define MPI_MIN     IMPI_MIN
#define MPI_SUM     IMPI_SUM
#define MPI_PROD    IMPI_PROD
#define MPI_LAND    IMPI_LAND
#define MPI_BAND    IMPI_BAND
#define MPI_LOR     IMPI_LOR
#define MPI_BOR     IMPI_BOR
#define MPI_LXOR    IMPI_LXOR
#define MPI_BXOR    IMPI_BXOR
#define MPI_MAXLOC  IMPI_MAXLOC
#define MPI_MINLOC  IMPI_MINLOC


/* Reserved groups */

#define MPI_GROUP_NULL  IMPI_GROUP_NULL
#define MPI_GROUP_EMPTY IMPI_GROUP_EMPTY


/* Reserved requests */

#define MPI_REQUEST_NULL IMPI_REQUEST_NULL


/* Topologies (C and Fortran) -- Must not clash with IMPI_UNDEFINED */

#define MPI_GRAPH  IMPI_GRAPH
#define MPI_CART   IMPI_CART


/* Prototypes for user-defined functions */

// NOT SUPPORTED
// typedef int MPI_Copy_function(IMPI_Comm oldcomm, int keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
// typedef int MPI_Delete_function(IMPI_Comm comm, int keyval, void *attribute_val, void *extra_state);
// typedef void MPI_Handler_function(IMPI_Comm *, int *, ...);
// typedef void MPI_User_function(void *invec, void *inoutvec, int *len, IMPI_Datatype *datatype);


/* Predefined functions in C and Fortran */

#define MPI_DUP_FN         IMPI_DUP_FN
#define MPI_NULL_COPY_FN   IMPI_NULL_COPY_FN
#define MPI_NULL_DELETE_FN IMPI_NULL_DELETE_FN


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
extern int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status);
extern int MPI_Testany(int count, MPI_Request *array_of_requests, int *index, int *flag, MPI_Status *status);
extern int MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses);
extern int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag, MPI_Status *array_of_statuses);
extern int MPI_Waitsome(int incount, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses);
extern int MPI_Testsome(int incount, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses);
extern int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);
extern int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
extern int MPI_Cancel(MPI_Request *request);
extern int MPI_Test_cancelled(MPI_Status *status, int *flag);
extern int MPI_Start(MPI_Request *request);
extern int MPI_Startall(int count, MPI_Request *array_of_requests);


/* Type definition */

extern int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_indexed(int count, int *array_of_blocklengths, int *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
extern int MPI_Type_hindexed(int count, int *array_of_blocklengths, MPI_Aint *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
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
extern int MPI_Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
extern int MPI_Reduce_scatter(void* sendbuf, void* recvbuf, int *recvcounts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
extern int MPI_Scan(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

// NOT SUPPORTED due to typecasting issues!
// extern int MPI_Op_create(MPI_User_function *function, int commute, MPI_Op *op);
// extern int MPI_Op_free(MPI_Op *op);

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
extern int MPI_Comm_test_inter(MPI_Comm comm, int *flag);
extern int MPI_Comm_remote_size(MPI_Comm comm, int *size);
extern int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);
extern int MPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader, int tag, MPI_Comm *newintercomm);
extern int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintracomm);
extern int MPI_Attr_put(MPI_Comm comm, int keyval, void* attribute_val);
extern int MPI_Attr_get(MPI_Comm comm, int keyval, void* attribute_val, int *flag);
extern int MPI_Attr_delete(MPI_Comm comm, int keyval);

// NOT SUPPORTED
// extern int MPI_Keyval_create(MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void* extra_state);
// extern int MPI_Keyval_free(int *keyval);

/* Process topology */

extern int MPI_Cart_create(MPI_Comm comm_old, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart);
extern int MPI_Dims_create(int nnodes, int ndims, int *dims);
extern int MPI_Graph_create(MPI_Comm comm_old, int nnodes, int *index, int *edges, int reorder, MPI_Comm *comm_graph);
extern int MPI_Topo_test(MPI_Comm comm, int *status);
extern int MPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);
extern int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges);
extern int MPI_Cartdim_get(MPI_Comm comm, int *ndims);
extern int MPI_Cart_get(MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords);
extern int MPI_Cart_rank(MPI_Comm comm, int *coords, int *rank);
extern int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int *coords);
extern int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);
extern int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int *neighbors);
extern int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
extern int MPI_Cart_sub(MPI_Comm comm, int *remain_dims, MPI_Comm *newcomm);
extern int MPI_Cart_map(MPI_Comm comm, int ndims, int *dims, int *periods, int *newrank);
extern int MPI_Graph_map(MPI_Comm comm, int nnodes, int *index, int *edges, int *newrank);


/* Environmental quiry */

extern int MPI_Get_version(int *version, int *subversion);
extern int MPI_Get_processor_name(char *name, int *resultlen);


/* Error handling */

// NOT SUPPORTED 
// extern int MPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler);
// extern int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);
// extern int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);
// extern int MPI_Errhandler_free(MPI_Errhandler *errhandler);
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

// NOT SUPPORTED	
// extern int MPI_Pcontrol(const int level, ...);

#endif
