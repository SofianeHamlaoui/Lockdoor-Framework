#!/bin/sh
MAX=25
UUID=378e52b0-c0a9-11cf-822d-00aa0051e40f
VERSION=2
PORT=1050
TARGET=192.168.1.100
STARTFUNCTION=0



i=$STARTFUNCTION
while [ "$i" -lt "$MAX" ]; do
echo "Doing $i"
./msrpcfuzz $TARGET $PORT $UUID 8a885d04-1ceb-11c9-9fe8-08002b104860 $VERSION 2 $i 1000 20
i=`expr $i + 1`

done

