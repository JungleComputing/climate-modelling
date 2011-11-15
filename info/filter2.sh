#!/bin/sh
find . -name "*.F90" -exec grep -Hn "call MPI\_" {} \; | \
awk '{ for (i=2; i<=NF; i++) { if (index($i, "MPI_") == 1) { print $1 " " $i } } }' | \
awk '{ i=index($2, "(") ; if (i > 0) { print $1 " " substr($2, 1, i-1) } else { print $1 " " $2 } }' | \
awk '{ i=index($2, ",") ; if (i > 0) { print $1 " " substr($2, 1, i-1) } else { print $1 " " $2 } }' | \
awk '{ i=index($1, ":") ; if (i > 0) { print substr($1, 1, i-1) " " toupper($2) } else { $1 " " toupper($2) } }'





#awk '{ i=index($2, "(") ; if (i > 0) { print substr($2, 1, i-1) } else { j=index($2, ",") ; if (j > 0) { print substr($2, 1, j-1) } else { print $2 } } }' | \

#awk '{ i=index($1, ":") ; if (i > 0) { line=substr($1, 1, i-1) } ; for (i=2; i<=NF; i++) { if (index($i, "MPI_") == 1) { print line " " $i } } }'
