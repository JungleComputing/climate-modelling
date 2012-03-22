#!/bin/bash

MPI=/var/scratch/jason/OpenMPI/openmpi-1.4.2-fixed-gnu

echo using $MPI

module purge
module list

rm -f *.o app.exe
$MPI/bin/mpicc -Wall collectives.c -o app.exe -lm

