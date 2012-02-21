#!/bin/bash

module purge
module list

MPI=/var/scratch/jason/OpenMPI/openmpi-1.4.2-fixed-gnu
MPICC=$MPI/bin/mpicc

CC=gcc 

echo using MPI `which $MPICC`
echo using GCC `which $CC`

rm -f *.o ../libmpibis.a

gfortran -c logical.f

$CC -c -g -Wall generated_mpi.c
$CC -c -g -Wall debugging.c
$CC -c -g -Wall wa_sockets.c

$MPICC -c -g -Wall util.c
$MPICC -c -g -Wall request.c
$MPICC -c -g -Wall group.c
$MPICC -c -g -Wall communicator.c
$MPICC -c -g -Wall messaging.c
$MPICC -c -g -Wall operation.c
$MPICC -c -g -Wall reduce_ops.c


#$MPICC -c -g -Wall mpibis.c

#ar -cr ../libmpibis.a generated_mpi.o debugging.o mpibis.o request.o communicator.o messaging.o wa_sockets.o group.o operation.o reduce_ops.o logical.o

rm -f *.o
