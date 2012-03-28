#include "flags.h"

#ifdef IBIS_INTERCEPT

#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#include "generated_header.h"
#include "logging.h"
#include "messaging.h"
#include "profiling.h"

void dump_profile_info__() 
{
   dump_profile_info();
}

void dump_profile_info()
{
   int error; 

   error = IMPI_Barrier(MPI_COMM_WORLD);

   if (error != MPI_SUCCESS) {
      ERROR(1, "Barrier failed");  
      return;
   }

   messaging_print_profile();
}

#endif
