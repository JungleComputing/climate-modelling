#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define DATA_COUNT 22464

#define COUNT 100
#define REPEAT 100

int main(int argc, char *argv[])
{
    int  namelen, rank, size, i, j, error, prev, next;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    double start, end;

    double *sbuf;
    double *rbuf;

    MPI_Status  rstat[2];
    MPI_Status  sstat[2];

    MPI_Request rreq[2];
    MPI_Request sreq[2];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Get_processor_name(processor_name, &namelen);

    fprintf(stderr, "Process %d of %d on %s\n", rank, size, processor_name);

    sbuf = malloc(DATA_COUNT*sizeof(double));
    rbuf = malloc(DATA_COUNT*sizeof(double));

    rreq[0] = MPI_REQUEST_NULL;
    rreq[1] = MPI_REQUEST_NULL;

    sreq[0] = MPI_REQUEST_NULL;
    sreq[1] = MPI_REQUEST_NULL;

    next = rank+1;

    if (next == size) {
       next = 0;
    }

    prev = rank-1;

    if (prev == -1) {
       prev = size-1;
    }

    for (i=0;i<COUNT;i++) {


       start = MPI_Wtime();

       for (j=0;j<REPEAT;j++) {

           // Handle prev
           error = MPI_Irecv(rbuf, DATA_COUNT, MPI_DOUBLE, prev, 0, MPI_COMM_WORLD, &rreq[0]);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "Irecv failed (1)! %d\n", error);
              return 1;
           }

           error = MPI_Isend(sbuf, DATA_COUNT, MPI_DOUBLE, prev, 1, MPI_COMM_WORLD, &sreq[0]);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "Irecv failed (1)! %d\n", error);
              return 1;
           }

           // Handle next
           error = MPI_Irecv(rbuf, DATA_COUNT, MPI_DOUBLE, next, 1, MPI_COMM_WORLD, &rreq[1]);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "Irecv failed (2)! %d\n", error);
              return 1;
           }

           error = MPI_Isend(sbuf, DATA_COUNT, MPI_DOUBLE, next, 0, MPI_COMM_WORLD, &sreq[1]);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "Irecv failed (2)! %d\n", error);
              return 1;
           }

           error = MPI_Wait(&rreq[0], &rstat[0]);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "WAIT failed (1)! %d\n", error);
              return 1;
           }

           error = MPI_Wait(&rreq[1], &rstat[1]);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "WAIT failed (2)! %d\n", error);
              return 1;
           }

           error = MPI_Waitall(2, sreq, sstat);

           if (error != MPI_SUCCESS) {
              fprintf(stderr, "WAITAll failed (1)! %d\n", error);
              return 1;
           }
       }

       end = MPI_Wtime();

       printf("Iteration %d took %f sec. (%f/sec/phase)\n", i, end-start, (end-start)/REPEAT);
    }

    fprintf(stderr, "Done!\n");

    MPI_Finalize();

    return 0;
}

