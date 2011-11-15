#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "debugging.h"

static char *maxindent = "         ";
static char *indents[10];

void init_debug()
{
   int i;
   char *tmp = maxindent;

   for (i=9;i>=0;i--) {
      indents[i] = tmp++;
   }
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
   va_list argp;
   va_start(argp, fmt);
   println(indent, "ERROR", fmt, argp);
   va_end(argp);
#endif
}

void IERROR(int indent, const char *fmt, ...)
{
#if VERBOSE > 0
   va_list argp;
   va_start(argp, fmt);
   println(indent, "INTERNAL ERROR", fmt, argp);
   va_end(argp);
#endif
}
