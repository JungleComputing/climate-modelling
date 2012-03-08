#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    MPI_Comm half;
    MPI_Comm oddeven;

    int  namelen, rank, size, newrank, newsize, color, key, i, j, error;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    int *sendbuffer;
    int *recvbuffer;

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

    fprintf(stderr, "Starting collective communication test\n");

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

    fprintf(stderr, "BCAST MPI_COMM_WORLD ************\n");

    for (i=0;i<size;i++) {

       for (j=0;j<size;j++) {
          sendbuffer[j] = rank;
       }

       error = MPI_Bcast(sendbuffer, size, MPI_INT, i, MPI_COMM_WORLD);

       if (error != MPI_SUCCESS) {
          fprintf(stderr, "BCAST failed!\n");
          MPI_Finalize();
          return 1;
       }

       for (j=0;j<size;j++) {
          if (sendbuffer[j] != i) {
             fprintf(stderr, "BCAST result incorrect on %d (expected %d got %d)\n", rank, i, sendbuffer[j]);
             MPI_Finalize();
             return 1;
          }
       }
    }

    fprintf(stderr, " - OK\n");



    fprintf(stderr, "Done!\n");

    MPI_Finalize();

    return 0;
}

