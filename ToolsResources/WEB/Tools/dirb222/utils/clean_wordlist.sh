#!/bin/sh
#
#

echo "Cleaning $1"

cat $1 > $1.old

sort $1 > tmp.1

uniq.exe tmp.1 > tmp.2

cat tmp.2 > $1

rm tmp.*