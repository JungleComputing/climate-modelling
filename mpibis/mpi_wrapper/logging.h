#ifndef _DEBUGGING_H_
#define _DEBUGGING_H_

#include "mpi.h"

// VERBOSE must be defined and >= 0. Usual values are:
//
// 0 only print fatal
// 1 print errors and fatal
// 2 print errors and warnings
// 3 print errors, warnings and info
// 4+ print all

#define VERBOSE 1

// Crash application when an error is printed ?
#define ERRORS_ARE_FATAL 1

// Print stacktrace when application is crashed ? (0=no, >0 is length of trace)
#define PRINT_STACK_TRACE 10

// Define several macros to print errors

#define FATAL(indent, error, message, ...) XERROR(indent, error, 1, "FATAL", __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)

#if VERBOSE > 0
#define ERROR(indent, error, message, ...) XERROR(indent, error, ERRORS_ARE_FATAL, "ERROR", __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)
#define IERROR(indent, error, message, ...) XERROR(indent, error, 1, "INTERNAL ERROR", __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define ERROR(indent, error, message, ...)
#define IERROR(indent, error, message, ...)
#endif


// Define macro to print warning
#if VERBOSE > 1
#define WARN(indent, message, ...) XLOG(indent, 1, "WARN", __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define WARN(indent, message, ...)
#endif


// Define macro to print info
#if VERBOSE > 2
#define INFO(indent, message, ...) XLOG(indent, 0, "INFO", __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define INFO(indent, message, ...)
#endif


// Define macro to print debug
#if VERBOSE > 3
#define DEBUG(indent, message, ...) XLOG(indent, 1, "DEBUG", __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define DEBUG(indent, message, ...)
#endif

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

// Should be replaced!!!
//void DEBUG(int indent, const char *fmt, ...);
//void INFO(int indent, const char *func, const char *fmt, ...);
//void ERROR(int indent, const char *fmt, ...);
//void IERROR(int indent, const char *fmt, ...);
//void WARN(int indent, const char *fmt, ...);

// New format...
int XERROR(int indent, int errorcode, const char *header, const char *func, const char *file, const int line, const char *fmt, ...);
int XLOG(int indent, int verbose, const char *header, const char *func, const char *file, const int line, const char *fmt, ...);

#endif

