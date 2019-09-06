This is SPIKE, a Fuzzer Creation Kit. It has a fairly interesting
generic API for data marshalling. Examples can be found in the src
directory. The API attempts to make duplicating an unknown protocol
easier for a reverse engineer or security researcher.

Included are working (although not completely finished) fuzzers and
supporting code. These include the following:

1. webfuzz: webfuzz is a combination of many small tools to provide a
   flexible and comprehensive web application fuzzing
   tool. Specifically, a modified version of Dug Song's webmitm tool
   is provided. When used as a transparent proxy (by modifying
   /etc/hosts files on the "attacking" machine) it records all client
   requests in separate files. It handles SSL reliably, and requires
   no additional libraries, other than openssl, to compile. These
   recorded client requests (or requests recorded by a network sniffer
   such as Ethereal) are translated into C source files using the
   SPIKE API. When compiled, these C files (webfuzz.c) will
   automatically fuzz that particular cgi. webfuzz.c files may, as
   source files, be easily modified or extended to close in on
   particular vulnerabilities. Because SPIKE is completely open
   source, you get a level of flexibility and effectiveness that
   cannot be matched by proprietary and expensive alternatives. SPIKE
   also includes a very pretty and effective GUI for fuzzing, like
   proprietary alternatives, if you use emacs and a nice window
   manager theme. In addition, because webfuzz completely relies on a
   browser to generate requests for it, it always correctly parses
   Java and scripting languages. 

2. msrpcfuzz: when combined with dcetest, msrpcfuzz attempts to
   exercise an arbitrary ncan_tcp program. It basically sends random
   arguments. If the port suddenly closes, you've found a potentially
   serious bug.

Comments, flames, letters, requests, patches, vicious ferrets
muzzelled with duct tape - send to dave@immunitysec.com


