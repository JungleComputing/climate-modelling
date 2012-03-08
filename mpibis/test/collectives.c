#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

static int test_bcast(MPI_Comm comm, char *name)
{
   int i, j, error, rank, size;
   int *sendbuffer;

   MPI_Comm_size(comm, &size);
   MPI_Comm_rank(comm, &rank);

   sendbuffer = malloc(size * sizeof(int));

   if (sendbuffer == NULL) {
      fprintf(stderr, "Failed to allocate sendbuffer!\n");
      MPI_Finalize();
      return 1;
   }

   fprintf(stderr, "BCAST %s ************\n", name);

   for (i=0;i<size;i++) {

      for (j=0;j<size;j++) {
         sendbuffer[j] = rank;
      }

      error = MPI_Bcast(sendbuffer, size, MPI_INT, i, comm);

      if (error != MPI_SUCCESS) {
         fprintf(stderr, "BCAST %s failed!\n", name);
         MPI_Finalize();
         return 1;
      }

      for (j=0;j<size;j++) {
         if (sendbuffer[j] != i) {
            fprintf(stderr, "BCAST %s result incorrect on %d (expected %d got %d)\n", name, rank, i, sendbuffer[j]);
            MPI_Finalize();
            return 1;
         }
      }
   }

   fprintf(stderr, " - BCAST %s OK\n", name);

   free(sendbuffer);

   return 0;
}

static int test_gather(MPI_Comm comm, char *name)
{
   int i, j, p, error, rank, size;
   int *sendbuffer;
   int *recvbuffer;

   MPI_Comm_size(comm, &size);
   MPI_Comm_rank(comm, &rank);

   sendbuffer = malloc(size * sizeof(int));

   if (sendbuffer == NULL) {
      fprintf(stderr, "Failed to allocate sendbuffer!\n");
      MPI_Finalize();
      return 1;
   }

   recvbuffer = malloc(size * size * sizeof(int));

   if (recvbuffer == NULL) {
      fprintf(stderr, "Failed to allocate recvbuffer!\n");
      MPI_Finalize();
      return 1;
   }

   fprintf(stderr, "GATHER %s ************\n", name);

   for (j=0;j<size;j++) {
      sendbuffer[j] = rank;
   }

   for (j=0;j<size*size;j++) {
      recvbuffer[j] = -1;
   }

   for (i=0;i<size;i++) {

      error = MPI_Gather(sendbuffer, size, MPI_INT, recvbuffer, size, MPI_INT, i, comm);

      if (error != MPI_SUCCESS) {
         fprintf(stderr, "GATHER %s failed!\n", name);
         MPI_Finalize();
         return 1;
      }

      if (rank == i) {
         for (j=0;j<size;j++) {
            for (p=0;p<size;p++) {
               if (recvbuffer[j*size+p] != j) {
                  fprintf(stderr, "GATHER %s result incorrect on %d (expected %d got %d)\n", name, rank, j, recvbuffer[j*size+p]);
                  MPI_Finalize();
                  return 1;
               }
            }
         }
      }
   }

   fprintf(stderr, " - GATHER %s OK\n", name);

   free(sendbuffer);
   free(recvbuffer);

   return 0;
}



int main(int argc, char *argv[])
{
    MPI_Comm half;
    MPI_Comm oddeven;

    int  namelen, rank, size, newrank, newsize, color, key, i, j, error;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    int halfsize;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Get_processor_name(processor_name, &namelen);

    fprintf(stderr, "Process %d of %d on %s\n", rank, size, processor_name);

    halfsize = size / 2;

    if (size % 2 == 1) {
       halfsize++;
    }

    color = rank / (halfsize);
    key = rank % (halfsize);

    fprintf(stderr, "Testing half split: rank=%d size=%d half=%d color=%d key=%d\n", rank, size, halfsize, color, key);

    error = MPI_Comm_split(MPI_COMM_WORLD, color, key, &half);

    if (error != MPI_SUCCESS) {
	fprintf(stderr, "Half split failed!\n");
        MPI_Finalize();
        return 1;
    }

    MPI_Comm_size(half, &newsize);
    MPI_Comm_rank(half, &newrank);

    fprintf(stderr, "Half split succeeded: I am now process %d of %d on %s\n", newrank, newsize, processor_name);

    // Split odd-even
    color = rank / 2;
    key = rank % 2;

    fprintf(stderr, "Testing odd-even split: rank=%d size=%d color=%d key=%d\n", rank, size, color, key);

    error = MPI_Comm_split(MPI_COMM_WORLD, color, key, &oddeven);

    if (error != MPI_SUCCESS) {
	fprintf(stderr, "Odd-even split failed!\n");
        MPI_Finalize();
        return 1;
    }

    MPI_Comm_size(oddeven, &newsize);
    MPI_Comm_rank(oddeven, &newrank);

    fprintf(stderr, "Odd-even split succeeded: I am now process %d of %d on %s\n", newrank, newsize, processor_name);

    fprintf(stderr, "\n****************************************************\n\n");

    fprintf(stderr, "Starting BCAST tests\n");

    error = test_bcast(MPI_COMM_WORLD, "MPI_COMM_WORLD");
    if (error != 0) return error;

    error = test_bcast(half, "world half");
    if (error != 0) return error;

    error = test_bcast(oddeven, "world odd/even");
    if (error != 0) return error;

    fprintf(stderr, "\n****************************************************\n\n");

    fprintf(stderr, "Starting GATHER tests\n");

    error = test_gather(MPI_COMM_WORLD, "MPI_COMM_WORLD");
    if (error != 0) return error;

    error = test_gather(half, "world half");
    if (error != 0) return error;

    error = test_gather(oddeven, "world odd/even");
    if (error != 0) return error;

    fprintf(stderr, "\n****************************************************\n\n");



    fprintf(stderr, "Done!\n");

    MPI_Finalize();

    return 0;
}

