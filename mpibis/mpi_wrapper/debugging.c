#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "flags.h"

#ifdef ENABLE_INTERCEPT

#include "mpi.h"
#include "debugging.h"
#include "logging.h"

#define PRIMITIVE_TYPES 37

static MPI_Datatype primitive_types[PRIMITIVE_TYPES] = {
   MPI_CHAR,
   MPI_SHORT,
   MPI_INT,
   MPI_LONG,
   MPI_UNSIGNED_CHAR,
   MPI_UNSIGNED_SHORT,
   MPI_UNSIGNED,
   MPI_UNSIGNED_LONG,
   MPI_FLOAT,
   MPI_DOUBLE,
   MPI_LONG_DOUBLE,
   MPI_BYTE,
   MPI_PACKED,
   MPI_INTEGER,
   MPI_REAL,
   MPI_DOUBLE_PRECISION,
   MPI_COMPLEX,
   MPI_LOGICAL,
   MPI_CHARACTER,
   MPI_BYTE,
   MPI_PACKED,
   MPI_FLOAT_INT,
   MPI_DOUBLE_INT,
   MPI_LONG_INT,
   MPI_2INT,
   MPI_SHORT_INT,
   MPI_LONG_DOUBLE_INT,
   MPI_2REAL,
   MPI_2DOUBLE_PRECISION,
   MPI_2INTEGER,
   MPI_DOUBLE_COMPLEX,
   MPI_INTEGER1,
   MPI_INTEGER2,
   MPI_INTEGER4,
   MPI_REAL4,
   MPI_REAL8,
   MPI_LONG_LONG_INT };

void catch_derived_datatype(MPI_Datatype datatype)
{
   int i;

   for (i=0;i<PRIMITIVE_TYPES;i++) {
      if (datatype == primitive_types[i]) {
         return;
      }
   }

   FATAL("Attempt to communicate using a derived datatype!");
}

void check_count(int count)
{
   if (count < 0) {
      FATAL("Invalid count %d", count);
   }
}


int check_rank(communicator *c, int rank)
{
   if (rank < 0 || rank >= c->global_size) { \
      FATAL("Invalid rank %d in communicator %d", rank, c->number);
   }
}

#endif // ENABLE_INTERCEPT
