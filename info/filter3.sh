#!/bin/sh
find . -name "*.F90" -exec grep -Hn "call MPI\_" {} \; | \
awk '{ for (i=2; i<=NF; i++) { if (index($i, "MPI_") == 1) { print $1 } } }' | \
awk '{ i=index($1, ":") ; if (i > 0) { print substr($1, 1, i-1) } else { print $1 } }'
