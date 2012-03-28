#include "flags.h"

#ifdef IBIS_INTERCEPT

#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#include "generated_header.h"
#include "logging.h"
#include "messaging.h"
#include "profiling.h"

static char *statistic_names[STATS_TOTAL+1] = {
   STATS_NAME_BARRIER,
   STATS_NAME_SEND,
   STATS_NAME_RECV,
   STATS_NAME_ISEND,
   STATS_NAME_IRECV,
   STATS_NAME_BCAST,
   STATS_NAME_SCATTER,
   STATS_NAME_GATHER,
   STATS_NAME_ALLGATHER,
   STATS_NAME_ALLTOALL,
   STATS_NAME_REDUCE,
   STATS_NAME_ALLREDUCE,
   STATS_NAME_SCAN,
   STATS_NAME_TOTAL
};

static uint32_t *total_use[MAX_COMMUNICATORS];
static uint64_t *total_ticks[MAX_COMMUNICATORS];

// Profiling timers
uint64_t start_ticks;
uint64_t end_ticks;

void profile_init()
{
   int i;

   for (i=0;i<MAX_COMMUNICATORS;i++) {
      total_use[i] = NULL;
      total_ticks[i] = NULL;
   }

   start_ticks = profile_start_ticks();

   INFO(1, "Profiling initialized! (%d)", MAX_COMMUNICATORS);
}

void profile_finalize()
{
   end_ticks = profile_start_ticks();

   profile_print_all_statistics();

   printf("Total profiled ticks: %ld\n", end_ticks-start_ticks);

   INFO(1, "Profiling done!");
}


void profile_add_statistics(MPI_Comm comm, int field, uint64_t ticks)
{
   int i, index;

   index = MPI_Comm_c2f(comm);

   if (index < 0 || index >= MAX_COMMUNICATORS) {
      WARN(1, "Communicator index out of bounds: %d", index);
      return;
   }

   if (field < 0 || field >= STATS_TOTAL) {
      WARN(1, "Statistics index out of bounds: %d (comm=%d)", field, index);
      return;
   }

   if (total_ticks[index] == NULL) {

      total_ticks[index] = malloc((STATS_TOTAL+1) * sizeof(uint64_t));

      if (total_ticks[index] == NULL) {
         ERROR(1, "Failed to allocate profiling buffer for communicator %d (1)", index);
         return;
      }

      total_use[index] = malloc((STATS_TOTAL+1) * sizeof(uint32_t));

      if (total_use[index] == NULL) {
         ERROR(1, "Failed to allocate profiling buffer for communicator %d (2)", index);
         return;
      }

      for (i=0;i<(STATS_TOTAL+1);i++) {
         total_ticks[index][i] = 0;
         total_use[index][i] = 0;
      }
   }

   total_ticks[index][field] += ticks;
   total_ticks[index][STATS_TOTAL] += ticks;

   total_use[index][field]++;
   total_use[index][STATS_TOTAL]++;
}

void profile_print_statistics(MPI_Comm comm)
{
   int i, index;

   index = MPI_Comm_c2f(comm);

   if (index < 0 || index >= MAX_COMMUNICATORS) {
      WARN(1, "Communicator index out of bounds: %d", index);
      return;
   }

   if (total_ticks[index] == NULL) {
      printf("No statistics available for communicator %d\n", index);
      return;
   }

   printf("Statistics for communicator %d: ", index);

   for (i=0;i<STATS_TOTAL+1;i++) {
      printf("%s %ld %d ", statistic_names[i], total_ticks[index][i], total_use[index][i]);
   }

   printf("\n");
}

void profile_print_all_statistics()
{
   int i, j;

   uint64_t ticks = 0;
   uint32_t use = 0;

   printf("Statistics for all communicators:\n");

   for (i=0;i<MAX_COMMUNICATORS;i++) {

      if (total_ticks[i] != NULL) {

         printf("  Communicator %d ", i);

         for (j=0;j<STATS_TOTAL;j++) {
            printf("%s %ld %d", statistic_names[j], total_ticks[i][j], total_use[i][j]);
         }

         printf("\n");

         ticks += total_ticks[i][STATS_TOTAL];
         use += total_use[i][STATS_TOTAL];
      }
   }

   printf("  Overall ticks %ld use %d\n", ticks, use);
}

void dump_profile_info__()
{
   dump_profile_info();
}

void dump_profile_info()
{
   int error;

   error = IMPI_Barrier(MPI_COMM_WORLD);

   if (error != MPI_SUCCESS) {
      ERROR(1, "Barrier failed");
      return;
   }

   messaging_print_profile();
}

#endif