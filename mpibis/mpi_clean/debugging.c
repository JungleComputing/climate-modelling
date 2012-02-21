#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "flags.h"

#include "debugging.h"

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

char *comm_to_string(IMPI_Comm comm)
{
   int len = 0;
   char tmp[64];

   if (comm == IMPI_COMM_WORLD) {
      len = sprintf(tmp, "<WORLD>");
      return copy_to_debug_buf(tmp, len+1);
   }

   if (comm == IMPI_COMM_SELF) {
      len = sprintf(tmp, "<SELF>");
      return copy_to_debug_buf(tmp, len+1);
   }

   if (comm == IMPI_COMM_NULL) {
      len = sprintf(tmp, "<NULL>");
      return copy_to_debug_buf(tmp, len+1);
   }

   len = sprintf(tmp, "<%d>", comm);
   return copy_to_debug_buf(tmp, len+1);
}

char *request_to_string(IMPI_Request r)
{
   int len;
   char tmp[64];

   if (r == IMPI_REQUEST_NULL) {
      len = sprintf(tmp, "<NULL>");
      return copy_to_debug_buf(tmp, len+1);
   }

   len = sprintf(tmp, "<%d>", r);
   return copy_to_debug_buf(tmp, len+1);
}

char *group_to_string(IMPI_Group g)
{
   int len;
   char tmp[64];

   if (g == IMPI_GROUP_EMPTY) {
      len = sprintf(tmp, "<EMPTY>");
      return copy_to_debug_buf(tmp, len+1);
   }

   if (g == IMPI_GROUP_NULL) {
      len = sprintf(tmp, "<NULL>");
      return copy_to_debug_buf(tmp, len+1);
   }

   len = sprintf(tmp, "<%d>", g);
   return copy_to_debug_buf(tmp, len+1);
}


char *type_to_string(IMPI_Datatype type)
{
   int len = 0;
   char tmp[1024];

   switch (type) {

   case IMPI_DATATYPE_NULL:     /*(0)*/
      len = sprintf(tmp, "<TYPE: NULL>");
      break;

   case IMPI_CHAR:              /*(1)*/
      len = sprintf(tmp, "<MPI_CHAR>");
      break;

   case IMPI_SHORT:             /*(2)*/
      len = sprintf(tmp, "<MPI_SHORT>");
      break;

   case IMPI_INT:               /*(3)*/
      len = sprintf(tmp, "<MPI_INT>");
      break;

   case IMPI_LONG:              /*(4)*/
      len = sprintf(tmp, "<MPI_LONG>");
      break;

   case IMPI_UNSIGNED_CHAR:     /*(5)*/
      len = sprintf(tmp, "<MPI_UNSIGNED_CHAR>");
      break;

   case IMPI_UNSIGNED_SHORT:    /*(6)*/
      len = sprintf(tmp, "<MPI_UNSIGNED_SHORT>");
      break;

   case IMPI_UNSIGNED:          /*(7)*/
      len = sprintf(tmp, "<MPI_UNSIGNED>");
      break;

   case IMPI_UNSIGNED_LONG:     /*(8)*/
      len = sprintf(tmp, "<MPI_UNSIGNED_LONG>");
      break;

   case IMPI_FLOAT:             /*(9)*/
      len = sprintf(tmp, "<MPI_FLOAT>");
      break;

   case IMPI_DOUBLE:            /*(10)*/
      len = sprintf(tmp, "<MPI_DOUBLE>");
      break;

   case IMPI_LONG_DOUBLE:       /*(11)*/
      len = sprintf(tmp, "<MPI_LONG_DOUBLE>");
      break;

   case IMPI_BYTE:              /*(12)*/
      len = sprintf(tmp, "<MPI_BYTE>");
      break;

   case IMPI_PACKED:            /*(13)*/
      len = sprintf(tmp, "<MPI_PACKED>");
      break;

   case IMPI_COMPLEX:           /*(14)*/
      len = sprintf(tmp, "<MPI_COMPLEX>");
      break;

   case IMPI_LOGICAL:           /*(15)*/
      len = sprintf(tmp, "<MPI_LOGICAL>");
      break;

   case IMPI_FLOAT_INT:         /*(16)*/ 
      len = sprintf(tmp, "<MPI_FLOAT_INT>");
      break;

   case IMPI_DOUBLE_INT:        /*(17)*/ 
      len = sprintf(tmp, "<MPI_DOUBLE_INT>");
      break;

   case IMPI_LONG_INT:          /*(18)*/
      len = sprintf(tmp, "<MPI_LONG_INT>");
      break;

   case IMPI_2INT:              /*(19)*/
      len = sprintf(tmp, "<MPI_2INT>");
      break;

   case IMPI_SHORT_INT:         /*(20)*/
      len = sprintf(tmp, "<MPI_SHORT_INT>");
      break;

   case IMPI_LONG_DOUBLE_INT:   /*(21)*/
      len = sprintf(tmp, "<MPI_LONG_DOUBLE_INT>");
      break;

   case IMPI_2REAL:             /*(22)*/
      len = sprintf(tmp, "<MPI_2REAL>");
      break;

   case IMPI_2DOUBLE_PRECISION: /*(23)*/
      len = sprintf(tmp, "<MPI_2DOUBLE_PRECISION>");
      break;

   case IMPI_LONG_LONG_INT:     /*(24)*/
      len = sprintf(tmp, "<MPI_LONG_LONG_INT>");
      break;

   case IMPI_DOUBLE_COMPLEX:    /*(25)*/
      len = sprintf(tmp, "<MPI_DOUBLE_COMPLEX>");
      break;

   case IMPI_REAL2:             /*(26)*/
      len = sprintf(tmp, "<MPI_REAL2>");
      break;

   default:
      len = sprintf(tmp, "<TYPE: %d>", type);
      break;
   }

   return copy_to_debug_buf(tmp, len+1);
}

char *op_to_string(IMPI_Op o)
{
   int len;
   char tmp[64];

   switch (o) { 
   
   case IMPI_OP_NULL: /*(0)*/
      len = sprintf(tmp, "<OP: NULL>");
      break;

   case IMPI_MAX:     /*(1)*/
      len = sprintf(tmp, "<MAX>");
      break;

   case IMPI_MIN:     /*(2)*/
      len = sprintf(tmp, "<MIN>");
      break;

   case IMPI_SUM:     /*(3)*/
      len = sprintf(tmp, "<SUM>");
      break;

   case IMPI_PROD:    /*(4)*/
      len = sprintf(tmp, "<PROD>");
      break;

   case IMPI_LAND:    /*(5)*/
      len = sprintf(tmp, "<LAND>");
      break;

   case IMPI_BAND:    /*(6)*/
      len = sprintf(tmp, "<BAND>");
      break;

   case IMPI_LOR:     /*(7)*/
      len = sprintf(tmp, "<LOR>");
      break;

   case IMPI_BOR:     /*(8)*/
      len = sprintf(tmp, "<BOR>");
      break;

   case IMPI_LXOR:    /*(9)*/
      len = sprintf(tmp, "<LXOR>");
      break;

   case IMPI_BXOR:    /*(10)*/
      len = sprintf(tmp, "<BXOR>");
      break;

   case IMPI_MAXLOC:  /*(11)*/
      len = sprintf(tmp, "<MAXLOC>");
      break;

   case IMPI_MINLOC:  /*(12)*/
      len = sprintf(tmp, "<MINLOC>");
      break;

   default:
      len = sprintf(tmp, "<OP: %d>", o);
      break;
   } 

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
