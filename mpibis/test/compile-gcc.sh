#!/bin/bash

MPI=/var/scratch/jason/OpenMPI/openmpi-1.4.2-fixed-gnu

echo using $MPI

module purge
module list

rm -f *.o app.exe
gcc -Wall collectives.c -o app.exe -I$MPI/include -L.. -lmpibis -L$MPI/lib -lmpi -lm

