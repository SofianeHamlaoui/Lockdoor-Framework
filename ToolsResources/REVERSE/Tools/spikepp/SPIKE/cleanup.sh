#!/bin/sh
echo "Cleaning up all directories"
cd dcedump
make clean
cd ..
cd spkproxy
./cleanup.sh
cd ..
cd src
make clean
cd ..
rm *~

