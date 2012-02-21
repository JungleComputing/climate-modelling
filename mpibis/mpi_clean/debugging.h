#ifndef _DEBUGGING_H_
#define _DEBUGGING_H_

#include "impi.h"

// VERBOSE must be defined and >= 0. Usual values are:
//
// 0 no prints
// 1 only print errors
// 2 print errors and warnings
// 3 print errors, warnings and info
// 4+ print all

#define VERBOSE 1

void init_debug();

char *comm_to_string(IMPI_Comm comm);
char *type_to_string(IMPI_Datatype type);
char *request_to_string(IMPI_Request r);
char *op_to_string(IMPI_Op o);

char *ranks_to_string(int *ranks, int n);
char *ranges_to_string(int range[][3], int n);
char *group_to_string(IMPI_Group g);

void DEBUG(int indent, const char *fmt, ...);
void INFO(int indent, const char *func, const char *fmt, ...);
void ERROR(int indent, const char *fmt, ...);
void IERROR(int indent, const char *fmt, ...);
void WARN(int indent, const char *fmt, ...);

#endif


