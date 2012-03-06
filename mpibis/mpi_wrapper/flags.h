#ifndef __FLAGS_H_
#define __FLAGS_H_

// These flags determine how much of the MPIbis library will be used.
// Switch features off by commenting a define.

#define ENABLE_INTERCEPT

// Print every call ?
#define TRACE_CALLS

// Intercept every call and forward it to MPIbis ?
#define IBIS_INTERCEPT

// Print error and terminate when an MPI_ERRORS is caught ?
#define TRACE_ERRORS

// Print error when a derived datatype is send. This is not supported yet.
#define CATCH_DERIVED_TYPES

// These constants determine various maxima used in the MPIbis library.

// Maximum number of MPI processes allowed (2^24).
#define MAX_PROCESSES_PER_CLUSTER 16777216

// Maximum number of clusters allowed (2^8).
#define MAX_CLUSTERS 256

// Macro to filter out cluster ID.
#define GET_CLUSTER_RANK(X) ((X & 0xFF000000) >> 24)

// Macro to filter out process ID.
#define GET_PROCESS_RANK(X) (X & 0x00FFFFFF)

// Macro to create process ID.
#define SET_PID(X, Y) ((X & 0xFF) << 24 | (Y & 0xFFFFFF))

// Macro to catch derived datatypes.
#define CHECK_TYPE(T) (catch_derived_datatype(T))

// Macro to print error
#define SERROR(indent, error, message, ...) XERROR(indent, error, __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)

// Macro to print internal error
#define SIERROR(indent, error, message, ...) XIERROR(indent, error, __FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)

// Macro to translate MPI communicator to MPIbis communicator
#define TRANSLATE_COMMUNICATOR(M, I) { \
   I = get_communicator(M); \
   if (I == NULL) { \
      return SERROR(1, MPI_ERR_COMM, "Communicator not found!");\
   } \
}

// Macro to check buffer count
#define CHECK_COUNT(C) { \
   if (C < 0) { \
      return SERROR(1, MPI_ERR_COUNT, "Invalid count! (%d)", C); \
   } \
}

// Macro to check rank
#define CHECK_RANK(R) { \
   if (R < 0 || R > c->global_size) { \
      return SERROR(1, MPI_ERR_RANK, "Invalid rank! (%d)", R);  \
   } \
}

// Maximum number of communicators that can be created (in total, shared by all processes).
#define MAX_COMMUNICATORS 1024

// Maximum number of groups that can be created (at once, for each individual process).
#define MAX_GROUPS 1024

// Maximum number of requests that can be created (at once, for each individual process).
#define MAX_REQUESTS 1024

// Maximum number of operations that can be created (at once, for each individual process).
#define MAX_OPERATIONS 256

#endif
