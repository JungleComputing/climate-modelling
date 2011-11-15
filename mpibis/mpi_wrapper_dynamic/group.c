#include "flags.h"

#ifdef IBIS_INTERCEPT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"
#include "types.h"
#include "group.h"
#include "communicator.h"

static group *groups[MAX_GROUPS];
static int next_group   = 0;

// A group is an ordered set of process identifiers (henceforth processes); processes are implementation-dependent
// objects. Each process in a group is associated with an integer rank. Ranks are contiguous and start from zero.
// Groups are represented by opaque group objects, and hence cannot be directly transferred from one process to
// another. A group is used within a communicator to describe the participants in a communication ``universe'' and
// to rank such participants (thus giving them unique names within that ``universe'' of communication).
//
// There is a special pre-defined group: MPI_GROUP_EMPTY, which is a group with no members. The predefined constant
// MPI_GROUP_NULL is the value used for invalid group handles.

static int FORTRAN_MPI_GROUP_NULL;
static int FORTRAN_MPI_GROUP_EMPTY;

int init_groups()
{
   int i;

   FORTRAN_MPI_GROUP_NULL = PMPI_Group_c2f(MPI_GROUP_NULL);
   FORTRAN_MPI_GROUP_EMPTY = PMPI_Group_c2f(MPI_GROUP_EMPTY);

   for (i=0;i<MAX_GROUPS;i++) {
      groups[i] = NULL;
   }

   return MPI_SUCCESS;
}

static int is_special(int index)
{
   return (index == FORTRAN_MPI_GROUP_NULL || index == FORTRAN_MPI_GROUP_EMPTY);
}

static int add_group(group *g)
{
   while (is_special(next_group) && next_group < MAX_GROUPS) {
      fprintf(stderr, "   WARNING: Skipping group %d\n", next_group);
      next_group++;
   }

   if (next_group >= MAX_GROUPS) {
      fprintf(stderr, "   INTERNAL ERROR: MAX_GROUPS reached!\n");
      return -1;
   }

   groups[next_group++] = g;
   return (next_group-1);
}


// Utility function to print a group value.
void print_group(MPI_Group g)
{
    unsigned char *buf;
    int i;

    buf = (unsigned char *) &g;

    for (i=0;i<sizeof(MPI_Group);i++) {

       if (i == 0) {
          fprintf(stderr, "[%x", buf[i]);
       } else {
          fprintf(stderr, ":%x", buf[i]);
       }
    }
    fprintf(stderr, "]");
}

// Utility function to get a group value.
group *get_group(MPI_Group src)
{
  group *res;

  if (src == MPI_GROUP_EMPTY || src == MPI_GROUP_NULL) {
#if VERBOSE
     fprintf(stderr, "   WARNING: get_group returns NULL!\n");
#endif
     return NULL;
  }

  memcpy(&res, &src, sizeof(group *));

//fprintf(stderr, "   get_group result: %p\n", res);

  return res;
}


group* get_group_with_index(int f)
{
   if (f < 0 || f >= MAX_GROUPS) {
      fprintf(stderr, "   ERROR: get_group_with_index(index=%d) index out of bounds!\n", f);
      return NULL;
   }

   if (groups[f] == NULL) {
      fprintf(stderr, "   ERROR: get_group_with_index(index=%d) index not active!\n", f);
      return NULL;
   }

   return groups[f];
}

// Utility function to set group ptr.
void set_group_ptr(MPI_Group *dst, group *src)
{
   memcpy(dst, &src, sizeof(group *));
}

static group *create_group(int members)
{
fprintf(stderr, "   ###: creating group of size %d\n", members);

   group *res = malloc(sizeof(group));

   if (res == NULL) {
      fprintf(stderr, "   INTERNAL ERROR: Failed to allocate group struct!\n");
      return NULL;
   }

fprintf(stderr, "   ###: allocating members array for %d members\n", members);

   res->members = malloc(members * sizeof(int));

   if (res->members == NULL) {
      fprintf(stderr, "   INTERNAL ERROR: Failed to allocate group member array!\n");
      free(res);
      return NULL;
   }

   res->index = add_group(res);

fprintf(stderr, "   ###: index of group is %d\n", res->index);

   if (res->index == -1) {
      fprintf(stderr, "   INTERNAL ERROR: Failed to store group!\n");
      free(res->members);
      free(res);
      return NULL;
   }

fprintf(stderr, "   ###: group creating done\n");

   return res;
}

// Returns in group a handle to the group of comm.
int group_comm_group(communicator *in, group **out)
{
   // TODO: makes sense to cache some of these groups!
   int i;
   group *res;

   if (in == NULL || out == NULL) {
      fprintf(stderr, "   ERROR: Group_comm_group(in=%p, out=%p) get NULL as input!\n", in, out);
      return MPI_ERR_COMM;
   }

   res = create_group(in->global_size);

   if (res == NULL) {
      fprintf(stderr, "   INTERNAL ERROR: Failed to create new group!\n");
      return MPI_ERR_INTERN;
   }

   res->size = in->global_size;
   res->pid = in->global_rank;
   res->rank = in->global_rank;

   for (i=0;i<in->global_size;i++) {
      res->members[i] = i;
   }

   *out = res;

   return MPI_SUCCESS;
}

// Returns the rank of this process in the given group.
int group_rank(group *g, int *rank)
{
   if (g == NULL) {
      return MPI_ERR_GROUP;
   }

   *rank = g->rank;

   return MPI_SUCCESS;
}

// Returns the size of a group.
int group_size(group *g, int *size)
{
   if (g == NULL) {
      return MPI_ERR_GROUP;
   }

   *size = g->size;

   return MPI_SUCCESS;
}

// returns all elements of the first group ( group1), followed by all elements of second group ( group2) not in first.
int group_union(group *in1, group *in2, group **out)
{
   group *res;
   int *members;
   int i, j, found, index, size, pid, rank, current;

//fprintf(stderr, "   JASON: Group union %p %p %p\n", in1, in2, out);

   size = in1->size + in2->size;

//fprintf(stderr, "   JASON: Group union size %d %d %d\n", in1->size, in2->size, size);

   members = malloc(size * sizeof(int));

   if (members == NULL) {
      fprintf(stderr, "   INTERNAL ERROR: Failed to allocate temporary group!\n");
      return MPI_ERR_INTERN;
   }

   index = 0;

   for (i=0;i<in1->size;i++) {
      members[index++] = in1->members[i];
   }

//fprintf(stderr, "   JASON: Group union group 1 copied\n");

   if (in2->pid != -1) {
      pid = in2->pid;
   }

   for (i=0;i<in2->size;i++) {

      found = 0;

      current = in2->members[i];

      for (j=0;j<in1->size;j++) {
         if (current == in1->members[j]) {
            found = 1;
            break;
         }
      }

      if (found == 0) {
         if (current == in2->pid) {
            rank = index;
         }

         members[index++] = current;
      }
   }

//fprintf(stderr, "   JASON: Group union create_group APP %d\n", index);

   res = create_group(index);

   if (res == NULL) {
      fprintf(stderr, "   INTERNAL ERROR: Failed to create group!\n");
      return MPI_ERR_INTERN;
   }

//fprintf(stderr, "   JASON: Group union create_group done %p\n", res);

   res->size = index;

   if (in1->pid != -1) {
      res->pid = in1->pid;
      res->rank = in1->rank;
   } else if (in2->pid != -1) {
      res->pid = pid;
      res->rank = rank;
   } else {
      res->pid = -1;
      res->rank = -1;
   }

//fprintf(stderr, "   JASON: Group union copy to group\n");

   for (i=0;i<index;i++) {
      res->members[i] = members[i];
   }

//fprintf(stderr, "   JASON: Group union done %p %p!\n", out, res);

   *out = res;

   return MPI_SUCCESS;
}

// returns all elements of the first group that are also in the second group, ordered as in first group.
int group_intersection(group *in1, group *in2, group **out)
{
   // TODO: no implemented
   return MPI_ERR_GROUP;
}

// returns all elements of the first group that are not in the second group, ordered as in the first group.
int group_difference(group *in1, group *in2, group **out)
{
   // TODO: no implemented
   return MPI_ERR_GROUP;
}

// The function MPI_GROUP_INCL creates a group newgroup that consists of the n processes in group with ranks
// rank[0], , rank[n-1]; the process with rank i in newgroup is the process with rank ranks[i] in group. Each
// of the n elements of ranks must be a valid rank in group and all elements must be distinct, or else the
// program is erroneous. If n~=~0, then newgroup is MPI_GROUP_EMPTY. This function can, for instance, be used
// to reorder the elements of a group. See also MPI_GROUP_COMPARE.
int group_incl(group *in, int n, int ranks[], group **out)
{
   group *res;
   int i, next;

   if (in == NULL) {
      fprintf(stderr, "   ERROR: Group include got NULL as input!\n");
      return MPI_ERR_GROUP;
   }

   res = create_group(n);

   if (res == NULL) {
      fprintf(stderr, "   ERROR: Failed to allocate space for new group!\n");
      return MPI_ERR_INTERN;
   }

   res->size = n;
   res->pid = in->pid;
   res->rank = -1;

   for (i=0;i<n;i++) {

      next = ranks[i];

      if (next < 0 || next >= in->size) {
         fprintf(stderr, "   ERROR: Rank out of bounds (%d)!\n", next);
         free(res);
         return MPI_ERR_RANK;
      }

      res->members[i] = in->members[next];

      if (res->members[i] == in->pid) {
         res->rank = i;
      }
   }

   *out = res;

   return MPI_SUCCESS;
}

// The function MPI_GROUP_EXCL creates a group of processes newgroup that is obtained by deleting from group
// those processes with ranks ranks[0] , ranks[n-1]. The ordering of processes in newgroup is identical to
// the ordering in group. Each of the n elements of ranks must be a valid rank in group and all elements must
// be distinct; otherwise, the program is erroneous. If n~=~0, then newgroup is identical to group.
int group_excl(group *in, int n, int ranks[], group **out)
{
   // TODO: no implemented
   return MPI_ERR_GROUP;
}

static int get_range(group *in, int *dst, int *next, int first, int last, int stride)
{
   // Extract a certain range of processes from the group in.
   // Note that stride may be positive or negative, but may not be 0!
   int i, index;

   index = *next;

   if (stride == 0 || first < 0 || first >= in->size || last < 0 || last >= in->size) {
      return MPI_ERR_RANK;
   }

   if (first < last && stride > 0) {
      // forward range
      for (i=first;(i<in->size && i<=last);i+=stride) {

         if (index >= in->size) {
            return MPI_ERR_ARG;
         }

         dst[index++] = in->members[i];
      }

   } else if (last < first && stride < 0) {
      // backward range
      for (i=first;(i>=0 && i>=last);i+=stride) {

         if (index >= in->size) {
            return MPI_ERR_ARG;
         }

         dst[index++] = in->members[i];
      }

   } else {
      return MPI_ERR_RANK;
   }

   *next = index;

   return MPI_SUCCESS;
}

// The ranges consist of the triplets (first, last, stride). The functionality of this routine is specified to
// be equivalent to expanding the array of ranges to an array of the included ranks and passing the resulting
// array of ranks and other arguments to MPI_GROUP_INCL. A call to MPI_GROUP_INCL is equivalent to a call to
// MPI_GROUP_RANGE_INCL with each rank i in ranks replaced by the triplet (i,i,1) in the argument ranges.
int group_range_incl(group *in, int n, int ranges[][3], group **out)
{
   group *res;
   int *tmp;
   int i, next, error;

   if (in == NULL) {
      fprintf(stderr, "   ERROR: Group_range_incl got NULL as input!\n");
      return MPI_ERR_GROUP;
   }

//   fprintf(stderr, "   *group_range_incl called. (%d %d %d) %d\n", in->rank, in->pid, in->size, n);

   tmp = malloc(in->size * sizeof(int));

   if (tmp == NULL) {
      fprintf(stderr, "   ERROR: Failed to allocate space for temporary group!\n");
      return MPI_ERR_INTERN;
   }

   next = 0;

   for (i=0;i<n;i++) {
      error = get_range(in, tmp, &next, ranges[i][0], ranges[i][1], ranges[i][2]);

      if (error != MPI_SUCCESS) {
         fprintf(stderr, "   ERROR: Failed to retrieve range %d (%d,%d,%d)!\n",
				i, ranges[i][0], ranges[i][1], ranges[i][2]);
         free(tmp);
         return error;
      }
   }

   res = create_group(next);

   if (res == NULL) {
      fprintf(stderr, "   ERROR: Failed to allocate space for new group!\n");
      free(tmp);
      return MPI_ERR_INTERN;
   }

   res->size = next;
   res->pid = in->pid;
   res->rank = -1;

   for (i=0;i<next;i++) {
      res->members[i] = tmp[i];

      if (tmp[i] == in->pid) {
         res->rank = i;
      }
   }

   free(tmp);

   *out = res;

   return MPI_SUCCESS;
}

// The functionality of this routine is specified to be equivalent to expanding the array of ranges to an array
// of the excluded ranks and passing the resulting array of ranks and other arguments to MPI_GROUP_EXCL. A call
// to MPI_GROUP_EXCL is equivalent to a call to MPI_GROUP_RANGE_EXCL with each rank i in ranks replaced by the
// triplet (i,i,1) in the argument ranges.
int group_range_excl(group *in, int n, int ranges[][3], group **out)
{
   // TODO: no implemented
   return MPI_ERR_GROUP;
}

/*
MPI_Group group_f2c(int f)
{
   int i;
   MPI_Group res;

fprintf(stderr, "   MPI_GROUP_F2C %d\n", f);

   if (f >= MAX_GROUPS) {
fprintf(stderr, "   MPI_GROUP_F2C %d returns NULL(0)\n", f);
      return MPI_GROUP_NULL;
   }

   if (groups[f] == NULL) {
fprintf(stderr, "   MPI_GROUP_F2C %d returns NULL(1)\n", f);
      return MPI_GROUP_NULL;
   }

   set_group_ptr(&res, groups[f]);

fprintf(stderr, "   MPI_GROUP_F2C %d returns %p\n", f, groups[f]);

   return res;
}
*/


#endif // IBIS_INTERCEPT
