#include "util.h"

#include "impi.h" 

// NOTE: this is the underlying MPI
#include "mpi.h" 

static int errorcodes_MPI[MPI_ERR_LASTCODE];
static MPI_Datatype types_MPI[IMPI_LAST_DATATYPE];

void init_convert()
{
   int i;

   // Initialize the error codes conversion table

   for (i=0;i<MPI_ERR_LASTCODE;i++) { 
      errorcodes_MPI[i] = IMPI_ERR_UNKNOWN;
   }

   errorcodes_MPI[MPI_SUCCESS]       = IMPI_SUCCESS;
   errorcodes_MPI[MPI_ERR_BUFFER]    = IMPI_ERR_BUFFER;
   errorcodes_MPI[MPI_ERR_COUNT]     = IMPI_ERR_COUNT;
   errorcodes_MPI[MPI_ERR_TYPE]      = IMPI_ERR_TYPE;
   errorcodes_MPI[MPI_ERR_TAG]       = IMPI_ERR_TAG;
   errorcodes_MPI[MPI_ERR_COMM]      = IMPI_ERR_COMM;
   errorcodes_MPI[MPI_ERR_RANK]      = IMPI_ERR_RANK;
   errorcodes_MPI[MPI_ERR_REQUEST]   = IMPI_ERR_REQUEST;
   errorcodes_MPI[MPI_ERR_ROOT]      = IMPI_ERR_ROOT;
   errorcodes_MPI[MPI_ERR_GROUP]     = IMPI_ERR_GROUP;
   errorcodes_MPI[MPI_ERR_OP]        = IMPI_ERR_OP;
   errorcodes_MPI[MPI_ERR_TOPOLOGY]  = IMPI_ERR_TOPOLOGY;
   errorcodes_MPI[MPI_ERR_DIMS]      = IMPI_ERR_DIMS;
   errorcodes_MPI[MPI_ERR_ARG]       = IMPI_ERR_ARG;
   errorcodes_MPI[MPI_ERR_UNKNOWN]   = IMPI_ERR_UNKNOWN;
   errorcodes_MPI[MPI_ERR_TRUNCATE]  = IMPI_ERR_TRUNCATE;
   errorcodes_MPI[MPI_ERR_OTHER]     = IMPI_ERR_OTHER;
   errorcodes_MPI[MPI_ERR_INTERN]    = IMPI_ERR_INTERN;
   errorcodes_MPI[MPI_ERR_PENDING]   = IMPI_ERR_PENDING;
   errorcodes_MPI[MPI_ERR_IN_STATUS] = IMPI_ERR_IN_STATUS;

   for (i=0;i<IMPI_LAST_DATATYPE;i++) { 
      types_MPI[i] = MPI_DATATYPE_NULL;
   }

   types_MPI[IMPI_DATATYPE_NULL]     = MPI_DATATYPE_NULL;
   types_MPI[IMPI_CHAR]              = MPI_CHAR;
   types_MPI[IMPI_SHORT]             = MPI_SHORT;
   types_MPI[IMPI_INT]               = MPI_INT;
   types_MPI[IMPI_LONG]              = MPI_LONG;
   types_MPI[IMPI_UNSIGNED_CHAR]     = MPI_UNSIGNED_CHAR;
   types_MPI[IMPI_UNSIGNED_SHORT]    = MPI_UNSIGNED_SHORT;
   types_MPI[IMPI_UNSIGNED]          = MPI_UNSIGNED;
   types_MPI[IMPI_UNSIGNED_LONG]     = MPI_UNSIGNED_LONG;
   types_MPI[IMPI_FLOAT]             = MPI_FLOAT;
   types_MPI[IMPI_DOUBLE]            = MPI_DOUBLE;
   types_MPI[IMPI_LONG_DOUBLE]       = MPI_LONG_DOUBLE;
   types_MPI[IMPI_BYTE]              = MPI_BYTE;
   types_MPI[IMPI_PACKED]            = MPI_PACKED;
   types_MPI[IMPI_INTEGER]           = MPI_INTEGER;
   types_MPI[IMPI_REAL]              = MPI_REAL;
   types_MPI[IMPI_DOUBLE_PRECISION]  = MPI_DOUBLE_PRECISION;
   types_MPI[IMPI_COMPLEX]           = MPI_COMPLEX;
   types_MPI[IMPI_LOGICAL]           = MPI_LOGICAL;
   types_MPI[IMPI_CHARACTER]         = MPI_CHARACTER;
   types_MPI[IMPI_FLOAT_INT]         = MPI_FLOAT_INT;
   types_MPI[IMPI_DOUBLE_INT]        = MPI_DOUBLE_INT; 
   types_MPI[IMPI_LONG_INT]          = MPI_LONG_INT;
   types_MPI[IMPI_2INT]              = MPI_2INT;
   types_MPI[IMPI_SHORT_INT]         = MPI_SHORT_INT;
   types_MPI[IMPI_LONG_DOUBLE_INT]   = MPI_LONG_DOUBLE_INT;
   types_MPI[IMPI_2REAL]             = MPI_2REAL;
   types_MPI[IMPI_2DOUBLE_PRECISION] = MPI_2DOUBLE_PRECISION;
   types_MPI[IMPI_2INTEGER]          = MPI_2INTEGER;
   types_MPI[IMPI_LONG_LONG_INT]     = MPI_LONG_LONG_INT;
   types_MPI[IMPI_DOUBLE_COMPLEX]    = MPI_DOUBLE_COMPLEX;
   types_MPI[IMPI_INTEGER1]          = MPI_INTEGER1;
   types_MPI[IMPI_INTEGER2]          = MPI_INTEGER2;
   types_MPI[IMPI_INTEGER4]          = MPI_INTEGER4;
   types_MPI[IMPI_INTEGER8]          = MPI_INTEGER8;
//   types_MPI[IMPI_REAL2]             = MPI_REAL2;
   types_MPI[IMPI_REAL4]             = MPI_REAL4;
   types_MPI[IMPI_REAL8]             = MPI_REAL8;  
}

int convert_error_MPI_to_IMPI(int error)
{
   if (error < 0 || error >= IMPI_ERR_LASTCODE) { 
      return IMPI_ERR_UNKNOWN;
   } 

   return errorcodes_MPI[error];
}

MPI_Datatype convert_type_IMPI_to_MPI(int type)
{
   if (type < 0 || type >= IMPI_LAST_DATATYPE) { 
      return MPI_DATATYPE_NULL;
   }
 
   return types_MPI[type];
}

int convert_rank_IMPI_to_MPI(int rank) 
{ 
   if (rank == IMPI_ANY_SOURCE) { 
      return MPI_ANY_SOURCE;
   } 

   return rank;
}

int convert_tag_IMPI_to_MPI(int tag) 
{ 
   if (tag == IMPI_ANY_TAG) { 
      return MPI_ANY_TAG;
   } 

   return tag;
}

