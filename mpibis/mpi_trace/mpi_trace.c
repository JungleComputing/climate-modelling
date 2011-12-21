#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "mpi.h"

// Circular string buffer used for debugging output. Note that if the
// (total) output of a single print operation is larger than DEBUG_BUF_SIZE
// the debug output will not be printed correctly.
#define DEBUG_BUF_SIZE 4096

static char debug_buf[DEBUG_BUF_SIZE+1];
static int debug_buf_pos = 0;

/*****************************************************************************/
/*                              Debugging functions                          */
/*****************************************************************************/

static char *copy_to_debug_buf(const char *tmp, int len)
{
   char *res;

   if (len > DEBUG_BUF_SIZE) {
      // Cannot store this string!
      IERROR(0, "Debug buffer overflow!");
      return &debug_buf[debug_buf_pos];
   }

   if (debug_buf_pos + len > DEBUG_BUF_SIZE) {
      debug_buf_pos = 0;
   }

   res = strcpy(&debug_buf[debug_buf_pos], tmp);
   debug_buf_pos += len;
   return res;
}

static char *comm_to_string(MPI_Comm comm)
{
   int val = 0;
   int len = 0;
   char tmp[64];

   if (comm == MPI_COMM_WORLD) {
      len = sprintf(tmp, "<WORLD>");
      return copy_to_debug_buf(tmp, len+1);
   }

   if (comm == MPI_COMM_SELF) {
      len = sprintf(tmp, "<SELF>");
      return copy_to_debug_buf(tmp, len+1);
   }

   if (comm == MPI_COMM_NULL) {
      len = sprintf(tmp, "<NULL>");
      return copy_to_debug_buf(tmp, len+1);
   }

   len = sprintf(tmp, "<%d>", PMPI_Comm_c2f(comm));

   return copy_to_debug_buf(tmp, len+1);
}

static char *group_to_string(MPI_Group g)
{
   int len;
   char tmp[64];

   if (g == MPI_GROUP_EMPTY) {
      len = sprintf(tmp, "<EMPTY>");
      return copy_to_debug_buf(tmp, len+1);
   }

   if (g == MPI_GROUP_NULL) {
      len = sprintf(tmp, "<NULL>");
      return copy_to_debug_buf(tmp, len+1);
   }

   len = sprintf(tmp, "<%d>", PMPI_Group_c2f(g));
   return copy_to_debug_buf(tmp, len+1);
}

static char *request_to_string(MPI_Request r)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_Request_c2f(r));
   return copy_to_debug_buf(tmp, len+1);
}

static char *type_to_string(MPI_Datatype type)
{
   int len = 0;
   char tmp[1024];

   int error = MPI_Type_get_name(type, tmp, &len);

   if (error != MPI_SUCCESS || len <= 0) {
      len = sprintf(tmp, "<UNKNOWN>");
   }

   return copy_to_debug_buf(tmp, len+1);
}

static char *op_to_string(MPI_Op o)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_Op_c2f(o));
   return copy_to_debug_buf(tmp, len+1);
}

static char *info_to_string(MPI_Info i)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_Info_c2f(i));
   return copy_to_debug_buf(tmp, len+1);
}

static char *file_to_string(MPI_File f)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_File_c2f(f));
   return copy_to_debug_buf(tmp, len+1);
}

static char *win_to_string(MPI_Win w)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_Win_c2f(w));
   return copy_to_debug_buf(tmp, len+1);
}

static char *ranks_to_string(int *ranks, int n)
{
   int i;
   int len = 0;
   char buf[1024];

   len += sprintf(buf, "[");

   for (i=0;i<n;i++) {
      len += sprintf(&buf[len], "%d", ranks[i]);

      if (i != n-1) {
         len += sprintf(&buf[len], ",");
      }
   }

   len += sprintf(&buf[len], "]");
   return copy_to_debug_buf(buf, len+1);
}

static char *ranges_to_string(int range[][3], int n)
{
   int i;
   int len = 0;
   char buf[1024];

   len += sprintf(buf, "[");

   for (i=0;i<n;i++) {
      len += sprintf(&buf[len], "(%d,%d,%d)", range[i][0], range[i][1], range[i][2]);

      if (i != n-1) {
         len += sprintf(&buf[len], ",");
      }
   }

   len += sprintf(&buf[len], "]");
   return copy_to_debug_buf(buf, len+1);
}

int MPI_Init(int *argc, char **argv[])
{
   int i=0;
   int local_rank, local_count;

   init_debug();

   INFO(0, "MPI_Init", "(int *argc=%p, char **argv[]=%p)", argc, argv);

   int status = PMPI_Init(argc, argv);

   if (status == MPI_SUCCESS) {
      PMPI_Comm_rank(MPI_COMM_WORLD, &local_rank);
      PMPI_Comm_size(MPI_COMM_WORLD, &local_count);

      INFO(1, "MPI_Init", "START MPI TRACE on %d of %d", local_rank, local_count);

      for (i=0;i<*argc;i++) {
         INFO(2, "argv[%d] = %s", i, (*argv)[i]);
      }
   }

   return MPI_SUCCESS;
}

#include "generated.c"