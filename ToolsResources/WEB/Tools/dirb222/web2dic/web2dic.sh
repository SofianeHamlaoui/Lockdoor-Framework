#!/bin/bash
# Web2Dic
# Created: 28 Enero 2005
# Last modified: 31 Marzo 2005
# By Warezzman for DirB
#
# Requires: wget
#

if ( ! test "$1" ) then
  echo "Usage: ./web2dic.sh http://www.target.com/"
  exit
fi

mkdir temp
cd temp
wget -r -nd -R .css,.CSS,.gif,.GIF,.jpg,.JPG,.jpeg,.JPEG,.png,.PNG,.bmp,.BMP,.pdf,.PDF,.ps,.PS,.swf,.SWF,.doc,.DOC,.ppt,.PPT,.xls,.XLS,.mdb,.MDB,.zip,.ZIP,.jar,.JAR,.cab,.CAB,.class,.CLASS,.rar,.RAR,.gz,.GZ,.z,.Z $1
find . -type f -exec cat {} >> ../input.txt \;
../html2dic ../input.txt | sort | uniq > ../output.txt
cd ..
rm -rf temp
rm input.txt
