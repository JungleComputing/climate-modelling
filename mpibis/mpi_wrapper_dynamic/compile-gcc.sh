#!/bin/bash

module purge
module list

MPI=/var/scratch/jason/OpenMPI/openmpi-1.4.2-fixed-gnu
MPICC=$MPI/bin/mpicc

echo using `which $MPICC`

rm -f *.o ../libmpibis.a

$MPICC -c -g -Wall generated_mpi.c
$MPICC -c -g -Wall debugging.c
$MPICC -c -g -Wall wa_sockets.c
$MPICC -c -g -Wall request.c
$MPICC -c -g -Wall communicator.c
$MPICC -c -g -Wall messaging.c
$MPICC -c -g -Wall group.c
$MPICC -c -g -Wall mpibis.c

ar -cr ../libmpibis.a generated_mpi.o debugging.o mpibis.o request.o communicator.o messaging.o wa_sockets.o group.o

rm -f *.o
