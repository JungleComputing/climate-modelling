#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "flags.h"

#ifdef ENABLE_INTERCEPT

#include "debugging.h"
#include "generated_header.h"

#ifdef IBIS_INTERCEPT

//#include "types.h"
//#include "request.h"
//#include "communicator.h"
//#include "group.h"

#endif

// Indentation levels (single string with multiple pointers
// pointing into it at different locations).
static char *maxindent = "         ";
static char *indents[10];

// Circular string buffer used for debugging output. Note that if the
// (total) output of a single print operation is larger than DEBUG_BUF_SIZE
// the debug output will not be printed correctly.
#define DEBUG_BUF_SIZE 4096

static char debug_buf[DEBUG_BUF_SIZE+1];
static int debug_buf_pos = 0;

#ifdef CATCH_DERIVED_TYPES

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

   ERROR(0, "CATCH_DERIVED_DATATYPE: Attempt to communicate using a derived datatype!");
}

#endif

/***************************************************************************/
/*                        Conversion functions                             */
/***************************************************************************/

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

char *comm_to_string(MPI_Comm comm)
{
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

#ifdef IBIS_INTERCEPT
   int number = IMPI_Comm_c2f(comm);
#else
   int number = PMPI_Comm_c2f(comm);
#endif

/*
   communicator *c = get_communicator(comm);

   if (c == NULL) {
      len = sprintf(tmp, "<ERROR>");
      return copy_to_debug_buf(tmp, len+1);
   }
*/

   len = sprintf(tmp, "<%d>", number);
   return copy_to_debug_buf(tmp, len+1);
}

char *request_to_string(MPI_Request r)
{
   int len;
   char tmp[64];

   if (r == MPI_REQUEST_NULL) {
      len = sprintf(tmp, "<NULL>");
      return copy_to_debug_buf(tmp, len+1);
   }

#ifdef IBIS_INTERCEPT
   int number = IMPI_Request_c2f(r);
#else
   int number = PMPI_Request_c2f(r);
#endif

   len = sprintf(tmp, "<%d>", number);
   return copy_to_debug_buf(tmp, len+1);
}

char *group_to_string(MPI_Group g)
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

#ifdef IBIS_INTERCEPT
   int number = IMPI_Group_c2f(g);
#else
   int number = PMPI_Group_c2f(g);
#endif

/*
   group *c = get_group(g);

   if (c == NULL) {
      len = sprintf(tmp, "<ERROR>");
      return copy_to_debug_buf(tmp, len+1);
   }
*/

   len = sprintf(tmp, "<%d>", number);
   return copy_to_debug_buf(tmp, len+1);
}


char *type_to_string(MPI_Datatype type)
{
   int len = 0;
   char tmp[1024];

   int error = PMPI_Type_get_name(type, tmp, &len);

   if (error != MPI_SUCCESS || len <= 0) {
      len = sprintf(tmp, "<UNKNOWN>");
   }

   return copy_to_debug_buf(tmp, len+1);
}

char *op_to_string(MPI_Op o)
{
   int len;
   char tmp[64];

#ifdef IBIS_INTERCEPT
   int number = IMPI_Op_c2f(o);
#else
   int number = PMPI_Op_c2f(o);
#endif

   len = sprintf(tmp, "<%d>", number);
   return copy_to_debug_buf(tmp, len+1);
}

char *info_to_string(MPI_Info i)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_Info_c2f(i));
   return copy_to_debug_buf(tmp, len+1);
}

char *file_to_string(MPI_File f)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_File_c2f(f));
   return copy_to_debug_buf(tmp, len+1);
}

char *win_to_string(MPI_Win w)
{
   int len;
   char tmp[64];

   len = sprintf(tmp, "<%d>", PMPI_Win_c2f(w));
   return copy_to_debug_buf(tmp, len+1);
}

char *ranks_to_string(int *ranks, int n)
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

char *ranges_to_string(int range[][3], int n)
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


void init_debug()
{
   int i;
   char *tmp = maxindent;

   for (i=9;i>=0;i--) {
      indents[i] = tmp++;
   }

   debug_buf[DEBUG_BUF_SIZE] = '\0';
}

static void println(int indent, const char * header, const char *fmt, va_list argp)
{
   if (indent < 0) {
      indent = 0;
   } else if (indent > 9) {
      indent = 9;
   }

   fprintf(stderr, "%s%s: ", indents[indent], header);
   vfprintf(stderr, fmt, argp);
   fprintf(stderr, "\n");
}

#if VERBOSE > 2

static void println2(int indent, const char * header, const char * func, const char *fmt, va_list argp)
{
   if (indent < 0) {
      indent = 0;
   } else if (indent > 9) {
      indent = 9;
   }

   fprintf(stderr, "%s%s: %s ", indents[indent], header, func);
   vfprintf(stderr, fmt, argp);
   fprintf(stderr, "\n");
}

#endif

void DEBUG(int indent, const char *fmt, ...)
{
#if VERBOSE > 3
   va_list argp;
   va_start(argp, fmt);
   println(indent, "DEBUG", fmt, argp);
   va_end(argp);
#endif
}

void INFO(int indent, const char *func, const char *fmt, ...)
{
#if VERBOSE > 2
   va_list argp;
   va_start(argp, fmt);
   println2(indent, "INFO", func, fmt, argp);
   va_end(argp);
#endif
}

void WARN(int indent, const char *fmt, ...)
{
#if VERBOSE > 1
   va_list argp;
   va_start(argp, fmt);
   println(indent, "WARN", fmt, argp);
   va_end(argp);
#endif
}

void ERROR(int indent, const char *fmt, ...)
{
#if VERBOSE > 0
   int *ptr;

   va_list argp;
   va_start(argp, fmt);
   println(indent, "ERROR", fmt, argp);
   va_end(argp);

   ptr = NULL;
   *ptr = 1;
#endif
}

void IERROR(int indent, const char *fmt, ...)
{
#if VERBOSE > 0
   int *ptr;

   va_list argp;
   va_start(argp, fmt);
   println(indent, "INTERNAL ERROR", fmt, argp);
   va_end(argp);

   ptr = NULL;
   *ptr = 1;

#endif
}


#endif // ENABLE_INTERCEPT
