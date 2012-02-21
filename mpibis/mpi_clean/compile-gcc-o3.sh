#!/bin/bash

module purge
module list

CC=gcc 

echo using `which $CC`

rm -f *.o ../libmpibis-clean.a

gfortran -c logical.f

$CC -c -O3 -Wall generated_mpi.c
$CC -c -O3 -Wall debugging.c
$CC -c -O3 -Wall wa_sockets.c
$CC -c -O3 -Wall request.c

#$CC -c -O3 -Wall communicator.c
#$CC -c -O3 -Wall messaging.c
#$CC -c -O3 -Wall group.c
#$CC -c -O3 -Wall mpibis.c
#$CC -c -O3 -Wall operation.c
#$CC -c -O3 -Wall reduce_ops.c

#ar -cr ../libmpibis-clean.a generated_mpi.o debugging.o mpibis.o request.o communicator.o messaging.o wa_sockets.o group.o operation.o reduce_ops.o logical.o
ar -cr ../libmpibis-clean.a generated_mpi.o debugging.o wa_sockets.o

rm -f *.o
