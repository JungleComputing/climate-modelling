#include "flags.h"

#ifdef ENABLE_INTERCEPT

#include "debugging.h"
#include "external/mpi.h"

int MPI_Init ( int *argc, char ***argv )
{
   init_debug();

#ifdef TRACE_CALLS
   INFO(0, "MPI_Init","(int *argc=%p, char ***argv=%p)", argc, argv);
#endif // TRACE_CALLS

   int error = IMPI_Init(argc, argv);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Init failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Abort ( MPI_Comm comm, int errorcode )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Abort","(MPI_Comm comm=%s, int errorcode=%d)", comm_to_string(comm), errorcode);
#endif // TRACE_CALLS

   int error = IMPI_Abort(comm, errorcode);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Abort failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}




int MPI_Address ( void *location, MPI_Aint *address )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Address","(void *location=%p, MPI_Aint *address=%p)", location, address);
#endif // TRACE_CALLS

   int error = IMPI_Address(location, address);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Address failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Allgather ( void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Allgather","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcount=%d, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)", sendbuf, sendcount, type_to_string(sendtype), recvbuf, recvcount, type_to_string(recvtype), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Allgather failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Allgatherv ( void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Allgatherv","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int *recvcounts=%p, int *displs=%p, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)", sendbuf, sendcount, type_to_string(sendtype), recvbuf, recvcounts, displs, type_to_string(recvtype), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Allgatherv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}




int MPI_Allreduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Allreduce","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
#endif // TRACE_CALLS

INFO(1, "JASON### MPI_Allreduce IN:", "%d %d\n", count, *((int*) sendbuf));

   int error = IMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);

INFO(1, "JASON### MPI_Allreduce OUT:", "%d %d %d\n", count, *((int*) recvbuf), error);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Allreduce failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Alltoall ( void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Alltoall","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcount=%d, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)", sendbuf, sendcount, type_to_string(sendtype), recvbuf, recvcount, type_to_string(recvtype), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Alltoall failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Alltoallv ( void *sendbuf, int *sendcnts, int *sdispls, MPI_Datatype sendtype, void *recvbuf, int *recvcnts, int *rdispls, MPI_Datatype recvtype, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Alltoallv","(void *sendbuf=%p, int *sendcnts=%p, int *sdispls=%p, MPI_Datatype sendtype=%s, void *recvbuf=%p, int *recvcnts=%p, int *rdispls=%p, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)", sendbuf, sendcnts, sdispls, type_to_string(sendtype), recvbuf, recvcnts, rdispls, type_to_string(recvtype), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Alltoallv(sendbuf, sendcnts, sdispls, sendtype, recvbuf, recvcnts, rdispls, recvtype, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Alltoallv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Attr_delete ( MPI_Comm comm, int keyval )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Attr_delete","(MPI_Comm comm=%s, int keyval=%d)", comm_to_string(comm), keyval);
#endif // TRACE_CALLS

   int error = IMPI_Attr_delete(comm, keyval);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Attr_delete failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Attr_get ( MPI_Comm comm, int keyval, void *attr_value, int *flag )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Attr_get","(MPI_Comm comm=%s, int keyval=%d, void *attr_value=%p, int *flag=%p)", comm_to_string(comm), keyval, attr_value, flag);
#endif // TRACE_CALLS

   int error = IMPI_Attr_get(comm, keyval, attr_value, flag);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Attr_get failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Attr_put ( MPI_Comm comm, int keyval, void *attr_value )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Attr_put","(MPI_Comm comm=%s, int keyval=%d, void *attr_value=%p)", comm_to_string(comm), keyval, attr_value);
#endif // TRACE_CALLS

   int error = IMPI_Attr_put(comm, keyval, attr_value);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Attr_put failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Barrier ( MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Barrier","(MPI_Comm comm=%s)", comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Barrier(comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Barrier failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Bcast ( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Bcast","(void *buffer=%p, int count=%d, MPI_Datatype datatype=%s, int root=%d, MPI_Comm comm=%s)", buffer, count, type_to_string(datatype), root, comm_to_string(comm));
#endif // TRACE_CALLS

if (count <= 0) { 
   ERROR(0, "Bcast count = 0!!");
}

   int error = IMPI_Bcast(buffer, count, datatype, root, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Bcast failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Bsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Bsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Bsend(buf, count, datatype, dest, tag, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Bsend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Bsend_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Bsend_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Bsend_init failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Buffer_attach ( void *buffer, int size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Buffer_attach","(void *buffer=%p, int size=%d)", buffer, size);
#endif // TRACE_CALLS

   int error = IMPI_Buffer_attach(buffer, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Buffer_attach failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Buffer_detach ( void *buffer, int *size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Buffer_detach","(void *buffer=%p, int *size=%p)", buffer, size);
#endif // TRACE_CALLS

   int error = IMPI_Buffer_detach(buffer, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Buffer_detach failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cancel ( MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cancel","(MPI_Request *request=%p)", request);
#endif // TRACE_CALLS

   int error = IMPI_Cancel(request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cancel failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_coords ( MPI_Comm comm, int rank, int maxdims, int *coords )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_coords","(MPI_Comm comm=%s, int rank=%d, int maxdims=%d, int *coords=%p)", comm_to_string(comm), rank, maxdims, coords);
#endif // TRACE_CALLS

   int error = IMPI_Cart_coords(comm, rank, maxdims, coords);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_coords failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_create ( MPI_Comm comm_old, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_create","(MPI_Comm comm_old=%s, int ndims=%d, int *dims=%p, int *periods=%p, int reorder=%d, MPI_Comm *comm_cart=%p)", comm_to_string(comm_old), ndims, dims, periods, reorder, comm_cart);
#endif // TRACE_CALLS

   int error = IMPI_Cart_create(comm_old, ndims, dims, periods, reorder, comm_cart);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cartdim_get ( MPI_Comm comm, int *ndims )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cartdim_get","(MPI_Comm comm=%s, int *ndims=%p)", comm_to_string(comm), ndims);
#endif // TRACE_CALLS

   int error = IMPI_Cartdim_get(comm, ndims);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cartdim_get failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_get ( MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_get","(MPI_Comm comm=%s, int maxdims=%d, int *dims=%p, int *periods=%p, int *coords=%p)", comm_to_string(comm), maxdims, dims, periods, coords);
#endif // TRACE_CALLS

   int error = IMPI_Cart_get(comm, maxdims, dims, periods, coords);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_get failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_map ( MPI_Comm comm_old, int ndims, int *dims, int *periods, int *newrank )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_map","(MPI_Comm comm_old=%s, int ndims=%d, int *dims=%p, int *periods=%p, int *newrank=%p)", comm_to_string(comm_old), ndims, dims, periods, newrank);
#endif // TRACE_CALLS

   int error = IMPI_Cart_map(comm_old, ndims, dims, periods, newrank);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_map failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_rank ( MPI_Comm comm, int *coords, int *rank )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_rank","(MPI_Comm comm=%s, int *coords=%p, int *rank=%p)", comm_to_string(comm), coords, rank);
#endif // TRACE_CALLS

   int error = IMPI_Cart_rank(comm, coords, rank);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_rank failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_shift ( MPI_Comm comm, int direction, int displ, int *source, int *dest )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_shift","(MPI_Comm comm=%s, int direction=%d, int displ=%d, int *source=%p, int *dest=%p)", comm_to_string(comm), direction, displ, source, dest);
#endif // TRACE_CALLS

   int error = IMPI_Cart_shift(comm, direction, displ, source, dest);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_shift failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Cart_sub ( MPI_Comm comm, int *remain_dims, MPI_Comm *comm_new )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Cart_sub","(MPI_Comm comm=%s, int *remain_dims=%p, MPI_Comm *comm_new=%p)", comm_to_string(comm), remain_dims, comm_new);
#endif // TRACE_CALLS

   int error = IMPI_Cart_sub(comm, remain_dims, comm_new);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Cart_sub failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Comm_compare ( MPI_Comm comm1, MPI_Comm comm2, int *result )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_compare","(MPI_Comm comm1=%s, MPI_Comm comm2=%s, int *result=%p)", comm_to_string(comm1), comm_to_string(comm2), result);
#endif // TRACE_CALLS

   int error = IMPI_Comm_compare(comm1, comm2, result);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_compare failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Comm_create ( MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_create","(MPI_Comm comm=%s, MPI_Group group=%s, MPI_Comm *newcomm=%p)", comm_to_string(comm), group_to_string(group), newcomm);
#endif // TRACE_CALLS

   int error = IMPI_Comm_create(comm, group, newcomm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Comm_dup ( MPI_Comm comm, MPI_Comm *newcomm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_dup","(MPI_Comm comm=%s, MPI_Comm *newcomm=%p)", comm_to_string(comm), newcomm);
#endif // TRACE_CALLS

   int error = IMPI_Comm_dup(comm, newcomm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_dup failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_free ( MPI_Comm *comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_free","(MPI_Comm *comm=%p)", comm);
#endif // TRACE_CALLS

   int error = IMPI_Comm_free(comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_group ( MPI_Comm comm, MPI_Group *group )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_group","(MPI_Comm comm=%s, MPI_Group *group=%p)", comm_to_string(comm), group);
#endif // TRACE_CALLS

   int error = IMPI_Comm_group(comm, group);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_group failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Comm_rank ( MPI_Comm comm, int *rank )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_rank","(MPI_Comm comm=%s, int *rank=%p)", comm_to_string(comm), rank);
#endif // TRACE_CALLS

   int error = IMPI_Comm_rank(comm, rank);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_rank failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_remote_group ( MPI_Comm comm, MPI_Group *group )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_remote_group","(MPI_Comm comm=%s, MPI_Group *group=%p)", comm_to_string(comm), group);
#endif // TRACE_CALLS

   int error = IMPI_Comm_remote_group(comm, group);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_remote_group failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_remote_size ( MPI_Comm comm, int *size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_remote_size","(MPI_Comm comm=%s, int *size=%p)", comm_to_string(comm), size);
#endif // TRACE_CALLS

   int error = IMPI_Comm_remote_size(comm, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_remote_size failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_size ( MPI_Comm comm, int *size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_size","(MPI_Comm comm=%s, int *size=%p)", comm_to_string(comm), size);
#endif // TRACE_CALLS

   int error = IMPI_Comm_size(comm, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_size failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_split ( MPI_Comm comm, int color, int key, MPI_Comm *newcomm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_split","(MPI_Comm comm=%s, int color=%d, int key=%d, MPI_Comm *newcomm=%p)", comm_to_string(comm), color, key, newcomm);
#endif // TRACE_CALLS

   int error = IMPI_Comm_split(comm, color, key, newcomm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_split failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Comm_test_inter ( MPI_Comm comm, int *flag )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Comm_test_inter","(MPI_Comm comm=%s, int *flag=%p)", comm_to_string(comm), flag);
#endif // TRACE_CALLS

   int error = IMPI_Comm_test_inter(comm, flag);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Comm_test_inter failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}

/* 

NOT SUPPORTED

int MPI_Errhandler_create ( MPI_Handler_function *function, MPI_Errhandler *errhandler )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Errhandler_create","(MPI_Handler_function *function=%p, MPI_Errhandler *errhandler=%p)", function, errhandler);
#endif // TRACE_CALLS

   int error = IMPI_Errhandler_create(function, errhandler);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Errhandler_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Errhandler_free ( MPI_Errhandler *errhandler )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Errhandler_free","(MPI_Errhandler *errhandler=%p)", errhandler);
#endif // TRACE_CALLS

   int error = IMPI_Errhandler_free(errhandler);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Errhandler_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Errhandler_get ( MPI_Comm comm, MPI_Errhandler *errhandler )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Errhandler_get","(MPI_Comm comm=%s, MPI_Errhandler *errhandler=%p)", comm_to_string(comm), errhandler);
#endif // TRACE_CALLS

   int error = IMPI_Errhandler_get(comm, errhandler);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Errhandler_get failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Errhandler_set ( MPI_Comm comm, MPI_Errhandler errhandler )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Errhandler_set","(MPI_Comm comm=%s, MPI_Errhandler errhandler=%d)", comm_to_string(comm), errhandler);
#endif // TRACE_CALLS

   int error = IMPI_Errhandler_set(comm, errhandler);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Errhandler_set failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}

*/

int MPI_Error_class ( int errorcode, int *errorclass )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Error_class","(int errorcode=%d, int *errorclass=%p)", errorcode, errorclass);
#endif // TRACE_CALLS

   int error = IMPI_Error_class(errorcode, errorclass);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Error_class failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Error_string ( int errorcode, char *string, int *resultlen )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Error_string","(int errorcode=%d, char *string=%p, int *resultlen=%p)", errorcode, string, resultlen);
#endif // TRACE_CALLS

   int error = IMPI_Error_string(errorcode, string, resultlen);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Error_string failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Finalize (  )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Finalize","()");
#endif // TRACE_CALLS

   int error = IMPI_Finalize();

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Finalize failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Gather ( void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Gather","(void *sendbuf=%p, int sendcnt=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcnt=%d, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)", sendbuf, sendcnt, type_to_string(sendtype), recvbuf, recvcnt, type_to_string(recvtype), root, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Gather(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Gather failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Gatherv ( void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int *recvcnts, int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Gatherv","(void *sendbuf=%p, int sendcnt=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int *recvcnts=%p, int *displs=%p, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)", sendbuf, sendcnt, type_to_string(sendtype), recvbuf, recvcnts, displs, type_to_string(recvtype), root, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Gatherv(sendbuf, sendcnt, sendtype, recvbuf, recvcnts, displs, recvtype, root, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Gatherv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Get_count ( MPI_Status *status, MPI_Datatype datatype, int *count )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Get_count","(MPI_Status *status=%p, MPI_Datatype datatype=%s, int *count=%p)", status, type_to_string(datatype), count);
#endif // TRACE_CALLS

   int error = IMPI_Get_count(status, datatype, count);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Get_count failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Get_elements ( MPI_Status *status, MPI_Datatype datatype, int *elements )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Get_elements","(MPI_Status *status=%p, MPI_Datatype datatype=%s, int *elements=%p)", status, type_to_string(datatype), elements);
#endif // TRACE_CALLS

   int error = IMPI_Get_elements(status, datatype, elements);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Get_elements failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Get_processor_name ( char *name, int *resultlen )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Get_processor_name","(char *name=%p, int *resultlen=%p)", name, resultlen);
#endif // TRACE_CALLS

   int error = IMPI_Get_processor_name(name, resultlen);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Get_processor_name failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Get_version ( int *version, int *subversion )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Get_version","(int *version=%p, int *subversion=%p)", version, subversion);
#endif // TRACE_CALLS

   int error = IMPI_Get_version(version, subversion);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Get_version failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Graph_create ( MPI_Comm comm_old, int nnodes, int *indx, int *edges, int reorder, MPI_Comm *comm_graph )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Graph_create","(MPI_Comm comm_old=%s, int nnodes=%d, int *indx=%p, int *edges=%p, int reorder=%d, MPI_Comm *comm_graph=%p)", comm_to_string(comm_old), nnodes, indx, edges, reorder, comm_graph);
#endif // TRACE_CALLS

   int error = IMPI_Graph_create(comm_old, nnodes, indx, edges, reorder, comm_graph);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Graph_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Graphdims_get ( MPI_Comm comm, int *nnodes, int *nedges )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Graphdims_get","(MPI_Comm comm=%s, int *nnodes=%p, int *nedges=%p)", comm_to_string(comm), nnodes, nedges);
#endif // TRACE_CALLS

   int error = IMPI_Graphdims_get(comm, nnodes, nedges);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Graphdims_get failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Graph_get ( MPI_Comm comm, int maxindex, int maxedges, int *indx, int *edges )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Graph_get","(MPI_Comm comm=%s, int maxindex=%d, int maxedges=%d, int *indx=%p, int *edges=%p)", comm_to_string(comm), maxindex, maxedges, indx, edges);
#endif // TRACE_CALLS

   int error = IMPI_Graph_get(comm, maxindex, maxedges, indx, edges);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Graph_get failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Graph_map ( MPI_Comm comm_old, int nnodes, int *indx, int *edges, int *newrank )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Graph_map","(MPI_Comm comm_old=%s, int nnodes=%d, int *indx=%p, int *edges=%p, int *newrank=%p)", comm_to_string(comm_old), nnodes, indx, edges, newrank);
#endif // TRACE_CALLS

   int error = IMPI_Graph_map(comm_old, nnodes, indx, edges, newrank);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Graph_map failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Graph_neighbors_count ( MPI_Comm comm, int rank, int *nneighbors )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Graph_neighbors_count","(MPI_Comm comm=%s, int rank=%d, int *nneighbors=%p)", comm_to_string(comm), rank, nneighbors);
#endif // TRACE_CALLS

   int error = IMPI_Graph_neighbors_count(comm, rank, nneighbors);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Graph_neighbors_count failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Graph_neighbors ( MPI_Comm comm, int rank, int maxneighbors, int *neighbors )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Graph_neighbors","(MPI_Comm comm=%s, int rank=%d, int maxneighbors=%d, int *neighbors=%p)", comm_to_string(comm), rank, maxneighbors, neighbors);
#endif // TRACE_CALLS

   int error = IMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Graph_neighbors failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Group_compare ( MPI_Group group1, MPI_Group group2, int *result )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_compare","(MPI_Group group1=%s, MPI_Group group2=%s, int *result=%p)", group_to_string(group1), group_to_string(group2), result);
#endif // TRACE_CALLS

   int error = IMPI_Group_compare(group1, group2, result);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_compare failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_difference ( MPI_Group group1, MPI_Group group2, MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_difference","(MPI_Group group1=%s, MPI_Group group2=%s, MPI_Group *newgroup=%p)", group_to_string(group1), group_to_string(group2), newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_difference(group1, group2, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_difference failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_excl ( MPI_Group group, int n, int *ranks, MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_excl","(MPI_Group group=%s, int n=%d, int *ranks=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranks, newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_excl(group, n, ranks, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_excl failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_free ( MPI_Group *group )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_free","(MPI_Group *group=%p)", group);
#endif // TRACE_CALLS

   int error = IMPI_Group_free(group);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_incl ( MPI_Group group, int n, int *ranks, MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_incl","(MPI_Group group=%s, int n=%d, int *ranks=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranks, newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_incl(group, n, ranks, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_incl failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_intersection ( MPI_Group group1, MPI_Group group2, MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_intersection","(MPI_Group group1=%s, MPI_Group group2=%s, MPI_Group *newgroup=%p)", group_to_string(group1), group_to_string(group2), newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_intersection(group1, group2, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_intersection failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_range_excl ( MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_range_excl","(MPI_Group group=%s, int n=%d, int ranges[][3]=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranges, newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_range_excl(group, n, ranges, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_range_excl failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_range_incl ( MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_range_incl","(MPI_Group group=%s, int n=%d, int ranges[][3]=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranges, newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_range_incl(group, n, ranges, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_range_incl failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_rank ( MPI_Group group, int *rank )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_rank","(MPI_Group group=%s, int *rank=%p)", group_to_string(group), rank);
#endif // TRACE_CALLS

   int error = IMPI_Group_rank(group, rank);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_rank failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_size ( MPI_Group group, int *size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_size","(MPI_Group group=%s, int *size=%p)", group_to_string(group), size);
#endif // TRACE_CALLS

   int error = IMPI_Group_size(group, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_size failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_translate_ranks ( MPI_Group group1, int n, int *ranks1, MPI_Group group2, int *ranks2 )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_translate_ranks","(MPI_Group group1=%s, int n=%d, int *ranks1=%p, MPI_Group group2=%s, int *ranks2=%p)", group_to_string(group1), n, ranks1, group_to_string(group2), ranks2);
#endif // TRACE_CALLS

   int error = IMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_translate_ranks failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Group_union ( MPI_Group group1, MPI_Group group2, MPI_Group *newgroup )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Group_union","(MPI_Group group1=%s, MPI_Group group2=%s, MPI_Group *newgroup=%p)", group_to_string(group1), group_to_string(group2), newgroup);
#endif // TRACE_CALLS

   int error = IMPI_Group_union(group1, group2, newgroup);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Group_union failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Ibsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Ibsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Ibsend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Initialized ( int *flag )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Initialized","(int *flag=%p)", flag);
#endif // TRACE_CALLS

   int error = IMPI_Initialized(flag);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Initialized failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Intercomm_create ( MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader, int tag, MPI_Comm *newintercomm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Intercomm_create","(MPI_Comm local_comm=%s, int local_leader=%d, MPI_Comm peer_comm=%s, int remote_leader=%d, int tag=%d, MPI_Comm *newintercomm=%p)", comm_to_string(local_comm), local_leader, comm_to_string(peer_comm), remote_leader, tag, newintercomm);
#endif // TRACE_CALLS

   int error = IMPI_Intercomm_create(local_comm, local_leader, peer_comm, remote_leader, tag, newintercomm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Intercomm_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Intercomm_merge ( MPI_Comm intercomm, int high, MPI_Comm *newintracomm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Intercomm_merge","(MPI_Comm intercomm=%s, int high=%d, MPI_Comm *newintracomm=%p)", comm_to_string(intercomm), high, newintracomm);
#endif // TRACE_CALLS

   int error = IMPI_Intercomm_merge(intercomm, high, newintracomm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Intercomm_merge failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Iprobe ( int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Iprobe","(int source=%d, int tag=%d, MPI_Comm comm=%s, int *flag=%p, MPI_Status *status=%p)", source, tag, comm_to_string(comm), flag, status);
#endif // TRACE_CALLS

   int error = IMPI_Iprobe(source, tag, comm, flag, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Iprobe failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Irecv ( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Irecv","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), source, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Irecv(buf, count, datatype, source, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Irecv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Irsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Irsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Irsend(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Irsend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Isend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Isend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Isend(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Isend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Issend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Issend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Issend(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Issend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


/*

NOT SUPPORTED

int MPI_Keyval_create ( MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Keyval_create","(MPI_Copy_function *copy_fn=%p, MPI_Delete_function *delete_fn=%p, int *keyval=%p, void *extra_state=%p)", copy_fn, delete_fn, keyval, extra_state);
#endif // TRACE_CALLS

   int error = IMPI_Keyval_create(copy_fn, delete_fn, keyval, extra_state);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Keyval_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Keyval_free ( int *keyval )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Keyval_free","(int *keyval=%p)", keyval);
#endif // TRACE_CALLS

   int error = IMPI_Keyval_free(keyval);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Keyval_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}

*/
/*

NOT SUPPORTED

int MPI_Op_create ( MPI_User_function *function, int commute, MPI_Op *op )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Op_create","(MPI_User_function *function=%p, int commute=%d, MPI_Op *op=%p)", function, commute, op);
#endif // TRACE_CALLS

   int error = IMPI_Op_create(function, commute, op);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Op_create failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Op_free ( MPI_Op *op )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Op_free","(MPI_Op *op=%p)", op);
#endif // TRACE_CALLS

   int error = IMPI_Op_free(op);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Op_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}

*/

int MPI_Pack ( void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outcount, int *position, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Pack","(void *inbuf=%p, int incount=%d, MPI_Datatype datatype=%s, void *outbuf=%p, int outcount=%d, int *position=%p, MPI_Comm comm=%s)", inbuf, incount, type_to_string(datatype), outbuf, outcount, position, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Pack(inbuf, incount, datatype, outbuf, outcount, position, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Pack failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Pack_size ( int incount, MPI_Datatype datatype, MPI_Comm comm, int *size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Pack_size","(int incount=%d, MPI_Datatype datatype=%s, MPI_Comm comm=%s, int *size=%p)", incount, type_to_string(datatype), comm_to_string(comm), size);
#endif // TRACE_CALLS

   int error = IMPI_Pack_size(incount, datatype, comm, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Pack_size failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}

/*

NOT SUPPORTED

int MPI_Pcontrol ( const int level, ... )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Pcontrol","(const int=%p, level ...=%p)", (void *) int, (void *) ...);
#endif // TRACE_CALLS

   int error = IMPI_Pcontrol(int, ...);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Pcontrol failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}
*/

int MPI_Probe ( int source, int tag, MPI_Comm comm, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Probe","(int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", source, tag, comm_to_string(comm), status);
#endif // TRACE_CALLS

   int error = IMPI_Probe(source, tag, comm, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Probe failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}




int MPI_Recv ( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Recv","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", buf, count, type_to_string(datatype), source, tag, comm_to_string(comm), status);
#endif // TRACE_CALLS

   int error = IMPI_Recv(buf, count, datatype, source, tag, comm, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Recv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Recv_init ( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Recv_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), source, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Recv_init(buf, count, datatype, source, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Recv_init failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Reduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Reduce","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, int root=%d, MPI_Comm comm=%s)", sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), root, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Reduce failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Reduce_scatter ( void *sendbuf, void *recvbuf, int *recvcnts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Reduce_scatter","(void *sendbuf=%p, void *recvbuf=%p, int *recvcnts=%p, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, recvcnts, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Reduce_scatter(sendbuf, recvbuf, recvcnts, datatype, op, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Reduce_scatter failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Request_free ( MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Request_free","(MPI_Request *request=%p)", request);
#endif // TRACE_CALLS

   int error = IMPI_Request_free(request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Request_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}




int MPI_Rsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Rsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Rsend(buf, count, datatype, dest, tag, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Rsend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Rsend_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Rsend_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Rsend_init failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Scan ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Scan","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Scan failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Scatter ( void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Scatter","(void *sendbuf=%p, int sendcnt=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcnt=%d, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)", sendbuf, sendcnt, type_to_string(sendtype), recvbuf, recvcnt, type_to_string(recvtype), root, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Scatter(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Scatter failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Scatterv ( void *sendbuf, int *sendcnts, int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Scatterv","(void *sendbuf=%p, int *sendcnts=%p, int *displs=%p, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcnt=%d, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)", sendbuf, sendcnts, displs, type_to_string(sendtype), recvbuf, recvcnt, type_to_string(recvtype), root, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Scatterv(sendbuf, sendcnts, displs, sendtype, recvbuf, recvcnt, recvtype, root, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Scatterv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Send ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Send","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Send(buf, count, datatype, dest, tag, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Send failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Send_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Send_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Send_init(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Send_init failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Sendrecv ( void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Sendrecv","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, int dest=%d, int sendtag=%d, void *recvbuf=%p, int recvcount=%d, MPI_Datatype recvtype=%s, int source=%d, int recvtag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", sendbuf, sendcount, type_to_string(sendtype), dest, sendtag, recvbuf, recvcount, type_to_string(recvtype), source, recvtag, comm_to_string(comm), status);
#endif // TRACE_CALLS

   int error = IMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Sendrecv failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Sendrecv_replace ( void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Sendrecv_replace","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int sendtag=%d, int source=%d, int recvtag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", buf, count, type_to_string(datatype), dest, sendtag, source, recvtag, comm_to_string(comm), status);
#endif // TRACE_CALLS

   int error = IMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Sendrecv_replace failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Ssend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Ssend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Ssend(buf, count, datatype, dest, tag, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Ssend failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Ssend_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Ssend_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
#endif // TRACE_CALLS

   int error = IMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Ssend_init failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Startall ( int count, MPI_Request array_of_requests[] )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Startall","(int count=%d, MPI_Request array_of_requests[]=%p)", count, array_of_requests);
#endif // TRACE_CALLS

   int error = IMPI_Startall(count, array_of_requests);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Startall failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Start ( MPI_Request *request )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Start","(MPI_Request *request=%p)", request);
#endif // TRACE_CALLS

   int error = IMPI_Start(request);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Start failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Testall ( int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[] )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Testall","(int count=%d, MPI_Request array_of_requests[]=%p, int *flag=%p, MPI_Status array_of_statuses[]=%p)", count, array_of_requests, flag, array_of_statuses);
#endif // TRACE_CALLS

   int error = IMPI_Testall(count, array_of_requests, flag, array_of_statuses);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Testall failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Testany ( int count, MPI_Request array_of_requests[], int *index, int *flag, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Testany","(int count=%d, MPI_Request array_of_requests[]=%p, int *index=%p, int *flag=%p, MPI_Status *status=%p)", count, array_of_requests, index, flag, status);
#endif // TRACE_CALLS

   int error = IMPI_Testany(count, array_of_requests, index, flag, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Testany failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Test_cancelled ( MPI_Status *status, int *flag )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Test_cancelled","(MPI_Status *status=%p, int *flag=%p)", status, flag);
#endif // TRACE_CALLS

   int error = IMPI_Test_cancelled(status, flag);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Test_cancelled failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Test ( MPI_Request *request, int *flag, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Test","(MPI_Request *request=%p, int *flag=%p, MPI_Status *status=%p)", request, flag, status);
#endif // TRACE_CALLS

   int error = IMPI_Test(request, flag, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Test failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Testsome ( int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[] )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Testsome","(int incount=%d, MPI_Request array_of_requests[]=%p, int *outcount=%p, int array_of_indices[]=%p, MPI_Status array_of_statuses[]=%p)", incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
#endif // TRACE_CALLS

   int error = IMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Testsome failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Topo_test ( MPI_Comm comm, int *topo_type )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Topo_test","(MPI_Comm comm=%s, int *topo_type=%p)", comm_to_string(comm), topo_type);
#endif // TRACE_CALLS

   int error = IMPI_Topo_test(comm, topo_type);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Topo_test failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_commit ( MPI_Datatype *datatype )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_commit","(MPI_Datatype *datatype=%p)", datatype);
#endif // TRACE_CALLS

   int error = IMPI_Type_commit(datatype);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_commit failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_contiguous ( int count, MPI_Datatype old_type, MPI_Datatype *new_type_p )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_contiguous","(int count=%d, MPI_Datatype old_type=%s, MPI_Datatype *new_type_p=%p)", count, type_to_string(old_type), new_type_p);
#endif // TRACE_CALLS

   int error = IMPI_Type_contiguous(count, old_type, new_type_p);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_contiguous failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Type_extent ( MPI_Datatype datatype, MPI_Aint *extent )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_extent","(MPI_Datatype datatype=%s, MPI_Aint *extent=%p)", type_to_string(datatype), extent);
#endif // TRACE_CALLS

   int error = IMPI_Type_extent(datatype, extent);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_extent failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_free ( MPI_Datatype *datatype )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_free","(MPI_Datatype *datatype=%p)", datatype);
#endif // TRACE_CALLS

   int error = IMPI_Type_free(datatype);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_free failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Type_hindexed ( int count, int blocklens[], MPI_Aint indices[], MPI_Datatype old_type, MPI_Datatype *newtype )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_hindexed","(int count=%d, int blocklens[]=%p, MPI_Aint indices[]=%p, MPI_Datatype old_type=%s, MPI_Datatype *newtype=%p)", count, blocklens, indices, type_to_string(old_type), newtype);
#endif // TRACE_CALLS

   int error = IMPI_Type_hindexed(count, blocklens, indices, old_type, newtype);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_hindexed failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_hvector ( int count, int blocklen, MPI_Aint stride, MPI_Datatype old_type, MPI_Datatype *newtype_p )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_hvector","(int count=%d, int blocklen=%d, MPI_Aint stride=%p, MPI_Datatype old_type=%s, MPI_Datatype *newtype_p=%p)", count, blocklen, (void *) stride, type_to_string(old_type), newtype_p);
#endif // TRACE_CALLS

   int error = IMPI_Type_hvector(count, blocklen, stride, old_type, newtype_p);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_hvector failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_indexed ( int count, int blocklens[], int indices[], MPI_Datatype old_type, MPI_Datatype *newtype )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_indexed","(int count=%d, int blocklens[]=%p, int indices[]=%p, MPI_Datatype old_type=%s, MPI_Datatype *newtype=%p)", count, blocklens, indices, type_to_string(old_type), newtype);
#endif // TRACE_CALLS

   int error = IMPI_Type_indexed(count, blocklens, indices, old_type, newtype);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_indexed failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_lb ( MPI_Datatype datatype, MPI_Aint *displacement )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_lb","(MPI_Datatype datatype=%s, MPI_Aint *displacement=%p)", type_to_string(datatype), displacement);
#endif // TRACE_CALLS

   int error = IMPI_Type_lb(datatype, displacement);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_lb failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Type_size ( MPI_Datatype datatype, int *size )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_size","(MPI_Datatype datatype=%s, int *size=%p)", type_to_string(datatype), size);
#endif // TRACE_CALLS

   int error = IMPI_Type_size(datatype, size);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_size failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_struct ( int count, int blocklens[], MPI_Aint indices[], MPI_Datatype old_types[], MPI_Datatype *newtype )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_struct","(int count=%d, int blocklens[]=%p, MPI_Aint indices[]=%p, MPI_Datatype old_types[]=%p, MPI_Datatype *newtype=%p)", count, blocklens, indices, old_types, newtype);
#endif // TRACE_CALLS

   int error = IMPI_Type_struct(count, blocklens, indices, old_types, newtype);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_struct failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_ub ( MPI_Datatype datatype, MPI_Aint *displacement )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_ub","(MPI_Datatype datatype=%s, MPI_Aint *displacement=%p)", type_to_string(datatype), displacement);
#endif // TRACE_CALLS

   int error = IMPI_Type_ub(datatype, displacement);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_ub failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Type_vector ( int count, int blocklength, int stride, MPI_Datatype old_type, MPI_Datatype *newtype_p )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Type_vector","(int count=%d, int blocklength=%d, int stride=%d, MPI_Datatype old_type=%s, MPI_Datatype *newtype_p=%p)", count, blocklength, stride, type_to_string(old_type), newtype_p);
#endif // TRACE_CALLS

   int error = IMPI_Type_vector(count, blocklength, stride, old_type, newtype_p);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Type_vector failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Unpack ( void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Unpack","(void *inbuf=%p, int insize=%d, int *position=%p, void *outbuf=%p, int outcount=%d, MPI_Datatype datatype=%s, MPI_Comm comm=%s)", inbuf, insize, position, outbuf, outcount, type_to_string(datatype), comm_to_string(comm));
#endif // TRACE_CALLS

   int error = IMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Unpack failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}



int MPI_Waitall ( int count, MPI_Request array_of_requests[], MPI_Status array_of_statuses[] )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Waitall","(int count=%d, MPI_Request array_of_requests[]=%p, MPI_Status array_of_statuses[]=%p)", count, array_of_requests, array_of_statuses);
#endif // TRACE_CALLS

   int error = IMPI_Waitall(count, array_of_requests, array_of_statuses);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Waitall failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Waitany ( int count, MPI_Request array_of_requests[], int *index, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Waitany","(int count=%d, MPI_Request array_of_requests[]=%p, int *index=%p, MPI_Status *status=%p)", count, array_of_requests, index, status);
#endif // TRACE_CALLS

   int error = IMPI_Waitany(count, array_of_requests, index, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Waitany failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Wait ( MPI_Request *request, MPI_Status *status )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Wait","(MPI_Request *request=%p, MPI_Status *status=%p)", request, status);
#endif // TRACE_CALLS

   int error = IMPI_Wait(request, status);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Wait failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}


int MPI_Waitsome ( int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[] )
{
#ifdef TRACE_CALLS
   INFO(0, "MPI_Waitsome","(int incount=%d, MPI_Request array_of_requests[]=%p, int *outcount=%p, int array_of_indices[]=%p, MPI_Status array_of_statuses[]=%p)", incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
#endif // TRACE_CALLS

   int error = IMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses);

#ifdef TRACE_ERRORS
   if (error != MPI_SUCCESS) {
      ERROR(0, "MPI_Waitsome failed (%d)!", error);
   }
#endif // TRACE_ERRORS
   return error;
}

/* The following calls are handled differently */

double MPI_Wtick ( )
{
   return IMPI_Wtick();
}

double MPI_Wtime( )
{
   return IMPI_Wtime();
}


MPI_Comm MPI_Comm_f2c(MPI_Fint comm)
{
   return IMPI_Comm_f2c(comm);
}

MPI_Fint MPI_Comm_c2f(MPI_Comm comm)
{
   return IMPI_Comm_c2f(comm);
}


MPI_Group MPI_Group_f2c(MPI_Fint g)
{
   return IMPI_Group_f2c(g);
}

MPI_Fint MPI_Group_c2f(MPI_Group g)
{
   return IMPI_Group_c2f(g);
}


MPI_Request MPI_Request_f2c(MPI_Fint request)
{
   return IMPI_Request_f2c(request);
}

MPI_Fint MPI_Request_c2f(MPI_Request request)
{
   return IMPI_Request_c2f(request);
}


MPI_Op MPI_Op_f2c(MPI_Fint op)
{
   return IMPI_Op_f2c(op);
}

MPI_Fint MPI_Op_c2f(MPI_Op op)
{
   return IMPI_Op_c2f(op);
}


/*
MPI_Status MPI_Status_f2c(MPI_Fint Status)
{
   return IMPI_Status_f2c(Status);
}

MPI_Fint MPI_Status_c2f(MPI_Status Status)
{
   return IMPI_Status_c2f(Status);
}
*/


MPI_Errhandler MPI_Errhandler_f2c(MPI_Fint Errhandler)
{
   return IMPI_Errhandler_f2c(Errhandler);
}

MPI_Fint MPI_Errhandler_c2f(MPI_Errhandler Errhandler)
{
   return IMPI_Errhandler_c2f(Errhandler);
}


MPI_Datatype MPI_Type_f2c(MPI_Fint Type)
{
   return IMPI_Type_f2c(Type);
}

MPI_Fint MPI_Type_c2f(MPI_Datatype Type)
{
   return IMPI_Type_c2f(Type);
}

#endif
