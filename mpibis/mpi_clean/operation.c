#include "flags.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "shared.h"
#include "types.h"
#include "debugging.h"
#include "operation.h"
#include "reduce_ops.h"

#include "impi.h"

// NOTE: this is the underlying MPI
#include "mpi.h"

static operation *ops[MAX_OPERATIONS];
static int next_operation = 0;

static int init_operation(MPI_Op op, IMPI_Op index, MPI_User_function *function, int commute)
{
   if (index < 0 || index > MAX_OPERATIONS) {
      IERROR(1, "Failed to initialize operations -- index %d out of bounds!\n", index);
      return IMPI_ERR_INTERN;
   }

   if (ops[index] != NULL) {
      IERROR(1, "Failed to initialize operations -- index %d already in use!\n", index);
      return IMPI_ERR_INTERN;
   }

   ops[index] = malloc(sizeof(operation));

   if (ops[index] == NULL) {
      IERROR(1, "Failed to initialize operations -- cannot allocate operation %d!\n", index);
      return IMPI_ERR_INTERN;
   }

   ops[index]->function = function;
   ops[index]->commute = commute;
   ops[index]->index = index;
   ops[index]->op = op;

   if (index >= next_operation) {
      next_operation = index+1;
   }

   return IMPI_SUCCESS;
}

int init_operations()
{
   int i, error;

   for (i=0;i<MAX_OPERATIONS;i++) {
      ops[i] = NULL;
   }

   error = init_operation(MPI_OP_NULL, IMPI_OP_NULL, NULL, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_MAX, IMPI_MAX, MAGPIE_MAX, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_MIN, IMPI_MIN, MAGPIE_MIN, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_SUM, IMPI_SUM, MAGPIE_SUM, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_PROD, IMPI_PROD, MAGPIE_PROD, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_MAXLOC, IMPI_MAXLOC, MAGPIE_MAXLOC, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_MINLOC, IMPI_MINLOC, MAGPIE_MINLOC, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_BOR, IMPI_BOR, MAGPIE_BOR, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_BAND, IMPI_BAND, MAGPIE_BAND, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_BXOR, IMPI_BXOR, MAGPIE_BXOR, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_LOR, IMPI_LOR, MAGPIE_LOR, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_LAND, IMPI_LAND, MAGPIE_LAND, 1);
   if (error != IMPI_SUCCESS) return error;

   error = init_operation(MPI_LXOR, IMPI_LXOR, MAGPIE_LXOR, 1);
   if (error != IMPI_SUCCESS) return error;

   return IMPI_SUCCESS;
}

operation *get_operation(IMPI_Op op)
{
   if (op == IMPI_OP_NULL) {
      return NULL;
   }

   return get_operation_with_index(op);
}

operation *get_operation_with_index(int index)
{
   if (index < 0 || index >= MAX_OPERATIONS) {
      ERROR(1, "Failed to retrieve operation, index %d out of bounds\n", index);
      return NULL;
   }

   return ops[index];
}

void set_operation_ptr(IMPI_Op *dst, operation *src)
{
   *dst = src->index;
}

#if 0 

NOT SUPPORTED

operation *create_operation(MPI_User_function *function, int commute)
{
   int error;
   MPI_Op op;
   operation *result;

   if (next_operation >= MAX_OPERATIONS) {
      IERROR(1, "Failed to create operation -- max operations %d reached!\n", MAX_OPERATIONS);
      return NULL;
   }

   if (ops[next_operation] != NULL) {
      IERROR(1, "Failed to create operation -- index %d already in use!\n", next_operation);
      return NULL;
   }

   error = PMPI_Op_create(function, commute, &op);

   if (error != MPI_SUCCESS) {
      ERROR(1, "Failed to create operation -- cannot create MPI operation!\n");
      return NULL;
   }

   result = malloc(sizeof(operation));

   if (result == NULL) {
      IERROR(1, "Failed to create operation -- cannot allocate operation %d!\n", next_operation);
      PMPI_Op_free(&op);
      return NULL;
   }

   result->function = function;
   result->commute = commute;
   result->index = next_operation;
   result->op = op;

   ops[next_operation++] = result;

   return result;
}

void free_operation(operation *op)
{
   int index = op->index;

   if (ops[index] == NULL) {
      return;
   }

   PMPI_Op_free(&ops[index]->op);

   free(ops[index]);
   ops[index] = NULL;
}


#endif



