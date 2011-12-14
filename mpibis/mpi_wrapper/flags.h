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

// These constants determine various maxima used in the MPIbis library.

// Maximum number of MPI processes allowed.
#define MAX_PROCESSES_PER_CLUSTER 1024 

// Maximum number of clusters allowed.
#define MAX_CLUSTERS 64

// Maximum PID size required
#define MAX_PID_SIZE (MAX_CLUSTERS*MAX_PROCESSES_PER_CLUSTER)

// Maximum number of communicators that can be created (in total, shared by all processes).
#define MAX_COMMUNICATORS 256

// Maximum number of groups that can be created (at once, for each individual process).
#define MAX_GROUPS 1024

// Maximum number of requests that can be created (at once, for each individual process).
#define MAX_REQUEST 1024

#endif
