#!/usr/bin/env python
import sys,os
outfiledata="""

s_int_variable(0x04,3);
s_string_variable("TEST");
s_string_variable("TEST");

"""

for i in range(0,0xff):
	print "i=%2.2x"%i
	fout=open("/tmp/sql.spk","w")
	outdata=outfiledata.replace("0x04","0x%2.2x"%i)
	fout.write(outdata)
	fout.close()
	os.system("./generic_send_udp 192.168.1.100 1434 /tmp/sql.spk 0 0  50000 > /dev/null")


	i+=1
