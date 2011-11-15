#ifndef _DEBUGGING_H_
#define _DEBUGGING_H_

#include "mpi.h"

// VERBOSE must be defined and >= 0. Usual values are:
//
// 0 no prints
// 1 only print errors
// 2 print errors and warnings
// 3 print errors, warnings and info
// 4+ print all

#define VERBOSE 4

void init_debug();

char *comm_to_string(MPI_Comm comm);
char *type_to_string(MPI_Datatype type);
char *request_to_string(MPI_Request r);
char *op_to_string(MPI_Op o);
char *info_to_string(MPI_Info i);
char *file_to_string(MPI_File f);

char *win_to_string(MPI_Win w);
char *ranks_to_string(int *ranks, int n);
char *ranges_to_string(int range[][3], int n);
char *group_to_string(MPI_Group g);

void DEBUG(int indent, const char *fmt, ...);
void INFO(int indent, const char *func, const char *fmt, ...);
void ERROR(int indent, const char *fmt, ...);
void IERROR(int indent, const char *fmt, ...);
void WARN(int indent, const char *fmt, ...);

#endif


