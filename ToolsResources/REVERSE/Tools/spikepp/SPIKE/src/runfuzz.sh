#!/bin/sh
i=0
while [ "$i" -lt 20 ]; do


  ./msrpcfuzz 10.25.25.15 1154 8cfb5d70-31a4-11cf-a7d8-00805f48a135 8a885d04-1ceb-11c9-9fe8-08002b104860 3 2 $i 100 55

i=`expr $i + 1`

done
