#ifndef _UTIL_H_
#define _UTIL_H_

#include "mpi.h"

#define DELETE_ARG { for (cnt=i; cnt<(*argc)-1; cnt++){ \
                       (*argv)[cnt] = (*argv)[cnt+1]; \
                     }\
                     changed = 1;\
                     (*argc)--;\
                   }

void init_fortran_logical_(int *, int*);

void init_convert();

int convert_error_MPI_to_IMPI(int error);
MPI_Datatype convert_type_IMPI_to_MPI(int type);
int convert_rank_IMPI_to_MPI(int rank);
int convert_tag_IMPI_to_MPI(int tag);

#endif

