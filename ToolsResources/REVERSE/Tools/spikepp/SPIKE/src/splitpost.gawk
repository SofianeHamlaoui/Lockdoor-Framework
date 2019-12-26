#!/bin/sh
gawk 'BEGIN {RS="&"} {print "s_string(\"&"$1"\");\n\n"}'
