#ifndef __PROFILING_H_
#define __PROFILING_H_

#include <stdint.h>
#include "mpi.h"

#define STATS_BARRIER   0
#define STATS_SEND      1
#define STATS_RECV      2
#define STATS_ISEND     3
#define STATS_IRECV     4
#define STATS_BCAST     5
#define STATS_SCATTER   6
#define STATS_GATHER    7
#define STATS_ALLGATHER 8
#define STATS_ALLTOALL  9
#define STATS_REDUCE    10
#define STATS_ALLREDUCE 11
#define STATS_SCAN      12
#define STATS_MISC      13
#define STATS_TOTAL     14

#define STATS_NAME_BARRIER   "barrier"
#define STATS_NAME_SEND      "send"
#define STATS_NAME_RECV      "receive"
#define STATS_NAME_ISEND     "isend"
#define STATS_NAME_IRECV     "ireceive"
#define STATS_NAME_BCAST     "bcast"
#define STATS_NAME_SCATTER   "scatter"
#define STATS_NAME_GATHER    "gather"
#define STATS_NAME_ALLGATHER "allgather"
#define STATS_NAME_ALLTOALL  "alltoall"
#define STATS_NAME_REDUCE    "reduce"
#define STATS_NAME_ALLREDUCE "allreduce"
#define STATS_NAME_SCAN      "scan"
#define STATS_NAME_TOTAL     "total"


static __inline__ uint64_t profile_start_ticks (void) {
  uint32_t cycles_low, cycles_high;
  asm volatile ("CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
		"%rax", "%rbx", "%rcx", "%rdx");
  return ((uint64_t)cycles_high << 32) | cycles_low;
}

static __inline__ uint64_t profile_stop_ticks (void) {
  uint32_t cycles_low, cycles_high;
  asm volatile("RDTSCP\n\t"
	       "mov %%edx, %0\n\t"
	       "mov %%eax, %1\n\t"
	       "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax",
	       "%rbx", "%rcx", "%rdx");

  return ((uint64_t)cycles_high << 32) | cycles_low;
}

void dump_profile_info();

void profile_init();
void profile_finalize();

void profile_add_statistics(MPI_Comm comm, int field, uint64_t ticks);
void profile_print_statistics(MPI_Comm comm);
void profile_print_all_statistics();

#endif
