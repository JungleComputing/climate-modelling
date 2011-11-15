int MPI_Abort ( MPI_Comm comm, int errorcode )
{
   INFO(0, "MPI_Abort","(MPI_Comm comm=%s, int errorcode=%d)", comm_to_string(comm), errorcode);
   return PMPI_Abort(comm, errorcode);
}

int MPI_Accumulate ( void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win )
{
   INFO(0, "MPI_Accumulate","(void *origin_addr=%p, int origin_count=%d, MPI_Datatype origin_datatype=%s, int target_rank=%d, MPI_Aint target_disp=%p, int target_count=%d, MPI_Datatype target_datatype=%s, MPI_Op op=%s, MPI_Win win=%s)", origin_addr, origin_count, type_to_string(origin_datatype), target_rank, (void *) target_disp, target_count, type_to_string(target_datatype), op_to_string(op), win_to_string(win));
   return PMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win);
}


int MPI_Add_error_class ( int *errorclass )
{
   INFO(0, "MPI_Add_error_class","(int *errorclass=%p)", errorclass);
   return PMPI_Add_error_class(errorclass);
}


int MPI_Add_error_code ( int errorclass, int *errorcode )
{
   INFO(0, "MPI_Add_error_code","(int errorclass=%d, int *errorcode=%p)", errorclass, errorcode);
   return PMPI_Add_error_code(errorclass, errorcode);
}


int MPI_Add_error_string ( int errorcode, char *string )
{
   INFO(0, "MPI_Add_error_string","(int errorcode=%d, char *string=%p)", errorcode, string);
   return PMPI_Add_error_string(errorcode, string);
}


int MPI_Address ( void *location, MPI_Aint *address )
{
   INFO(0, "MPI_Address","(void *location=%p, MPI_Aint *address=%p)", location, address);
   return PMPI_Address(location, address);
}

int MPI_Allgather ( void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm )
{
   INFO(0, "MPI_Allgather","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcount=%d, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)",
             sendbuf, sendcount, type_to_string(sendtype), recvbuf, recvcount, type_to_string(recvtype), comm_to_string(comm));
   return PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
}

int MPI_Allgatherv ( void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm )
{
   INFO(0, "MPI_Allgatherv","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int *recvcounts=%p, int *displs=%p, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)", 
             sendbuf, sendcount, type_to_string(sendtype), recvbuf, recvcounts, displs, type_to_string(recvtype), comm_to_string(comm));
   return PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
}

int MPI_Alloc_mem ( MPI_Aint size, MPI_Info info, void *baseptr )
{
   INFO(0, "MPI_Alloc_mem","(MPI_Aint size=%p, MPI_Info info=%s, void *baseptr=%p)", (void *) size, info_to_string(info), baseptr);
   return PMPI_Alloc_mem(size, info, baseptr);
}

int MPI_Allreduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
   INFO(0, "MPI_Allreduce","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
   return PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

int MPI_Alltoall ( void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm )
{
   INFO(0, "MPI_Alltoall","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcount=%d, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)",
             sendbuf, sendcount, type_to_string(sendtype), recvbuf, recvcount, type_to_string(recvtype), comm_to_string(comm));
   return PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
}

int MPI_Alltoallv ( void *sendbuf, int *sendcnts, int *sdispls, MPI_Datatype sendtype, void *recvbuf, int *recvcnts, int *rdispls, MPI_Datatype recvtype, MPI_Comm comm )
{
   INFO(0, "MPI_Alltoallv","(void *sendbuf=%p, int *sendcnts=%p, int *sdispls=%p, MPI_Datatype sendtype=%s, void *recvbuf=%p, int *recvcnts=%p, int *rdispls=%p, MPI_Datatype recvtype=%s, MPI_Comm comm=%s)", 
             sendbuf, sendcnts, sdispls, type_to_string(sendtype), recvbuf, recvcnts, rdispls, type_to_string(recvtype), comm_to_string(comm));
   return PMPI_Alltoallv(sendbuf, sendcnts, sdispls, sendtype, recvbuf, recvcnts, rdispls, recvtype, comm);
}

int MPI_Alltoallw ( void *sendbuf, int *sendcnts, int *sdispls, MPI_Datatype *sendtypes, void *recvbuf, int *recvcnts, int *rdispls, MPI_Datatype *recvtypes, MPI_Comm comm )
{
   INFO(0, "MPI_Alltoallw","(void *sendbuf=%p, int *sendcnts=%p, int *sdispls=%p, MPI_Datatype *sendtypes=%p, void *recvbuf=%p, int *recvcnts=%p, int *rdispls=%p, MPI_Datatype *recvtypes=%p, MPI_Comm comm=%s)", sendbuf, sendcnts, sdispls, sendtypes, recvbuf, recvcnts, rdispls, recvtypes, comm_to_string(comm));
   return PMPI_Alltoallw(sendbuf, sendcnts, sdispls, sendtypes, recvbuf, recvcnts, rdispls, recvtypes, comm);
}


int MPI_Attr_delete ( MPI_Comm comm, int keyval )
{
   INFO(0, "MPI_Attr_delete","(MPI_Comm comm=%s, int keyval=%d)", comm_to_string(comm), keyval);
   return PMPI_Attr_delete(comm, keyval);
}


int MPI_Attr_get ( MPI_Comm comm, int keyval, void *attr_value, int *flag )
{
   INFO(0, "MPI_Attr_get","(MPI_Comm comm=%s, int keyval=%d, void *attr_value=%p, int *flag=%p)", comm_to_string(comm), keyval, attr_value, flag);
   return PMPI_Attr_get(comm, keyval, attr_value, flag);
}


int MPI_Attr_put ( MPI_Comm comm, int keyval, void *attr_value )
{
   INFO(0, "MPI_Attr_put","(MPI_Comm comm=%s, int keyval=%d, void *attr_value=%p)", comm_to_string(comm), keyval, attr_value);
   return PMPI_Attr_put(comm, keyval, attr_value);
}

int MPI_Barrier ( MPI_Comm comm )
{
   INFO(0, "MPI_Barrier","(MPI_Comm comm=%s)", comm_to_string(comm));
   return PMPI_Barrier(comm);
}

int MPI_Bcast ( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
{
   INFO(0, "MPI_Bcast","(void *buffer=%p, int count=%d, MPI_Datatype datatype=%s, int root=%d, MPI_Comm comm=%s)", buffer, count, type_to_string(datatype), root, comm_to_string(comm));
   return PMPI_Bcast(buffer, count, datatype, root, comm);
}

int MPI_Bsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
   INFO(0, "MPI_Bsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
   return PMPI_Bsend(buf, count, datatype, dest, tag, comm);
}


int MPI_Bsend_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Bsend_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Buffer_attach ( void *buffer, int size )
{
   INFO(0, "MPI_Buffer_attach","(void *buffer=%p, int size=%d)", buffer, size);
   return PMPI_Buffer_attach(buffer, size);
}


int MPI_Buffer_detach ( void *buffer, int *size )
{
   INFO(0, "MPI_Buffer_detach","(void *buffer=%p, int *size=%p)", buffer, size);
   return PMPI_Buffer_detach(buffer, size);
}


int MPI_Cancel ( MPI_Request *request )
{
   INFO(0, "MPI_Cancel","(MPI_Request *request=%p)", request);
   return PMPI_Cancel(request);
}


int MPI_Cart_coords ( MPI_Comm comm, int rank, int maxdims, int *coords )
{
   INFO(0, "MPI_Cart_coords","(MPI_Comm comm=%s, int rank=%d, int maxdims=%d, int *coords=%p)", comm_to_string(comm), rank, maxdims, coords);
   return PMPI_Cart_coords(comm, rank, maxdims, coords);
}


int MPI_Cart_create ( MPI_Comm comm_old, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart )
{
   INFO(0, "MPI_Cart_create","(MPI_Comm comm_old=%s, int ndims=%d, int *dims=%p, int *periods=%p, int reorder=%d, MPI_Comm *comm_cart=%p)", comm_to_string(comm_old), ndims, dims, periods, reorder, comm_cart);
   return PMPI_Cart_create(comm_old, ndims, dims, periods, reorder, comm_cart);
}


int MPI_Cartdim_get ( MPI_Comm comm, int *ndims )
{
   INFO(0, "MPI_Cartdim_get","(MPI_Comm comm=%s, int *ndims=%p)", comm_to_string(comm), ndims);
   return PMPI_Cartdim_get(comm, ndims);
}


int MPI_Cart_get ( MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords )
{
   INFO(0, "MPI_Cart_get","(MPI_Comm comm=%s, int maxdims=%d, int *dims=%p, int *periods=%p, int *coords=%p)", comm_to_string(comm), maxdims, dims, periods, coords);
   return PMPI_Cart_get(comm, maxdims, dims, periods, coords);
}


int MPI_Cart_map ( MPI_Comm comm_old, int ndims, int *dims, int *periods, int *newrank )
{
   INFO(0, "MPI_Cart_map","(MPI_Comm comm_old=%s, int ndims=%d, int *dims=%p, int *periods=%p, int *newrank=%p)", comm_to_string(comm_old), ndims, dims, periods, newrank);
   return PMPI_Cart_map(comm_old, ndims, dims, periods, newrank);
}


int MPI_Cart_rank ( MPI_Comm comm, int *coords, int *rank )
{
   INFO(0, "MPI_Cart_rank","(MPI_Comm comm=%s, int *coords=%p, int *rank=%p)", comm_to_string(comm), coords, rank);
   return PMPI_Cart_rank(comm, coords, rank);
}


int MPI_Cart_shift ( MPI_Comm comm, int direction, int displ, int *source, int *dest )
{
   INFO(0, "MPI_Cart_shift","(MPI_Comm comm=%s, int direction=%d, int displ=%d, int *source=%p, int *dest=%p)", comm_to_string(comm), direction, displ, source, dest);
   return PMPI_Cart_shift(comm, direction, displ, source, dest);
}


int MPI_Cart_sub ( MPI_Comm comm, int *remain_dims, MPI_Comm *comm_new )
{
   INFO(0, "MPI_Cart_sub","(MPI_Comm comm=%s, int *remain_dims=%p, MPI_Comm *comm_new=%p)", comm_to_string(comm), remain_dims, comm_new);
   return PMPI_Cart_sub(comm, remain_dims, comm_new);
}


int MPI_Close_port ( char *port_name )
{
   INFO(0, "MPI_Close_port","(char *port_name=%p)", port_name);
   return PMPI_Close_port(port_name);
}


int MPI_Comm_accept ( char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm )
{
   INFO(0, "MPI_Comm_accept","(char *port_name=%p, MPI_Info info=%s, int root=%d, MPI_Comm comm=%s, MPI_Comm *newcomm=%p)", port_name, info_to_string(info), root, comm_to_string(comm), newcomm);
   return PMPI_Comm_accept(port_name, info, root, comm, newcomm);
}

int MPI_Comm_call_errhandler ( MPI_Comm comm, int errorcode )
{
   INFO(0, "MPI_Comm_call_errhandler","(MPI_Comm comm=%s, int errorcode=%d)", comm_to_string(comm), errorcode);
   return PMPI_Comm_call_errhandler(comm, errorcode);
}


int MPI_Comm_compare ( MPI_Comm comm1, MPI_Comm comm2, int *result )
{
   INFO(0, "MPI_Comm_compare","(MPI_Comm comm1=%s, MPI_Comm comm2=%s, int *result=%p)", comm_to_string(comm1), comm_to_string(comm2), result);
   return PMPI_Comm_compare(comm1, comm2, result);
}


int MPI_Comm_connect ( char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm )
{
   INFO(0, "MPI_Comm_connect","(char *port_name=%p, MPI_Info info=%s, int root=%d, MPI_Comm comm=%s, MPI_Comm *newcomm=%p)", port_name, info_to_string(info), root, comm_to_string(comm), newcomm);
   return PMPI_Comm_connect(port_name, info, root, comm, newcomm);
}


int MPI_Comm_create_errhandler ( MPI_Comm_errhandler_fn *function, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Comm_create_errhandler","(MPI_Comm_errhandler_fn *function=%p, MPI_Errhandler *errhandler=%p)", function, errhandler);
   return PMPI_Comm_create_errhandler(function, errhandler);
}


int MPI_Comm_create ( MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm )
{
   INFO(0, "MPI_Comm_create","(MPI_Comm comm=%s, MPI_Group group=%s, MPI_Comm *newcomm=%p)", comm_to_string(comm), group_to_string(group), newcomm);
   return PMPI_Comm_create(comm, group, newcomm);
}


int MPI_Comm_create_keyval ( MPI_Comm_copy_attr_function *comm_copy_attr_fn, MPI_Comm_delete_attr_function *comm_delete_attr_fn, int *comm_keyval, void *extra_state )
{
   INFO(0, "MPI_Comm_create_keyval","(MPI_Comm_copy_attr_function *comm_copy_attr_fn=%p, MPI_Comm_delete_attr_function *comm_delete_attr_fn=%p, int *comm_keyval=%p, void *extra_state=%p)", comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state);
   return PMPI_Comm_create_keyval(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state);
}


int MPI_Comm_delete_attr ( MPI_Comm comm, int comm_keyval )
{
   INFO(0, "MPI_Comm_delete_attr","(MPI_Comm comm=%s, int comm_keyval=%d)", comm_to_string(comm), comm_keyval);
   return PMPI_Comm_delete_attr(comm, comm_keyval);
}


int MPI_Comm_disconnect ( MPI_Comm *comm )
{
   INFO(0, "MPI_Comm_disconnect","(MPI_Comm *comm=%p)", comm);
   return PMPI_Comm_disconnect(comm);
}


int MPI_Comm_dup ( MPI_Comm comm, MPI_Comm *newcomm )
{
   INFO(0, "MPI_Comm_dup","(MPI_Comm comm=%s, MPI_Comm *newcomm=%p)", comm_to_string(comm), newcomm);
   return PMPI_Comm_dup(comm, newcomm);
}


int MPI_Comm_free ( MPI_Comm *comm )
{
   INFO(0, "MPI_Comm_free","(MPI_Comm *comm=%p)", comm);
   return PMPI_Comm_free(comm);
}


int MPI_Comm_free_keyval ( int *comm_keyval )
{
   INFO(0, "MPI_Comm_free_keyval","(int *comm_keyval=%p)", comm_keyval);
   return PMPI_Comm_free_keyval(comm_keyval);
}


int MPI_Comm_get_attr ( MPI_Comm comm, int comm_keyval, void *attribute_val, int *flag )
{
   INFO(0, "MPI_Comm_get_attr","(MPI_Comm comm=%s, int comm_keyval=%d, void *attribute_val=%p, int *flag=%p)", comm_to_string(comm), comm_keyval, attribute_val, flag);
   return PMPI_Comm_get_attr(comm, comm_keyval, attribute_val, flag);
}


int MPI_Comm_get_errhandler ( MPI_Comm comm, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Comm_get_errhandler","(MPI_Comm comm=%s, MPI_Errhandler *errhandler=%p)", comm_to_string(comm), errhandler);
   return PMPI_Comm_get_errhandler(comm, errhandler);
}


int MPI_Comm_get_name ( MPI_Comm comm, char *comm_name, int *resultlen )
{
   INFO(0, "MPI_Comm_get_name","(MPI_Comm comm=%s, char *comm_name=%p, int *resultlen=%p)", comm_to_string(comm), comm_name, resultlen);
   return PMPI_Comm_get_name(comm, comm_name, resultlen);
}


int MPI_Comm_get_parent ( MPI_Comm *parent )
{
   INFO(0, "MPI_Comm_get_parent","(MPI_Comm *parent=%p)", parent);
   return PMPI_Comm_get_parent(parent);
}


int MPI_Comm_group ( MPI_Comm comm, MPI_Group *group )
{
   INFO(0, "MPI_Comm_group","(MPI_Comm comm=%s, MPI_Group *group=%p)", comm_to_string(comm), group);
   return PMPI_Comm_group(comm, group);
}


int MPI_Comm_join ( int fd, MPI_Comm *intercomm )
{
   INFO(0, "MPI_Comm_join","(int fd=%d, MPI_Comm *intercomm=%p)", fd, intercomm);
   return PMPI_Comm_join(fd, intercomm);
}


int MPI_Comm_rank ( MPI_Comm comm, int *rank )
{
   INFO(0, "MPI_Comm_rank","(MPI_Comm comm=%s, int *rank=%p)", comm_to_string(comm), rank);
   return PMPI_Comm_rank(comm, rank);
}


int MPI_Comm_remote_group ( MPI_Comm comm, MPI_Group *group )
{
   INFO(0, "MPI_Comm_remote_group","(MPI_Comm comm=%s, MPI_Group *group=%p)", comm_to_string(comm), group);
   return PMPI_Comm_remote_group(comm, group);
}


int MPI_Comm_remote_size ( MPI_Comm comm, int *size )
{
   INFO(0, "MPI_Comm_remote_size","(MPI_Comm comm=%s, int *size=%p)", comm_to_string(comm), size);
   return PMPI_Comm_remote_size(comm, size);
}


int MPI_Comm_set_attr ( MPI_Comm comm, int comm_keyval, void *attribute_val )
{
   INFO(0, "MPI_Comm_set_attr","(MPI_Comm comm=%s, int comm_keyval=%d, void *attribute_val=%p)", comm_to_string(comm), comm_keyval, attribute_val);
   return PMPI_Comm_set_attr(comm, comm_keyval, attribute_val);
}


int MPI_Comm_set_errhandler ( MPI_Comm comm, MPI_Errhandler errhandler )
{
   INFO(0, "MPI_Comm_set_errhandler","(MPI_Comm comm=%s, MPI_Errhandler errhandler=%p)", comm_to_string(comm), (void *) errhandler);
   return PMPI_Comm_set_errhandler(comm, errhandler);
}


int MPI_Comm_set_name ( MPI_Comm comm, char *comm_name )
{
   INFO(0, "MPI_Comm_set_name","(MPI_Comm comm=%s, char *comm_name=%p)", comm_to_string(comm), comm_name);
   return PMPI_Comm_set_name(comm, comm_name);
}


int MPI_Comm_size ( MPI_Comm comm, int *size )
{
   INFO(0, "MPI_Comm_size","(MPI_Comm comm=%s, int *size=%p)", comm_to_string(comm), size);
   return PMPI_Comm_size(comm, size);
}


int MPI_Comm_spawn ( char *command, char *argv[], int maxprocs, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[] )
{
   INFO(0, "MPI_Comm_spawn","(char *command=%p, char *argv[]=%p, int maxprocs=%d, MPI_Info info=%s, int root=%d, MPI_Comm comm=%s, MPI_Comm *intercomm=%p, int array_of_errcodes[]=%p)", command, argv, maxprocs, info_to_string(info), root, comm_to_string(comm), intercomm, array_of_errcodes);
   return PMPI_Comm_spawn(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes);
}


int MPI_Comm_spawn_multiple ( int count, char *array_of_commands[], char* *array_of_argv[], int array_of_maxprocs[], MPI_Info array_of_info[], int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[] )
{
   INFO(0, "MPI_Comm_spawn_multiple","(int count=%d, char *array_of_commands[]=%p, char* *array_of_argv[]=%p, int array_of_maxprocs[]=%p, MPI_Info array_of_info[]=%p, int root=%d, MPI_Comm comm=%s, MPI_Comm *intercomm=%p, int array_of_errcodes[]=%p)", count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm_to_string(comm), intercomm, array_of_errcodes);
   return PMPI_Comm_spawn_multiple(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes);
}


int MPI_Comm_split ( MPI_Comm comm, int color, int key, MPI_Comm *newcomm )
{
   INFO(0, "MPI_Comm_split","(MPI_Comm comm=%s, int color=%d, int key=%d, MPI_Comm *newcomm=%p)", comm_to_string(comm), color, key, newcomm);
   return PMPI_Comm_split(comm, color, key, newcomm);
}

int MPI_Comm_test_inter ( MPI_Comm comm, int *flag )
{
   INFO(0, "MPI_Comm_test_inter","(MPI_Comm comm=%s, int *flag=%p)", comm_to_string(comm), flag);
   return PMPI_Comm_test_inter(comm, flag);
}


int MPI_Dims_create ( int nnodes, int ndims, int *dims )
{
   INFO(0, "MPI_Dims_create","(int nnodes=%d, int ndims=%d, int *dims=%p)", nnodes, ndims, dims);
   return PMPI_Dims_create(nnodes, ndims, dims);
}

int MPI_Errhandler_create ( MPI_Handler_function *function, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Errhandler_create","(MPI_Handler_function *function=%p, MPI_Errhandler *errhandler=%p)", function, errhandler);
   return PMPI_Errhandler_create(function, errhandler);
}


int MPI_Errhandler_free ( MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Errhandler_free","(MPI_Errhandler *errhandler=%p)", errhandler);
   return PMPI_Errhandler_free(errhandler);
}


int MPI_Errhandler_get ( MPI_Comm comm, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Errhandler_get","(MPI_Comm comm=%s, MPI_Errhandler *errhandler=%p)", comm_to_string(comm), errhandler);
   return PMPI_Errhandler_get(comm, errhandler);
}


int MPI_Errhandler_set ( MPI_Comm comm, MPI_Errhandler errhandler )
{
   INFO(0, "MPI_Errhandler_set","(MPI_Comm comm=%s, MPI_Errhandler errhandler=%p)", comm_to_string(comm), (void *) errhandler);
   return PMPI_Errhandler_set(comm, errhandler);
}


int MPI_Error_class ( int errorcode, int *errorclass )
{
   INFO(0, "MPI_Error_class","(int errorcode=%d, int *errorclass=%p)", errorcode, errorclass);
   return PMPI_Error_class(errorcode, errorclass);
}

int MPI_Error_string ( int errorcode, char *string, int *resultlen )
{
   INFO(0, "MPI_Error_string","(int errorcode=%d, char *string=%p, int *resultlen=%p)", errorcode, string, resultlen);
   return PMPI_Error_string(errorcode, string, resultlen);
}

int MPI_Exscan ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
   INFO(0, "MPI_Exscan","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
   return PMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm);
}


int MPI_File_call_errhandler ( MPI_File fh, int errorcode )
{
   INFO(0, "MPI_File_call_errhandler","(MPI_File fh=%s, int errorcode=%d)", file_to_string(fh), errorcode);
   return PMPI_File_call_errhandler(fh, errorcode);
}


int MPI_File_close ( MPI_File *mpi_fh )
{
   INFO(0, "MPI_File_close","(MPI_File *mpi_fh=%p)", mpi_fh);
   return PMPI_File_close(mpi_fh);
}


int MPI_File_create_errhandler ( MPI_File_errhandler_fn *function, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_File_create_errhandler","(MPI_File_errhandler_fn *function=%p, MPI_Errhandler *errhandler=%p)", function, errhandler);
   return PMPI_File_create_errhandler(function, errhandler);
}


int MPI_File_delete ( char *filename, MPI_Info info )
{
   INFO(0, "MPI_File_delete","(char *filename=%p, MPI_Info info=%s)", filename, info_to_string(info));
   return PMPI_File_delete(filename, info);
}


int MPI_File_get_amode ( MPI_File mpi_fh, int *amode )
{
   INFO(0, "MPI_File_get_amode","(MPI_File mpi_fh=%s, int *amode=%p)", file_to_string(mpi_fh), amode);
   return PMPI_File_get_amode(mpi_fh, amode);
}


int MPI_File_get_atomicity ( MPI_File mpi_fh, int *flag )
{
   INFO(0, "MPI_File_get_atomicity","(MPI_File mpi_fh=%s, int *flag=%p)", file_to_string(mpi_fh), flag);
   return PMPI_File_get_atomicity(mpi_fh, flag);
}


int MPI_File_get_byte_offset ( MPI_File mpi_fh, MPI_Offset offset, MPI_Offset *disp )
{
   INFO(0, "MPI_File_get_byte_offset","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, MPI_Offset *disp=%p)", file_to_string(mpi_fh), (void *) offset, disp);
   return PMPI_File_get_byte_offset(mpi_fh, offset, disp);
}


int MPI_File_get_errhandler ( MPI_File file, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_File_get_errhandler","(MPI_File file=%s, MPI_Errhandler *errhandler=%p)", file_to_string(file), errhandler);
   return PMPI_File_get_errhandler(file, errhandler);
}


int MPI_File_get_group ( MPI_File mpi_fh, MPI_Group *group )
{
   INFO(0, "MPI_File_get_group","(MPI_File mpi_fh=%s, MPI_Group *group=%p)", file_to_string(mpi_fh), group);
   return PMPI_File_get_group(mpi_fh, group);
}


int MPI_File_get_info ( MPI_File mpi_fh, MPI_Info *info_used )
{
   INFO(0, "MPI_File_get_info","(MPI_File mpi_fh=%s, MPI_Info *info_used=%p)", file_to_string(mpi_fh), info_used);
   return PMPI_File_get_info(mpi_fh, info_used);
}


int MPI_File_get_position ( MPI_File mpi_fh, MPI_Offset *offset )
{
   INFO(0, "MPI_File_get_position","(MPI_File mpi_fh=%s, MPI_Offset *offset=%p)", file_to_string(mpi_fh), offset);
   return PMPI_File_get_position(mpi_fh, offset);
}


int MPI_File_get_position_shared ( MPI_File mpi_fh, MPI_Offset *offset )
{
   INFO(0, "MPI_File_get_position_shared","(MPI_File mpi_fh=%s, MPI_Offset *offset=%p)", file_to_string(mpi_fh), offset);
   return PMPI_File_get_position_shared(mpi_fh, offset);
}


int MPI_File_get_size ( MPI_File mpi_fh, MPI_Offset *size )
{
   INFO(0, "MPI_File_get_size","(MPI_File mpi_fh=%s, MPI_Offset *size=%p)", file_to_string(mpi_fh), size);
   return PMPI_File_get_size(mpi_fh, size);
}


int MPI_File_get_type_extent ( MPI_File mpi_fh, MPI_Datatype datatype, MPI_Aint *extent )
{
   INFO(0, "MPI_File_get_type_extent","(MPI_File mpi_fh=%s, MPI_Datatype datatype=%s, MPI_Aint *extent=%p)", file_to_string(mpi_fh), type_to_string(datatype), extent);
   return PMPI_File_get_type_extent(mpi_fh, datatype, extent);
}


int MPI_File_get_view ( MPI_File mpi_fh, MPI_Offset *disp, MPI_Datatype *etype, MPI_Datatype *filetype, char *datarep )
{
   INFO(0, "MPI_File_get_view","(MPI_File mpi_fh=%s, MPI_Offset *disp=%p, MPI_Datatype *etype=%p, MPI_Datatype *filetype=%p, char *datarep=%p)", file_to_string(mpi_fh), disp, etype, filetype, datarep);
   return PMPI_File_get_view(mpi_fh, disp, etype, filetype, datarep);
}


int MPI_File_iread_at ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPIO_Request *request )
{
   INFO(0, "MPI_File_iread_at","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPIO_Request *request=%p)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype), request);
   return PMPI_File_iread_at(mpi_fh, offset, buf, count, datatype, request);
}


int MPI_File_iread ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request )
{
   INFO(0, "MPI_File_iread","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Request *request=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), request);
   return PMPI_File_iread(mpi_fh, buf, count, datatype, request);
}


int MPI_File_iread_shared ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request )
{
   INFO(0, "MPI_File_iread_shared","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Request *request=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), request);
   return PMPI_File_iread_shared(mpi_fh, buf, count, datatype, request);
}


int MPI_File_iwrite_at ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPIO_Request *request )
{
   INFO(0, "MPI_File_iwrite_at","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPIO_Request *request=%p)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype), request);
   return PMPI_File_iwrite_at(mpi_fh, offset, buf, count, datatype, request);
}


int MPI_File_iwrite ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request )
{
   INFO(0, "MPI_File_iwrite","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Request *request=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), request);
   return PMPI_File_iwrite(mpi_fh, buf, count, datatype, request);
}


int MPI_File_iwrite_shared ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPIO_Request *request )
{
   INFO(0, "MPI_File_iwrite_shared","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPIO_Request *request=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), request);
   return PMPI_File_iwrite_shared(mpi_fh, buf, count, datatype, request);
}


int MPI_File_open ( MPI_Comm comm, char *filename, int amode, MPI_Info info, MPI_File *fh )
{
   INFO(0, "MPI_File_open","(MPI_Comm comm=%s, char *filename=%p, int amode=%d, MPI_Info info=%s, MPI_File *fh=%p)", comm_to_string(comm), filename, amode, info_to_string(info), fh);
   return PMPI_File_open(comm, filename, amode, info, fh);
}


int MPI_File_preallocate ( MPI_File mpi_fh, MPI_Offset size )
{
   INFO(0, "MPI_File_preallocate","(MPI_File mpi_fh=%s, MPI_Offset size=%p)", file_to_string(mpi_fh), (void *) size);
   return PMPI_File_preallocate(mpi_fh, size);
}


int MPI_File_read_all_begin ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype )
{
   INFO(0, "MPI_File_read_all_begin","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s)", file_to_string(mpi_fh), buf, count, type_to_string(datatype));
   return PMPI_File_read_all_begin(mpi_fh, buf, count, datatype);
}


int MPI_File_read_all_end ( MPI_File mpi_fh, void *buf, MPI_Status *status )
{
   INFO(0, "MPI_File_read_all_end","(MPI_File mpi_fh=%s, void *buf=%p, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, status);
   return PMPI_File_read_all_end(mpi_fh, buf, status);
}


int MPI_File_read_all ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_read_all","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_read_all(mpi_fh, buf, count, datatype, status);
}


int MPI_File_read_at_all_begin ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype )
{
   INFO(0, "MPI_File_read_at_all_begin","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype));
   return PMPI_File_read_at_all_begin(mpi_fh, offset, buf, count, datatype);
}


int MPI_File_read_at_all_end ( MPI_File mpi_fh, void *buf, MPI_Status *status )
{
   INFO(0, "MPI_File_read_at_all_end","(MPI_File mpi_fh=%s, void *buf=%p, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, status);
   return PMPI_File_read_at_all_end(mpi_fh, buf, status);
}


int MPI_File_read_at_all ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_read_at_all","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype), status);
   return PMPI_File_read_at_all(mpi_fh, offset, buf, count, datatype, status);
}


int MPI_File_read_at ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_read_at","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype), status);
   return PMPI_File_read_at(mpi_fh, offset, buf, count, datatype, status);
}


int MPI_File_read ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_read","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_read(mpi_fh, buf, count, datatype, status);
}


int MPI_File_read_ordered_begin ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype )
{
   INFO(0, "MPI_File_read_ordered_begin","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s)", file_to_string(mpi_fh), buf, count, type_to_string(datatype));
   return PMPI_File_read_ordered_begin(mpi_fh, buf, count, datatype);
}


int MPI_File_read_ordered_end ( MPI_File mpi_fh, void *buf, MPI_Status *status )
{
   INFO(0, "MPI_File_read_ordered_end","(MPI_File mpi_fh=%s, void *buf=%p, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, status);
   return PMPI_File_read_ordered_end(mpi_fh, buf, status);
}


int MPI_File_read_ordered ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_read_ordered","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_read_ordered(mpi_fh, buf, count, datatype, status);
}


int MPI_File_read_shared ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_read_shared","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_read_shared(mpi_fh, buf, count, datatype, status);
}


int MPI_File_seek ( MPI_File mpi_fh, MPI_Offset offset, int whence )
{
   INFO(0, "MPI_File_seek","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, int whence=%d)", file_to_string(mpi_fh), (void *) offset, whence);
   return PMPI_File_seek(mpi_fh, offset, whence);
}


int MPI_File_seek_shared ( MPI_File mpi_fh, MPI_Offset offset, int whence )
{
   INFO(0, "MPI_File_seek_shared","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, int whence=%d)", file_to_string(mpi_fh), (void *) offset, whence);
   return PMPI_File_seek_shared(mpi_fh, offset, whence);
}


int MPI_File_set_atomicity ( MPI_File mpi_fh, int flag )
{
   INFO(0, "MPI_File_set_atomicity","(MPI_File mpi_fh=%s, int flag=%d)", file_to_string(mpi_fh), flag);
   return PMPI_File_set_atomicity(mpi_fh, flag);
}


int MPI_File_set_errhandler ( MPI_File file, MPI_Errhandler errhandler )
{
   INFO(0, "MPI_File_set_errhandler","(MPI_File file=%s, MPI_Errhandler errhandler=%p)", file_to_string(file), (void *) errhandler);
   return PMPI_File_set_errhandler(file, errhandler);
}


int MPI_File_set_info ( MPI_File mpi_fh, MPI_Info info )
{
   INFO(0, "MPI_File_set_info","(MPI_File mpi_fh=%s, MPI_Info info=%s)", file_to_string(mpi_fh), info_to_string(info));
   return PMPI_File_set_info(mpi_fh, info);
}


int MPI_File_set_size ( MPI_File mpi_fh, MPI_Offset size )
{
   INFO(0, "MPI_File_set_size","(MPI_File mpi_fh=%s, MPI_Offset size=%p)", file_to_string(mpi_fh), (void *) size);
   return PMPI_File_set_size(mpi_fh, size);
}


int MPI_File_set_view ( MPI_File mpi_fh, MPI_Offset disp, MPI_Datatype etype, MPI_Datatype filetype, char *datarep, MPI_Info info )
{
   INFO(0, "MPI_File_set_view","(MPI_File mpi_fh=%s, MPI_Offset disp=%p, MPI_Datatype etype=%s, MPI_Datatype filetype=%s, char *datarep=%p, MPI_Info info=%s)", file_to_string(mpi_fh), (void *) disp, type_to_string(etype), type_to_string(filetype), datarep, info_to_string(info));
   return PMPI_File_set_view(mpi_fh, disp, etype, filetype, datarep, info);
}


int MPI_File_sync ( MPI_File mpi_fh )
{
   INFO(0, "MPI_File_sync","(MPI_File mpi_fh=%s)", file_to_string(mpi_fh));
   return PMPI_File_sync(mpi_fh);
}


int MPI_File_write_all_begin ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype )
{
   INFO(0, "MPI_File_write_all_begin","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s)", file_to_string(mpi_fh), buf, count, type_to_string(datatype));
   return PMPI_File_write_all_begin(mpi_fh, buf, count, datatype);
}


int MPI_File_write_all_end ( MPI_File mpi_fh, void *buf, MPI_Status *status )
{
   INFO(0, "MPI_File_write_all_end","(MPI_File mpi_fh=%s, void *buf=%p, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, status);
   return PMPI_File_write_all_end(mpi_fh, buf, status);
}


int MPI_File_write_all ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_write_all","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_write_all(mpi_fh, buf, count, datatype, status);
}


int MPI_File_write_at_all_begin ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype )
{
   INFO(0, "MPI_File_write_at_all_begin","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype));
   return PMPI_File_write_at_all_begin(mpi_fh, offset, buf, count, datatype);
}


int MPI_File_write_at_all_end ( MPI_File mpi_fh, void *buf, MPI_Status *status )
{
   INFO(0, "MPI_File_write_at_all_end","(MPI_File mpi_fh=%s, void *buf=%p, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, status);
   return PMPI_File_write_at_all_end(mpi_fh, buf, status);
}


int MPI_File_write_at_all ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_write_at_all","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype), status);
   return PMPI_File_write_at_all(mpi_fh, offset, buf, count, datatype, status);
}


int MPI_File_write_at ( MPI_File mpi_fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_write_at","(MPI_File mpi_fh=%s, MPI_Offset offset=%p, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), (void *) offset, buf, count, type_to_string(datatype), status);
   return PMPI_File_write_at(mpi_fh, offset, buf, count, datatype, status);
}


int MPI_File_write ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_write","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_write(mpi_fh, buf, count, datatype, status);
}


int MPI_File_write_ordered_begin ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype )
{
   INFO(0, "MPI_File_write_ordered_begin","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s)", file_to_string(mpi_fh), buf, count, type_to_string(datatype));
   return PMPI_File_write_ordered_begin(mpi_fh, buf, count, datatype);
}


int MPI_File_write_ordered_end ( MPI_File mpi_fh, void *buf, MPI_Status *status )
{
   INFO(0, "MPI_File_write_ordered_end","(MPI_File mpi_fh=%s, void *buf=%p, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, status);
   return PMPI_File_write_ordered_end(mpi_fh, buf, status);
}


int MPI_File_write_ordered ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_write_ordered","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_write_ordered(mpi_fh, buf, count, datatype, status);
}


int MPI_File_write_shared ( MPI_File mpi_fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status )
{
   INFO(0, "MPI_File_write_shared","(MPI_File mpi_fh=%s, void *buf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Status *status=%p)", file_to_string(mpi_fh), buf, count, type_to_string(datatype), status);
   return PMPI_File_write_shared(mpi_fh, buf, count, datatype, status);
}


int MPI_Finalized ( int *flag )
{
   INFO(0, "MPI_Finalized","(int *flag=%p)", flag);
   return PMPI_Finalized(flag);
}

int MPI_Finalize (  )
{
   INFO(0, "MPI_Finalize","()");
   return PMPI_Finalize();
}


int MPI_Free_mem ( void *base )
{
   INFO(0, "MPI_Free_mem","(void *base=%p)", base);
   return PMPI_Free_mem(base);
}

int MPI_Gather ( void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
   INFO(0, "MPI_Gather","(void *sendbuf=%p, int sendcnt=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcnt=%d, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)",
             sendbuf, sendcnt, type_to_string(sendtype), recvbuf, recvcnt, type_to_string(recvtype), root, comm_to_string(comm));
   return PMPI_Gather(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm);
}


int MPI_Gatherv ( void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int *recvcnts, int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
   INFO(0, "MPI_Gatherv","(void *sendbuf=%p, int sendcnt=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int *recvcnts=%p, int *displs=%p, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)", 
             sendbuf, sendcnt, type_to_string(sendtype), recvbuf, recvcnts, displs, type_to_string(recvtype), root, comm_to_string(comm));
   return PMPI_Gatherv(sendbuf, sendcnt, sendtype, recvbuf, recvcnts, displs, recvtype, root, comm);
}


int MPI_Get_address ( void *location, MPI_Aint *address )
{
   INFO(0, "MPI_Get_address","(void *location=%p, MPI_Aint *address=%p)", location, address);
   return PMPI_Get_address(location, address);
}


int MPI_Get_count ( MPI_Status *status, MPI_Datatype datatype, int *count )
{
   INFO(0, "MPI_Get_count","(MPI_Status *status=%p, MPI_Datatype datatype=%s, int *count=%p)", status, type_to_string(datatype), count);
   return PMPI_Get_count(status, datatype, count);
}


int MPI_Get_elements ( MPI_Status *status, MPI_Datatype datatype, int *elements )
{
   INFO(0, "MPI_Get_elements","(MPI_Status *status=%p, MPI_Datatype datatype=%s, int *elements=%p)", status, type_to_string(datatype), elements);
   return PMPI_Get_elements(status, datatype, elements);
}


int MPI_Get ( void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win )
{
   INFO(0, "MPI_Get","(void *origin_addr=%p, int origin_count=%d, MPI_Datatype origin_datatype=%s, int target_rank=%d, MPI_Aint target_disp=%p, int target_count=%d, MPI_Datatype target_datatype=%s, MPI_Win win=%s)", origin_addr, origin_count, type_to_string(origin_datatype), target_rank, (void *) target_disp, target_count, type_to_string(target_datatype), win_to_string(win));
   return PMPI_Get(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win);
}

int MPI_Get_processor_name ( char *name, int *resultlen )
{
   INFO(0, "MPI_Get_processor_name","(char *name=%p, int *resultlen=%p)", name, resultlen);
   return PMPI_Get_processor_name(name, resultlen);
}

int MPI_Get_version ( int *version, int *subversion )
{
   INFO(0, "MPI_Get_version","(int *version=%p, int *subversion=%p)", version, subversion);
   return PMPI_Get_version(version, subversion);
}


int MPI_Graph_create ( MPI_Comm comm_old, int nnodes, int *indx, int *edges, int reorder, MPI_Comm *comm_graph )
{
   INFO(0, "MPI_Graph_create","(MPI_Comm comm_old=%s, int nnodes=%d, int *indx=%p, int *edges=%p, int reorder=%d, MPI_Comm *comm_graph=%p)", comm_to_string(comm_old), nnodes, indx, edges, reorder, comm_graph);
   return PMPI_Graph_create(comm_old, nnodes, indx, edges, reorder, comm_graph);
}


int MPI_Graphdims_get ( MPI_Comm comm, int *nnodes, int *nedges )
{
   INFO(0, "MPI_Graphdims_get","(MPI_Comm comm=%s, int *nnodes=%p, int *nedges=%p)", comm_to_string(comm), nnodes, nedges);
   return PMPI_Graphdims_get(comm, nnodes, nedges);
}


int MPI_Graph_get ( MPI_Comm comm, int maxindex, int maxedges, int *indx, int *edges )
{
   INFO(0, "MPI_Graph_get","(MPI_Comm comm=%s, int maxindex=%d, int maxedges=%d, int *indx=%p, int *edges=%p)", comm_to_string(comm), maxindex, maxedges, indx, edges);
   return PMPI_Graph_get(comm, maxindex, maxedges, indx, edges);
}


int MPI_Graph_map ( MPI_Comm comm_old, int nnodes, int *indx, int *edges, int *newrank )
{
   INFO(0, "MPI_Graph_map","(MPI_Comm comm_old=%s, int nnodes=%d, int *indx=%p, int *edges=%p, int *newrank=%p)", comm_to_string(comm_old), nnodes, indx, edges, newrank);
   return PMPI_Graph_map(comm_old, nnodes, indx, edges, newrank);
}


int MPI_Graph_neighbors_count ( MPI_Comm comm, int rank, int *nneighbors )
{
   INFO(0, "MPI_Graph_neighbors_count","(MPI_Comm comm=%s, int rank=%d, int *nneighbors=%p)", comm_to_string(comm), rank, nneighbors);
   return PMPI_Graph_neighbors_count(comm, rank, nneighbors);
}


int MPI_Graph_neighbors ( MPI_Comm comm, int rank, int maxneighbors, int *neighbors )
{
   INFO(0, "MPI_Graph_neighbors","(MPI_Comm comm=%s, int rank=%d, int maxneighbors=%d, int *neighbors=%p)", comm_to_string(comm), rank, maxneighbors, neighbors);
   return PMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors);
}


int MPI_Grequest_complete ( MPI_Request request )
{
   INFO(0, "MPI_Grequest_complete","(MPI_Request request=%s)", request_to_string(request));
   return PMPI_Grequest_complete(request);
}


int MPI_Grequest_start ( MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, void *extra_state, MPI_Request *request )
{
   INFO(0, "MPI_Grequest_start","(MPI_Grequest_query_function *query_fn=%p, MPI_Grequest_free_function *free_fn=%p, MPI_Grequest_cancel_function *cancel_fn=%p, void *extra_state=%p, MPI_Request *request=%p)", query_fn, free_fn, cancel_fn, extra_state, request);
   return PMPI_Grequest_start(query_fn, free_fn, cancel_fn, extra_state, request);
}


int MPI_Group_compare ( MPI_Group group1, MPI_Group group2, int *result )
{
   INFO(0, "MPI_Group_compare","(MPI_Group group1=%s, MPI_Group group2=%s, int *result=%p)", group_to_string(group1), group_to_string(group2), result);
   return PMPI_Group_compare(group1, group2, result);
}


int MPI_Group_difference ( MPI_Group group1, MPI_Group group2, MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_difference","(MPI_Group group1=%s, MPI_Group group2=%s, MPI_Group *newgroup=%p)", group_to_string(group1), group_to_string(group2), newgroup);
   return PMPI_Group_difference(group1, group2, newgroup);
}


int MPI_Group_excl ( MPI_Group group, int n, int *ranks, MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_excl","(MPI_Group group=%s, int n=%d, int *ranks=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranks, newgroup);
   return PMPI_Group_excl(group, n, ranks, newgroup);
}


int MPI_Group_free ( MPI_Group *group )
{
   INFO(0, "MPI_Group_free","(MPI_Group *group=%p)", group);
   return PMPI_Group_free(group);
}


int MPI_Group_incl ( MPI_Group group, int n, int *ranks, MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_incl","(MPI_Group group=%s, int n=%d, int *ranks=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranks, newgroup);
   return PMPI_Group_incl(group, n, ranks, newgroup);
}


int MPI_Group_intersection ( MPI_Group group1, MPI_Group group2, MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_intersection","(MPI_Group group1=%s, MPI_Group group2=%s, MPI_Group *newgroup=%p)", group_to_string(group1), group_to_string(group2), newgroup);
   return PMPI_Group_intersection(group1, group2, newgroup);
}


int MPI_Group_range_excl ( MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_range_excl","(MPI_Group group=%s, int n=%d, int ranges[][3]=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranges, newgroup);
   return PMPI_Group_range_excl(group, n, ranges, newgroup);
}


int MPI_Group_range_incl ( MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_range_incl","(MPI_Group group=%s, int n=%d, int ranges[][3]=%p, MPI_Group *newgroup=%p)", group_to_string(group), n, ranges, newgroup);
   return PMPI_Group_range_incl(group, n, ranges, newgroup);
}


int MPI_Group_rank ( MPI_Group group, int *rank )
{
   INFO(0, "MPI_Group_rank","(MPI_Group group=%s, int *rank=%p)", group_to_string(group), rank);
   return PMPI_Group_rank(group, rank);
}


int MPI_Group_size ( MPI_Group group, int *size )
{
   INFO(0, "MPI_Group_size","(MPI_Group group=%s, int *size=%p)", group_to_string(group), size);
   return PMPI_Group_size(group, size);
}


int MPI_Group_translate_ranks ( MPI_Group group1, int n, int *ranks1, MPI_Group group2, int *ranks2 )
{
   INFO(0, "MPI_Group_translate_ranks","(MPI_Group group1=%s, int n=%d, int *ranks1=%p, MPI_Group group2=%s, int *ranks2=%p)", group_to_string(group1), n, ranks1, group_to_string(group2), ranks2);
   return PMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2);
}


int MPI_Group_union ( MPI_Group group1, MPI_Group group2, MPI_Group *newgroup )
{
   INFO(0, "MPI_Group_union","(MPI_Group group1=%s, MPI_Group group2=%s, MPI_Group *newgroup=%p)", group_to_string(group1), group_to_string(group2), newgroup);
   return PMPI_Group_union(group1, group2, newgroup);
}


int MPI_Ibsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Ibsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Info_create ( MPI_Info *info )
{
   INFO(0, "MPI_Info_create","(MPI_Info *info=%p)", info);
   return PMPI_Info_create(info);
}


int MPI_Info_delete ( MPI_Info info, char *key )
{
   INFO(0, "MPI_Info_delete","(MPI_Info info=%s, char *key=%p)", info_to_string(info), key);
   return PMPI_Info_delete(info, key);
}


int MPI_Info_dup ( MPI_Info info, MPI_Info *newinfo )
{
   INFO(0, "MPI_Info_dup","(MPI_Info info=%s, MPI_Info *newinfo=%p)", info_to_string(info), newinfo);
   return PMPI_Info_dup(info, newinfo);
}


int MPI_Info_free ( MPI_Info *info )
{
   INFO(0, "MPI_Info_free","(MPI_Info *info=%p)", info);
   return PMPI_Info_free(info);
}


int MPI_Info_get ( MPI_Info info, char *key, int valuelen, char *value, int *flag )
{
   INFO(0, "MPI_Info_get","(MPI_Info info=%s, char *key=%p, int valuelen=%d, char *value=%p, int *flag=%p)", info_to_string(info), key, valuelen, value, flag);
   return PMPI_Info_get(info, key, valuelen, value, flag);
}


int MPI_Info_get_nkeys ( MPI_Info info, int *nkeys )
{
   INFO(0, "MPI_Info_get_nkeys","(MPI_Info info=%s, int *nkeys=%p)", info_to_string(info), nkeys);
   return PMPI_Info_get_nkeys(info, nkeys);
}


int MPI_Info_get_nthkey ( MPI_Info info, int n, char *key )
{
   INFO(0, "MPI_Info_get_nthkey","(MPI_Info info=%s, int n=%d, char *key=%p)", info_to_string(info), n, key);
   return PMPI_Info_get_nthkey(info, n, key);
}


int MPI_Info_get_valuelen ( MPI_Info info, char *key, int *valuelen, int *flag )
{
   INFO(0, "MPI_Info_get_valuelen","(MPI_Info info=%s, char *key=%p, int *valuelen=%p, int *flag=%p)", info_to_string(info), key, valuelen, flag);
   return PMPI_Info_get_valuelen(info, key, valuelen, flag);
}


int MPI_Info_set ( MPI_Info info, char *key, char *value )
{
   INFO(0, "MPI_Info_set","(MPI_Info info=%s, char *key=%p, char *value=%p)", info_to_string(info), key, value);
   return PMPI_Info_set(info, key, value);
}


int MPI_Init ( int *argc, char ***argv )
{
   INFO(0, "MPI_Init","(int *argc=%p, char ***argv=%p)", argc, argv);
   return PMPI_Init(argc, argv);
}


int MPI_Initialized ( int *flag )
{
   INFO(0, "MPI_Initialized","(int *flag=%p)", flag);
   return PMPI_Initialized(flag);
}


int MPI_Init_thread ( int *argc, char ***argv, int required, int *provided )
{
   INFO(0, "MPI_Init_thread","(int *argc=%p, char ***argv=%p, int required=%d, int *provided=%p)", argc, argv, required, provided);
   return PMPI_Init_thread(argc, argv, required, provided);
}


int MPI_Intercomm_create ( MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader, int tag, MPI_Comm *newintercomm )
{
   INFO(0, "MPI_Intercomm_create","(MPI_Comm local_comm=%s, int local_leader=%d, MPI_Comm peer_comm=%s, int remote_leader=%d, int tag=%d, MPI_Comm *newintercomm=%p)", comm_to_string(local_comm), local_leader, comm_to_string(peer_comm), remote_leader, tag, newintercomm);
   return PMPI_Intercomm_create(local_comm, local_leader, peer_comm, remote_leader, tag, newintercomm);
}


int MPI_Intercomm_merge ( MPI_Comm intercomm, int high, MPI_Comm *newintracomm )
{
   INFO(0, "MPI_Intercomm_merge","(MPI_Comm intercomm=%s, int high=%d, MPI_Comm *newintracomm=%p)", comm_to_string(intercomm), high, newintracomm);
   return PMPI_Intercomm_merge(intercomm, high, newintracomm);
}


int MPI_Iprobe ( int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status )
{
   INFO(0, "MPI_Iprobe","(int source=%d, int tag=%d, MPI_Comm comm=%s, int *flag=%p, MPI_Status *status=%p)", source, tag, comm_to_string(comm), flag, status);
   return PMPI_Iprobe(source, tag, comm, flag, status);
}


int MPI_Irecv ( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Irecv","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), source, tag, comm_to_string(comm), request);
   return PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
}


int MPI_Irsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Irsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Isend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Isend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Issend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Issend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Is_thread_main ( int *flag )
{
   INFO(0, "MPI_Is_thread_main","(int *flag=%p)", flag);
   return PMPI_Is_thread_main(flag);
}


int MPI_Keyval_create ( MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state )
{
   INFO(0, "MPI_Keyval_create","(MPI_Copy_function *copy_fn=%p, MPI_Delete_function *delete_fn=%p, int *keyval=%p, void *extra_state=%p)", copy_fn, delete_fn, keyval, extra_state);
   return PMPI_Keyval_create(copy_fn, delete_fn, keyval, extra_state);
}


int MPI_Keyval_free ( int *keyval )
{
   INFO(0, "MPI_Keyval_free","(int *keyval=%p)", keyval);
   return PMPI_Keyval_free(keyval);
}


int MPI_Lookup_name ( char *service_name, MPI_Info info, char *port_name )
{
   INFO(0, "MPI_Lookup_name","(char *service_name=%p, MPI_Info info=%s, char *port_name=%p)", service_name, info_to_string(info), port_name);
   return PMPI_Lookup_name(service_name, info, port_name);
}

int MPI_Op_create ( MPI_User_function *function, int commute, MPI_Op *op )
{
   INFO(0, "MPI_Op_create","(MPI_User_function *function=%p, int commute=%d, MPI_Op *op=%p)", function, commute, op);
   return PMPI_Op_create(function, commute, op);
}


int MPI_Open_port ( MPI_Info info, char *port_name )
{
   INFO(0, "MPI_Open_port","(MPI_Info info=%s, char *port_name=%p)", info_to_string(info), port_name);
   return PMPI_Open_port(info, port_name);
}


int MPI_Op_free ( MPI_Op *op )
{
   INFO(0, "MPI_Op_free","(MPI_Op *op=%p)", op);
   return PMPI_Op_free(op);
}


int MPI_Pack_external ( char *datarep, void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, MPI_Aint outcount, MPI_Aint *position )
{
   INFO(0, "MPI_Pack_external","(char *datarep=%p, void *inbuf=%p, int incount=%d, MPI_Datatype datatype=%s, void *outbuf=%p, MPI_Aint outcount=%p, MPI_Aint *position=%p)", datarep, inbuf, incount, type_to_string(datatype), outbuf, (void *) outcount, position);
   return PMPI_Pack_external(datarep, inbuf, incount, datatype, outbuf, outcount, position);
}


int MPI_Pack_external_size ( char *datarep, int incount, MPI_Datatype datatype, MPI_Aint *size )
{
   INFO(0, "MPI_Pack_external_size","(char *datarep=%p, int incount=%d, MPI_Datatype datatype=%s, MPI_Aint *size=%p)", datarep, incount, type_to_string(datatype), size);
   return PMPI_Pack_external_size(datarep, incount, datatype, size);
}


int MPI_Pack ( void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outcount, int *position, MPI_Comm comm )
{
   INFO(0, "MPI_Pack","(void *inbuf=%p, int incount=%d, MPI_Datatype datatype=%s, void *outbuf=%p, int outcount=%d, int *position=%p, MPI_Comm comm=%s)", inbuf, incount, type_to_string(datatype), outbuf, outcount, position, comm_to_string(comm));
   return PMPI_Pack(inbuf, incount, datatype, outbuf, outcount, position, comm);
}


int MPI_Pack_size ( int incount, MPI_Datatype datatype, MPI_Comm comm, int *size )
{
   INFO(0, "MPI_Pack_size","(int incount=%d, MPI_Datatype datatype=%s, MPI_Comm comm=%s, int *size=%p)", incount, type_to_string(datatype), comm_to_string(comm), size);
   return PMPI_Pack_size(incount, datatype, comm, size);
}

int MPI_Pcontrol ( const int level, ... )
{
   INFO(0, "MPI_Pcontrol","(const int level=%d, ...)", level);
   return PMPI_Pcontrol(int, ...);
}

int MPI_Probe ( int source, int tag, MPI_Comm comm, MPI_Status *status )
{
   INFO(0, "MPI_Probe","(int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", source, tag, comm_to_string(comm), status);
   return PMPI_Probe(source, tag, comm, status);
}


int MPI_Publish_name ( char *service_name, MPI_Info info, char *port_name )
{
   INFO(0, "MPI_Publish_name","(char *service_name=%p, MPI_Info info=%s, char *port_name=%p)", service_name, info_to_string(info), port_name);
   return PMPI_Publish_name(service_name, info, port_name);
}


int MPI_Put ( void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win )
{
   INFO(0, "MPI_Put","(void *origin_addr=%p, int origin_count=%d, MPI_Datatype origin_datatype=%s, int target_rank=%d, MPI_Aint target_disp=%p, int target_count=%d, MPI_Datatype target_datatype=%s, MPI_Win win=%s)", origin_addr, origin_count, type_to_string(origin_datatype), target_rank, (void *) target_disp, target_count, type_to_string(target_datatype), win_to_string(win));
   return PMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win);
}


int MPI_Query_thread ( int *provided )
{
   INFO(0, "MPI_Query_thread","(int *provided=%p)", provided);
   return PMPI_Query_thread(provided);
}

int MPI_Recv ( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status )
{
   INFO(0, "MPI_Recv","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", buf, count, type_to_string(datatype), source, tag, comm_to_string(comm), status);
   return PMPI_Recv(buf, count, datatype, source, tag, comm, status);
}

int MPI_Recv_init ( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Recv_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int source=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), source, tag, comm_to_string(comm), request);
   return PMPI_Recv_init(buf, count, datatype, source, tag, comm, request);
}

int MPI_Reduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm )
{
   INFO(0, "MPI_Reduce","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, int root=%d, MPI_Comm comm=%s)",
             sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), root, comm_to_string(comm));
   return PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
}

int MPI_Reduce_scatter ( void *sendbuf, void *recvbuf, int *recvcnts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
   INFO(0, "MPI_Reduce_scatter","(void *sendbuf=%p, void *recvbuf=%p, int *recvcnts=%p, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, recvcnts, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
   return PMPI_Reduce_scatter(sendbuf, recvbuf, recvcnts, datatype, op, comm);
}


int MPI_Register_datarep ( char *name, MPI_Datarep_conversion_function *read_conv_fn, MPI_Datarep_conversion_function *write_conv_fn, MPI_Datarep_extent_function *extent_fn, void *state )
{
   INFO(0, "MPI_Register_datarep","(char *name=%p, MPI_Datarep_conversion_function *read_conv_fn=%p, MPI_Datarep_conversion_function *write_conv_fn=%p, MPI_Datarep_extent_function *extent_fn=%p, void *state=%p)", name, read_conv_fn, write_conv_fn, extent_fn, state);
   return PMPI_Register_datarep(name, read_conv_fn, write_conv_fn, extent_fn, state);
}

int MPI_Request_free ( MPI_Request *request )
{
   INFO(0, "MPI_Request_free","(MPI_Request *request=%p)", request);
   return PMPI_Request_free(request);
}

int MPI_Request_get_status ( MPI_Request request, int *flag, MPI_Status *status )
{
   INFO(0, "MPI_Request_get_status","(MPI_Request request=%s, int *flag=%p, MPI_Status *status=%p)", request_to_string(request), flag, status);
   return PMPI_Request_get_status(request, flag, status);
}

int MPI_Rsend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
   INFO(0, "MPI_Rsend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
   return PMPI_Rsend(buf, count, datatype, dest, tag, comm);
}

int MPI_Rsend_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Rsend_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);
}

int MPI_Scan ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
   INFO(0, "MPI_Scan","(void *sendbuf=%p, void *recvbuf=%p, int count=%d, MPI_Datatype datatype=%s, MPI_Op op=%s, MPI_Comm comm=%s)", sendbuf, recvbuf, count, type_to_string(datatype), op_to_string(op), comm_to_string(comm));
   return PMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);
}

int MPI_Scatter ( void *sendbuf, int sendcnt, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
   INFO(0, "MPI_Scatter","(void *sendbuf=%p, int sendcnt=%d, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcnt=%d, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)",
             sendbuf, sendcnt, type_to_string(sendtype), recvbuf, recvcnt, type_to_string(recvtype), root, comm_to_string(comm));
   return PMPI_Scatter(sendbuf, sendcnt, sendtype, recvbuf, recvcnt, recvtype, root, comm);
}

int MPI_Scatterv ( void *sendbuf, int *sendcnts, int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm )
{
   INFO(0, "MPI_Scatterv","(void *sendbuf=%p, int *sendcnts=%p, int *displs=%p, MPI_Datatype sendtype=%s, void *recvbuf=%p, int recvcnt=%d, MPI_Datatype recvtype=%s, int root=%d, MPI_Comm comm=%s)", 
             sendbuf, sendcnts, displs, type_to_string(sendtype), recvbuf, recvcnt, type_to_string(recvtype), root, comm_to_string(comm));
   return PMPI_Scatterv(sendbuf, sendcnts, displs, sendtype, recvbuf, recvcnt, recvtype, root, comm);
}

int MPI_Send ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
   INFO(0, "MPI_Send","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
   return PMPI_Send(buf, count, datatype, dest, tag, comm);
}

int MPI_Send_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Send_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Send_init(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Sendrecv ( void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status )
{
   INFO(0, "MPI_Sendrecv","(void *sendbuf=%p, int sendcount=%d, MPI_Datatype sendtype=%s, int dest=%d, int sendtag=%d, void *recvbuf=%p, int recvcount=%d, MPI_Datatype recvtype=%s, int source=%d, int recvtag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", sendbuf, sendcount, type_to_string(sendtype), dest, sendtag, recvbuf, recvcount, type_to_string(recvtype), source, recvtag, comm_to_string(comm), status);
   return PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
}


int MPI_Sendrecv_replace ( void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status )
{
   INFO(0, "MPI_Sendrecv_replace","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int sendtag=%d, int source=%d, int recvtag=%d, MPI_Comm comm=%s, MPI_Status *status=%p)", buf, count, type_to_string(datatype), dest, sendtag, source, recvtag, comm_to_string(comm), status);
   return PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);
}

int MPI_Ssend ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm )
{
   INFO(0, "MPI_Ssend","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm));
   return PMPI_Ssend(buf, count, datatype, dest, tag, comm);
}

int MPI_Ssend_init ( void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request )
{
   INFO(0, "MPI_Ssend_init","(void *buf=%p, int count=%d, MPI_Datatype datatype=%s, int dest=%d, int tag=%d, MPI_Comm comm=%s, MPI_Request *request=%p)", buf, count, type_to_string(datatype), dest, tag, comm_to_string(comm), request);
   return PMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);
}


int MPI_Startall ( int count, MPI_Request array_of_requests[] )
{
   INFO(0, "MPI_Startall","(int count=%d, MPI_Request array_of_requests[]=%p)", count, array_of_requests);
   return PMPI_Startall(count, array_of_requests);
}


int MPI_Start ( MPI_Request *request )
{
   INFO(0, "MPI_Start","(MPI_Request *request=%p)", request);
   return PMPI_Start(request);
}


int MPI_Status_set_cancelled ( MPI_Status *status, int flag )
{
   INFO(0, "MPI_Status_set_cancelled","(MPI_Status *status=%p, int flag=%d)", status, flag);
   return PMPI_Status_set_cancelled(status, flag);
}


int MPI_Status_set_elements ( MPI_Status *status, MPI_Datatype datatype, int count )
{
   INFO(0, "MPI_Status_set_elements","(MPI_Status *status=%p, MPI_Datatype datatype=%s, int count=%d)", status, type_to_string(datatype), count);
   return PMPI_Status_set_elements(status, datatype, count);
}


int MPI_Testall ( int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[] )
{
   INFO(0, "MPI_Testall","(int count=%d, MPI_Request array_of_requests[]=%p, int *flag=%p, MPI_Status array_of_statuses[]=%p)", count, array_of_requests, flag, array_of_statuses);
   return PMPI_Testall(count, array_of_requests, flag, array_of_statuses);
}


int MPI_Testany ( int count, MPI_Request array_of_requests[], int *index, int *flag, MPI_Status *status )
{
   INFO(0, "MPI_Testany","(int count=%d, MPI_Request array_of_requests[]=%p, int *index=%p, int *flag=%p, MPI_Status *status=%p)", count, array_of_requests, index, flag, status);
   return PMPI_Testany(count, array_of_requests, index, flag, status);
}


int MPI_Test_cancelled ( MPI_Status *status, int *flag )
{
   INFO(0, "MPI_Test_cancelled","(MPI_Status *status=%p, int *flag=%p)", status, flag);
   return PMPI_Test_cancelled(status, flag);
}

int MPI_Test ( MPI_Request *request, int *flag, MPI_Status *status )
{
   INFO(0, "MPI_Test","(MPI_Request *request=%p, int *flag=%p, MPI_Status *status=%p)", request, flag, status);
   return PMPI_Test(request, flag, status);
}

int MPI_Testsome ( int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[] )
{
   INFO(0, "MPI_Testsome","(int incount=%d, MPI_Request array_of_requests[]=%p, int *outcount=%p, int array_of_indices[]=%p, MPI_Status array_of_statuses[]=%p)", incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
   return PMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
}


int MPI_Topo_test ( MPI_Comm comm, int *topo_type )
{
   INFO(0, "MPI_Topo_test","(MPI_Comm comm=%s, int *topo_type=%p)", comm_to_string(comm), topo_type);
   return PMPI_Topo_test(comm, topo_type);
}


int MPI_Type_commit ( MPI_Datatype *datatype )
{
   INFO(0, "MPI_Type_commit","(MPI_Datatype *datatype=%p)", datatype);
   return PMPI_Type_commit(datatype);
}


int MPI_Type_contiguous ( int count, MPI_Datatype old_type, MPI_Datatype *new_type_p )
{
   INFO(0, "MPI_Type_contiguous","(int count=%d, MPI_Datatype old_type=%s, MPI_Datatype *new_type_p=%p)", count, type_to_string(old_type), new_type_p);
   return PMPI_Type_contiguous(count, old_type, new_type_p);
}


int MPI_Type_create_darray ( int size, int rank, int ndims, int array_of_gsizes[], int array_of_distribs[], int array_of_dargs[], int array_of_psizes[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_darray","(int size=%d, int rank=%d, int ndims=%d, int array_of_gsizes[]=%p, int array_of_distribs[]=%p, int array_of_dargs[]=%p, int array_of_psizes[]=%p, int order=%d, MPI_Datatype oldtype=%s, MPI_Datatype *newtype=%p)", size, rank, ndims, array_of_gsizes, array_of_distribs, array_of_dargs, array_of_psizes, order, type_to_string(oldtype), newtype);
   return PMPI_Type_create_darray(size, rank, ndims, array_of_gsizes, array_of_distribs, array_of_dargs, array_of_psizes, order, oldtype, newtype);
}

int MPI_Type_create_f90_complex ( int p, int r, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_f90_complex","(int p=%d, int r=%d, MPI_Datatype *newtype=%p)", p, r, newtype);
   return MPI_Type_create_f90_complex(p, r, newtype);
}

int MPI_Type_create_f90_integer ( int r, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_f90_integer","(r=%d, MPI_Datatype *newtype=%p)", r, newtype);
   return MPI_Type_create_f90_integer(r, newtype);
}

int MPI_Type_create_f90_real ( int p, int r, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_f90_real","(int p=%d, int r=%d, MPI_Datatype *newtype=%p)", p, r, newtype);
   return MPI_Type_create_f90_real(p, r, newtype);
}

int MPI_Type_create_hindexed ( int count, int blocklengths[], MPI_Aint displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_hindexed","(int count=%d, int blocklengths[]=%p, MPI_Aint displacements[]=%p, MPI_Datatype oldtype=%s, MPI_Datatype *newtype=%p)", count, blocklengths, displacements, type_to_string(oldtype), newtype);
   return PMPI_Type_create_hindexed(count, blocklengths, displacements, oldtype, newtype);
}


int MPI_Type_create_hvector ( int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_hvector","(int count=%d, int blocklength=%d, MPI_Aint stride=%p, MPI_Datatype oldtype=%s, MPI_Datatype *newtype=%p)", count, blocklength, (void *) stride, type_to_string(oldtype), newtype);
   return PMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype);
}


int MPI_Type_create_indexed_block ( int count, int blocklength, int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_indexed_block","(int count=%d, int blocklength=%d, int array_of_displacements[]=%p, MPI_Datatype oldtype=%s, MPI_Datatype *newtype=%p)", count, blocklength, array_of_displacements, type_to_string(oldtype), newtype);
   return PMPI_Type_create_indexed_block(count, blocklength, array_of_displacements, oldtype, newtype);
}


int MPI_Type_create_keyval ( MPI_Type_copy_attr_function *type_copy_attr_fn, MPI_Type_delete_attr_function *type_delete_attr_fn, int *type_keyval, void *extra_state )
{
   INFO(0, "MPI_Type_create_keyval","(MPI_Type_copy_attr_function *type_copy_attr_fn=%p, MPI_Type_delete_attr_function *type_delete_attr_fn=%p, int *type_keyval=%p, void *extra_state=%p)", type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state);
   return PMPI_Type_create_keyval(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state);
}


int MPI_Type_create_resized ( MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_resized","(MPI_Datatype oldtype=%s, MPI_Aint lb=%p, MPI_Aint extent=%p, MPI_Datatype *newtype=%p)", type_to_string(oldtype), (void *) lb, (void *) extent, newtype);
   return PMPI_Type_create_resized(oldtype, lb, extent, newtype);
}


int MPI_Type_create_struct ( int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype array_of_types[], MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_struct","(int count=%d, int array_of_blocklengths[]=%p, MPI_Aint array_of_displacements[]=%p, MPI_Datatype array_of_types[]=%p, MPI_Datatype *newtype=%p)", count, array_of_blocklengths, array_of_displacements, array_of_types, newtype);
   return PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, newtype);
}


int MPI_Type_create_subarray ( int ndims, int array_of_sizes[], int array_of_subsizes[], int array_of_starts[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_create_subarray","(int ndims=%d, int array_of_sizes[]=%p, int array_of_subsizes[]=%p, int array_of_starts[]=%p, int order=%d, MPI_Datatype oldtype=%s, MPI_Datatype *newtype=%p)", ndims, array_of_sizes, array_of_subsizes, array_of_starts, order, type_to_string(oldtype), newtype);
   return PMPI_Type_create_subarray(ndims, array_of_sizes, array_of_subsizes, array_of_starts, order, oldtype, newtype);
}


int MPI_Type_delete_attr ( MPI_Datatype type, int type_keyval )
{
   INFO(0, "MPI_Type_delete_attr","(MPI_Datatype type=%s, int type_keyval=%d)", type_to_string(type), type_keyval);
   return PMPI_Type_delete_attr(type, type_keyval);
}


int MPI_Type_dup ( MPI_Datatype datatype, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_dup","(MPI_Datatype datatype=%s, MPI_Datatype *newtype=%p)", type_to_string(datatype), newtype);
   return PMPI_Type_dup(datatype, newtype);
}


int MPI_Type_extent ( MPI_Datatype datatype, MPI_Aint *extent )
{
   INFO(0, "MPI_Type_extent","(MPI_Datatype datatype=%s, MPI_Aint *extent=%p)", type_to_string(datatype), extent);
   return PMPI_Type_extent(datatype, extent);
}


int MPI_Type_free ( MPI_Datatype *datatype )
{
   INFO(0, "MPI_Type_free","(MPI_Datatype *datatype=%p)", datatype);
   return PMPI_Type_free(datatype);
}


int MPI_Type_free_keyval ( int *type_keyval )
{
   INFO(0, "MPI_Type_free_keyval","(int *type_keyval=%p)", type_keyval);
   return PMPI_Type_free_keyval(type_keyval);
}


int MPI_Type_get_attr ( MPI_Datatype type, int type_keyval, void *attribute_val, int *flag )
{
   INFO(0, "MPI_Type_get_attr","(MPI_Datatype type=%s, int type_keyval=%d, void *attribute_val=%p, int *flag=%p)", type_to_string(type), type_keyval, attribute_val, flag);
   return PMPI_Type_get_attr(type, type_keyval, attribute_val, flag);
}


int MPI_Type_get_contents ( MPI_Datatype datatype, int max_integers, int max_addresses, int max_datatypes, int array_of_integers[], MPI_Aint array_of_addresses[], MPI_Datatype array_of_datatypes[] )
{
   INFO(0, "MPI_Type_get_contents","(MPI_Datatype datatype=%s, int max_integers=%d, int max_addresses=%d, int max_datatypes=%d, int array_of_integers[]=%p, MPI_Aint array_of_addresses[]=%p, MPI_Datatype array_of_datatypes[]=%p)", type_to_string(datatype), max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes);
   return PMPI_Type_get_contents(datatype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes);
}


int MPI_Type_get_envelope ( MPI_Datatype datatype, int *num_integers, int *num_addresses, int *num_datatypes, int *combiner )
{
   INFO(0, "MPI_Type_get_envelope","(MPI_Datatype datatype=%s, int *num_integers=%p, int *num_addresses=%p, int *num_datatypes=%p, int *combiner=%p)", type_to_string(datatype), num_integers, num_addresses, num_datatypes, combiner);
   return PMPI_Type_get_envelope(datatype, num_integers, num_addresses, num_datatypes, combiner);
}


int MPI_Type_get_extent ( MPI_Datatype datatype, MPI_Aint *lb, MPI_Aint *extent )
{
   INFO(0, "MPI_Type_get_extent","(MPI_Datatype datatype=%s, MPI_Aint *lb=%p, MPI_Aint *extent=%p)", type_to_string(datatype), lb, extent);
   return PMPI_Type_get_extent(datatype, lb, extent);
}


int MPI_Type_get_name ( MPI_Datatype datatype, char *type_name, int *resultlen )
{
   INFO(0, "MPI_Type_get_name","(MPI_Datatype datatype=%s, char *type_name=%p, int *resultlen=%p)", type_to_string(datatype), type_name, resultlen);
   return PMPI_Type_get_name(datatype, type_name, resultlen);
}


int MPI_Type_get_true_extent ( MPI_Datatype datatype, MPI_Aint *true_lb, MPI_Aint *true_extent )
{
   INFO(0, "MPI_Type_get_true_extent","(MPI_Datatype datatype=%s, MPI_Aint *true_lb=%p, MPI_Aint *true_extent=%p)", type_to_string(datatype), true_lb, true_extent);
   return PMPI_Type_get_true_extent(datatype, true_lb, true_extent);
}


int MPI_Type_hindexed ( int count, int blocklens[], MPI_Aint indices[], MPI_Datatype old_type, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_hindexed","(int count=%d, int blocklens[]=%p, MPI_Aint indices[]=%p, MPI_Datatype old_type=%s, MPI_Datatype *newtype=%p)", count, blocklens, indices, type_to_string(old_type), newtype);
   return PMPI_Type_hindexed(count, blocklens, indices, old_type, newtype);
}


int MPI_Type_hvector ( int count, int blocklen, MPI_Aint stride, MPI_Datatype old_type, MPI_Datatype *newtype_p )
{
   INFO(0, "MPI_Type_hvector","(int count=%d, int blocklen=%d, MPI_Aint stride=%p, MPI_Datatype old_type=%s, MPI_Datatype *newtype_p=%p)", count, blocklen, (void *) stride, type_to_string(old_type), newtype_p);
   return PMPI_Type_hvector(count, blocklen, stride, old_type, newtype_p);
}


int MPI_Type_indexed ( int count, int blocklens[], int indices[], MPI_Datatype old_type, MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_indexed","(int count=%d, int blocklens[]=%p, int indices[]=%p, MPI_Datatype old_type=%s, MPI_Datatype *newtype=%p)", count, blocklens, indices, type_to_string(old_type), newtype);
   return PMPI_Type_indexed(count, blocklens, indices, old_type, newtype);
}


int MPI_Type_lb ( MPI_Datatype datatype, MPI_Aint *displacement )
{
   INFO(0, "MPI_Type_lb","(MPI_Datatype datatype=%s, MPI_Aint *displacement=%p)", type_to_string(datatype), displacement);
   return PMPI_Type_lb(datatype, displacement);
}


int MPI_Type_match_size ( int typeclass, int size, MPI_Datatype *datatype )
{
   INFO(0, "MPI_Type_match_size","(int typeclass=%d, int size=%d, MPI_Datatype *datatype=%p)", typeclass, size, datatype);
   return PMPI_Type_match_size(typeclass, size, datatype);
}


int MPI_Type_set_attr ( MPI_Datatype type, int type_keyval, void *attribute_val )
{
   INFO(0, "MPI_Type_set_attr","(MPI_Datatype type=%s, int type_keyval=%d, void *attribute_val=%p)", type_to_string(type), type_keyval, attribute_val);
   return PMPI_Type_set_attr(type, type_keyval, attribute_val);
}


int MPI_Type_set_name ( MPI_Datatype type, char *type_name )
{
   INFO(0, "MPI_Type_set_name","(MPI_Datatype type=%s, char *type_name=%p)", type_to_string(type), type_name);
   return PMPI_Type_set_name(type, type_name);
}


int MPI_Type_size ( MPI_Datatype datatype, int *size )
{
   INFO(0, "MPI_Type_size","(MPI_Datatype datatype=%s, int *size=%p)", type_to_string(datatype), size);
   return PMPI_Type_size(datatype, size);
}


int MPI_Type_struct ( int count, int blocklens[], MPI_Aint indices[], MPI_Datatype old_types[], MPI_Datatype *newtype )
{
   INFO(0, "MPI_Type_struct","(int count=%d, int blocklens[]=%p, MPI_Aint indices[]=%p, MPI_Datatype old_types[]=%p, MPI_Datatype *newtype=%p)", count, blocklens, indices, old_types, newtype);
   return PMPI_Type_struct(count, blocklens, indices, old_types, newtype);
}


int MPI_Type_ub ( MPI_Datatype datatype, MPI_Aint *displacement )
{
   INFO(0, "MPI_Type_ub","(MPI_Datatype datatype=%s, MPI_Aint *displacement=%p)", type_to_string(datatype), displacement);
   return PMPI_Type_ub(datatype, displacement);
}


int MPI_Type_vector ( int count, int blocklength, int stride, MPI_Datatype old_type, MPI_Datatype *newtype_p )
{
   INFO(0, "MPI_Type_vector","(int count=%d, int blocklength=%d, int stride=%d, MPI_Datatype old_type=%s, MPI_Datatype *newtype_p=%p)", count, blocklength, stride, type_to_string(old_type), newtype_p);
   return PMPI_Type_vector(count, blocklength, stride, old_type, newtype_p);
}


int MPI_Unpack_external ( char *datarep, void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype )
{
   INFO(0, "MPI_Unpack_external","(char *datarep=%p, void *inbuf=%p, MPI_Aint insize=%p, MPI_Aint *position=%p, void *outbuf=%p, int outcount=%d, MPI_Datatype datatype=%s)", datarep, inbuf, (void *) insize, position, outbuf, outcount, type_to_string(datatype));
   return PMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype);
}


int MPI_Unpack ( void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm )
{
   INFO(0, "MPI_Unpack","(void *inbuf=%p, int insize=%d, int *position=%p, void *outbuf=%p, int outcount=%d, MPI_Datatype datatype=%s, MPI_Comm comm=%s)", inbuf, insize, position, outbuf, outcount, type_to_string(datatype), comm_to_string(comm));
   return PMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm);
}


int MPI_Unpublish_name ( char *service_name, MPI_Info info, char *port_name )
{
   INFO(0, "MPI_Unpublish_name","(char *service_name=%p, MPI_Info info=%s, char *port_name=%p)", service_name, info_to_string(info), port_name);
   return PMPI_Unpublish_name(service_name, info, port_name);
}

int MPI_Waitall ( int count, MPI_Request array_of_requests[], MPI_Status array_of_statuses[] )
{
   INFO(0, "MPI_Waitall","(int count=%d, MPI_Request array_of_requests[]=%p, MPI_Status array_of_statuses[]=%p)", count, array_of_requests, array_of_statuses);
   return PMPI_Waitall(count, array_of_requests, array_of_statuses);
}

int MPI_Waitany ( int count, MPI_Request array_of_requests[], int *index, MPI_Status *status )
{
   INFO(0, "MPI_Waitany","(int count=%d, MPI_Request array_of_requests[]=%p, int *index=%p, MPI_Status *status=%p)", count, array_of_requests, index, status);
   return PMPI_Waitany(count, array_of_requests, index, status);
}

int MPI_Wait ( MPI_Request *request, MPI_Status *status )
{
   INFO(0, "MPI_Wait","(MPI_Request *request=%p, MPI_Status *status=%p)", request, status);
   return PMPI_Wait(request, status);
}

int MPI_Waitsome ( int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[] )
{
   INFO(0, "MPI_Waitsome","(int incount=%d, MPI_Request array_of_requests[]=%p, int *outcount=%p, int array_of_indices[]=%p, MPI_Status array_of_statuses[]=%p)", incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
   return PMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
}


int MPI_Win_call_errhandler ( MPI_Win win, int errorcode )
{
   INFO(0, "MPI_Win_call_errhandler","(MPI_Win win=%s, int errorcode=%d)", win_to_string(win), errorcode);
   return PMPI_Win_call_errhandler(win, errorcode);
}


int MPI_Win_complete ( MPI_Win win )
{
   INFO(0, "MPI_Win_complete","(MPI_Win win=%s)", win_to_string(win));
   return PMPI_Win_complete(win);
}


int MPI_Win_create_errhandler ( MPI_Win_errhandler_fn *function, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Win_create_errhandler","(MPI_Win_errhandler_fn *function=%p, MPI_Errhandler *errhandler=%p)", function, errhandler);
   return PMPI_Win_create_errhandler(function, errhandler);
}


int MPI_Win_create ( void *base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, MPI_Win *win )
{
   INFO(0, "MPI_Win_create","(void *base=%p, MPI_Aint size=%p, int disp_unit=%d, MPI_Info info=%s, MPI_Comm comm=%s, MPI_Win *win=%p)", base, (void *) size, disp_unit, info_to_string(info), comm_to_string(comm), win);
  return PMPI_Win_create(base, size, disp_unit, info, comm, win);
}


int MPI_Win_create_keyval ( MPI_Win_copy_attr_function *win_copy_attr_fn, MPI_Win_delete_attr_function *win_delete_attr_fn, int *win_keyval, void *extra_state )
{
   INFO(0, "MPI_Win_create_keyval","(MPI_Win_copy_attr_function *win_copy_attr_fn=%p, MPI_Win_delete_attr_function *win_delete_attr_fn=%p, int *win_keyval=%p, void *extra_state=%p)", win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state);
   return PMPI_Win_create_keyval(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state);
}


int MPI_Win_delete_attr ( MPI_Win win, int win_keyval )
{
   INFO(0, "MPI_Win_delete_attr","(MPI_Win win=%s, int win_keyval=%d)", win_to_string(win), win_keyval);
   return PMPI_Win_delete_attr(win, win_keyval);
}


int MPI_Win_fence ( int assert, MPI_Win win )
{
   INFO(0, "MPI_Win_fence","(int assert=%d, MPI_Win win=%s)", assert, win_to_string(win));
   return PMPI_Win_fence(assert, win);
}


int MPI_Win_free ( MPI_Win *win )
{
   INFO(0, "MPI_Win_free","(MPI_Win *win=%p)", win);
   return PMPI_Win_free(win);
}


int MPI_Win_free_keyval ( int *win_keyval )
{
   INFO(0, "MPI_Win_free_keyval","(int *win_keyval=%p)", win_keyval);
   return PMPI_Win_free_keyval(win_keyval);
}


int MPI_Win_get_attr ( MPI_Win win, int win_keyval, void *attribute_val, int *flag )
{
   INFO(0, "MPI_Win_get_attr","(MPI_Win win=%s, int win_keyval=%d, void *attribute_val=%p, int *flag=%p)", win_to_string(win), win_keyval, attribute_val, flag);
   return PMPI_Win_get_attr(win, win_keyval, attribute_val, flag);
}


int MPI_Win_get_errhandler ( MPI_Win win, MPI_Errhandler *errhandler )
{
   INFO(0, "MPI_Win_get_errhandler","(MPI_Win win=%s, MPI_Errhandler *errhandler=%p)", win_to_string(win), errhandler);
   return PMPI_Win_get_errhandler(win, errhandler);
}


int MPI_Win_get_group ( MPI_Win win, MPI_Group *group )
{
   INFO(0, "MPI_Win_get_group","(MPI_Win win=%s, MPI_Group *group=%p)", win_to_string(win), group);
   return PMPI_Win_get_group(win, group);
}


int MPI_Win_get_name ( MPI_Win win, char *win_name, int *resultlen )
{
   INFO(0, "MPI_Win_get_name","(MPI_Win win=%s, char *win_name=%p, int *resultlen=%p)", win_to_string(win), win_name, resultlen);
   return PMPI_Win_get_name(win, win_name, resultlen);
}


int MPI_Win_lock ( int lock_type, int rank, int assert, MPI_Win win )
{
   INFO(0, "MPI_Win_lock","(int lock_type=%d, int rank=%d, int assert=%d, MPI_Win win=%s)", lock_type, rank, assert, win_to_string(win));
   return PMPI_Win_lock(lock_type, rank, assert, win);
}


int MPI_Win_post ( MPI_Group group, int assert, MPI_Win win )
{
   INFO(0, "MPI_Win_post","(MPI_Group group=%s, int assert=%d, MPI_Win win=%s)", group_to_string(group), assert, win_to_string(win));
   return PMPI_Win_post(group, assert, win);
}


int MPI_Win_set_attr ( MPI_Win win, int win_keyval, void *attribute_val )
{
   INFO(0, "MPI_Win_set_attr","(MPI_Win win=%s, int win_keyval=%d, void *attribute_val=%p)", win_to_string(win), win_keyval, attribute_val);
   return PMPI_Win_set_attr(win, win_keyval, attribute_val);
}


int MPI_Win_set_errhandler ( MPI_Win win, MPI_Errhandler errhandler )
{
   INFO(0, "MPI_Win_set_errhandler","(MPI_Win win=%s, MPI_Errhandler errhandler=%p)", win_to_string(win), (void *) errhandler);
   return PMPI_Win_set_errhandler(win, errhandler);
}


int MPI_Win_set_name ( MPI_Win win, char *win_name )
{
   INFO(0, "MPI_Win_set_name","(MPI_Win win=%s, char *win_name=%p)", win_to_string(win), win_name);
   return PMPI_Win_set_name(win, win_name);
}


int MPI_Win_start ( MPI_Group group, int assert, MPI_Win win )
{
   INFO(0, "MPI_Win_start","(MPI_Group group=%s, int assert=%d, MPI_Win win=%s)", group_to_string(group), assert, win_to_string(win));
   return PMPI_Win_start(group, assert, win);
}


int MPI_Win_test ( MPI_Win win, int *flag )
{
   INFO(0, "MPI_Win_test","(MPI_Win win=%s, int *flag=%p)", win_to_string(win), flag);
   return PMPI_Win_test(win, flag);
}


int MPI_Win_unlock ( int rank, MPI_Win win )
{
   INFO(0, "MPI_Win_unlock","(int rank=%d, MPI_Win win=%s)", rank, win_to_string(win));
   return PMPI_Win_unlock(rank, win);
}


int MPI_Win_wait ( MPI_Win win )
{
   INFO(0, "MPI_Win_wait","(MPI_Win win=%s)", win_to_string(win));
   return PMPI_Win_wait(win);
}


double MPI_Wtick (  )
{
   INFO(0, "MPI_Wtick","()");
   return PMPI_Wtick();
}

double MPI_Wtime (  )
{
   INFO(0, "MPI_Wtime","()");
   return PMPI_Wtime();
}

/* not reported

MPI_Comm MPI_Comm_f2c ( MPI_Fint comm )
{
   return PMPI_Comm_f2c(comm);
}

MPI_Fint MPI_Comm_c2f ( MPI_Comm comm )
{
   return PMPI_Comm_c2f(comm);
}

MPI_Group MPI_Group_f2c ( MPI_Fint g )
{
   return PMPI_Group_f2c(g);
}

MPI_Fint MPI_Group_c2f ( MPI_Group g )
{
   return PMPI_Group_c2f(g);
}

MPI_Info MPI_Info_f2c ( MPI_Fint info )
{
   return PMPI_Info_f2c(info);
}

MPI_Fint MPI_Info_c2f ( MPI_Info info )
{
   return PMPI_Info_c2f(info);
}

MPI_Op MPI_Op_f2c ( MPI_Fint op )
{
   return PMPI_Op_f2c(op);
}

MPI_Fint MPI_Op_c2f ( MPI_Op op )
{
   return PMPI_Op_c2f(op);
}

MPI_Request MPI_Request_f2c ( MPI_Fint req )
{
   return PMPI_Request_f2c(req);
}

MPI_Fint MPI_Request_c2f ( MPI_Request req )
{
   return PMPI_Request_c2f(req);
}

MPI_Datatype MPI_Type_f2c ( MPI_Fint datatype )
{
   return PMPI_Type_f2c(datatype);
}

MPI_Fint MPI_Type_c2f ( MPI_Datatype datatype )
{
   return PMPI_Type_c2f(datatype);
}

MPI_Win MPI_Win_f2c ( MPI_Fint win )
{
   return PMPI_Win_f2c(win);
}

MPI_Fint MPI_Win_c2f ( MPI_Win win )
{
   return PMPI_Win_c2f(win);
}

MPI_Status MPI_Status_f2c ( MPI_Fint s )
{
   return PMPI_Status_f2c(s);
}

MPI_Fint MPI_Status_c2f ( MPI_Status s )
{
   return PMPI_Status_c2f(s);
}

MPI_Errhandler MPI_Errhandler_f2c ( MPI_Fint e )
{
   return MPI_Errhandler_f2c(e);
}

MPI_Fint MPI_Errhandler_c2f ( MPI_Errhandler e )
{
   return PMPI_Errhandler_c2f(e);
}

MPI_Fint MPI_File_c2f ( MPI_File mpi_fh )
{
   return PMPI_File_c2f(mpi_fh);
}

MPI_File MPI_File_f2c ( MPI_Fint fh )
{
   return PMPI_File_f2c(fh);
}

*/
