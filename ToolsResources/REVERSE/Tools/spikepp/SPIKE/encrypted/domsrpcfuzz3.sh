#!/bin/sh
MAX=25
UUID=e1af8308-5d1f-11c9-91a4-08002b14a0fa
VERSION=3
PORT=135
TARGET=192.168.1.101
STARTFUNCTION=0



i=$STARTFUNCTION
while [ "$i" -lt "$MAX" ]; do
echo "Doing $i"
./msrpcfuzz $TARGET $PORT $UUID 8a885d04-1ceb-11c9-9fe8-08002b104860 $VERSION 2 $i 1000 20
i=`expr $i + 1`

done

