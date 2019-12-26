#!/bin/sh

TARGET=target
PORT=80	
METHODS="GET HEAD DELETE PUT POST COPY MOVE MKCOL PROPFIND PROPPATCH LOCK UNLOCK SEARCH BROWSE NOTAMETHOD %n%n%n%n%n%n%n"
#don't do: GET HEAD DELETE PUT 
#don't do trace. Too slow
#nothing in OPTIONS

#EXTENTIONS=".nsf"
EXTENTIONS=".php .asa .asp .cdx .cer .htw .ida .idc .idq .shtm .shtml .stm .printer .jsp .jhtml .bat .exe .com .gif .jpg .mpg .rma .wma .cfm .pl .py .pike .nsf"
#don't do .htr - we know it's buggy

SKIPTO="GET"
SKIPTO2=""

for METHOD in $METHODS; do

if [ "$SKIPTO" != "" ]; then
if [ "$SKIPTO" != "$METHOD" ]; then
#we want to skip past until we get here
continue;
fi
SKIPTO=""
fi

  for EXTENTION in $EXTENTIONS; do

if [ "$SKIPTO2" != "" ]; then
if [ "$SKIPTO2" != "$EXTENTION" ]; then
#we want to skip past until we get here
continue;
fi
SKIPTO2=""
fi


    echo "Doing $METHOD on $EXTENTION"
    ./closed_source_web_server_fuzz "$TARGET" "$PORT" "$METHOD" / bob "$EXTENTION"
  done
done
