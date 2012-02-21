#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "flags.h"
#include "types.h"

#include "impi.h"
#include "mpi.h"


struct s_operation {
     int index; 
     int commute;
     MPI_Op op;
     MPI_User_function *function;
};

int init_operations();

operation *get_operation(IMPI_Op op);
operation *get_operation_with_index(int index);

void set_operation_ptr(IMPI_Op *dst, operation *src);

/* 

NOT SUPPORTED 

operation *create_operation(MPI_User_function *function, int commute);
void free_operation(operation *op);

*/

#endif // _OPERATION_H_
