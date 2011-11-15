#!/bin/bash -v
MPI=/cm/shared/apps/openmpi/gcc/64/current/

rm -f *.o app.exe ../libmpibis.a
mpicc -c -Wall wa_sockets.c
mpicc -c -Wall request.c
mpicc -c -Wall communicator.c
mpicc -c -Wall messaging.c
mpicc -c -Wall mpibis.c
ar -cr ../libmpibis.a mpibis.o request.o communicator.o messaging.o wa_sockets.o
gcc -Wall app4.c -o app.exe -I$MPI/include -L/home/jason/test -lmpibis -L$MPI/lib64 -lmpi -lm
