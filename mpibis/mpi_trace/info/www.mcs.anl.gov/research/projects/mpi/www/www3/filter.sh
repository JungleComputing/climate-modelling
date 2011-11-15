#!/bin/sh

cat $1 | awk 'BEGIN { echo = 0 } { if (echo == 1) { print $0 } else { if ($1 == "<PRE>") { echo = 1} }}' | awk 'BEGIN { echo = 1 } { if ($1 == "</PRE>") { echo = 0 } else { if (echo == 1) { print $0 } }}'

echo "#"
