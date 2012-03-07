#ifndef _DEBUGGING_H_
#define _DEBUGGING_H_

#include "flags.h"
#include "communicator.h"

// Macro to catch derived datatypes.
#ifdef CATCH_DERIVED_TYPES
#define CHECK_TYPE(T) (catch_derived_datatype(T))
#else
#define CHECK_TYPE(T)
#endif

// Macros to check parameters
#ifdef CHECK_PARAMETERS
#define CHECK_COUNT(C)  (check_count(C))
#define CHECK_RANK(C,R) (check_rank(C, R))
#else
#define CHECK_COUNT(C)
#define CHECK_RANK(C,R)
#endif

// Function definitions
void catch_derived_datatype(MPI_Datatype type);
void check_count(int count);
void check_rank(communicator *c, int rank);

#endif


