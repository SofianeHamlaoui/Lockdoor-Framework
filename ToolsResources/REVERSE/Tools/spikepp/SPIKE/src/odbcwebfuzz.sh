#!/bin/sh

#takes in a directory full of files which are web requests. Generates webfuzzers for each of these
#runs them, and lets you know which ones have ODBC errors.

TARGET=localhost
PORT=80

if [ "$1" == "" ]; then
    echo "Usage: odbcwebfuzz.sh directory"
    echo "This silly script written by Dave Aitel. daitel@atstake.com"
    exit -1
fi

for file in $1/*; do
    echo "analyzing query $file"
    cp "$file" ./webfuzz.in
    ./makewebfuzz.pl ./webfuzz.in > webfuzz.c
    make
    cp webfuzz "$file".webfuzz
    cp webfuzz.c "$file".c
    ./webfuzz "$TARGET" "$PORT" > "$file".output
done

echo "now just type:"
echo "grep -c ODBC $1/*output*"
echo ":>"
