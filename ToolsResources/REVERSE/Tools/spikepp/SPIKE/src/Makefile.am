DL_LIB = @DL_LIB@
SUFFIXES =  .a .o .c
CC		=  gcc
CFLAGS		=  -Wall -funsigned-char -c -fPIC -ggdb

#webfuzz goes last so we don't crash on it early...
#removed: oldmsrpcfuzz

BINS = ss_spike pmspike statd_spike x11_spike post_fuzz post_spike msrpcfuzz do_post citrix ntlm2 ntlm_brute closed_source_web_server_fuzz quakeserver quake halflife webfuzz dltest gopherd generic_listen_tcp libdlrpc.so generic_web_server_fuzz generic_chunked generic_web_server_fuzz2 generic_send_udp generic_send_tcp sendmsrpc sunrpcfuzz dceoversmb msrpcfuzz_udp line_send_tcp

ALL = $(BINS)

INCLUDE = -I/usr/local/include -I../include  -Ilibntlm-0.21/

LIBSOCKET = 

SPIKE_OBS = spike.o listener.o hdebug.o tcpstuff.o spike_dcerpc.o base64.o udpstuff.o spike_oncrpc.o

SS_OBS = $(SPIKE_OBS) ss_spike.o

PM_OBS = $(SPIKE_OBS) pmspike.o

SD_OBS = $(SPIKE_OBS) statd_spike.o

X11_OBS= $(SPIKE_OBS) x11_spike.o

PS_OBS= $(SPIKE_OBS) post_spike.o


#SPIKE_HEADERS = ../include/spike.h 

HC_LIBS = $(LIBSOCKET) 


.c.o:
	${CC} ${CFLAGS} ${INCLUDE} $<

all: $(ALL)

ss_spike: $(SS_OBS)
	$(CC) -o ss_spike $(SS_OBS)

pmspike: $(PM_OBS)
	$(CC) -o pmspike $(PM_OBS)

statd_spike: $(SD_OBS)
	$(CC) -o statd_spike $(SD_OBS)

x11_spike: $(X11_OBS)
	$(CC) -o x11_spike $(X11_OBS)

post_spike: $(PS_OBS)
	$(CC) -o post_spike $(PS_OBS)

webfuzz: $(SPIKE_OBS) webfuzz.o
	$(CC) -o webfuzz $(SPIKE_OBS) webfuzz.o

gopherd: $(SPIKE_OBS) gopherd.o
	$(CC) -o gopherd $(SPIKE_OBS) gopherd.o

post_fuzz: $(SPIKE_OBS) post_fuzz.o
	$(CC) -o post_fuzz $(SPIKE_OBS) post_fuzz.o


closed_source_web_server_fuzz: $(SPIKE_OBS) closed_source_web_server_fuzz.o
	$(CC) -o closed_source_web_server_fuzz $(SPIKE_OBS) closed_source_web_server_fuzz.o

msrpcfuzz: $(SPIKE_OBS) msrpcfuzz.o
	$(CC) -ggdb -o msrpcfuzz $(SPIKE_OBS) msrpcfuzz.o

msrpcfuzz_udp: $(SPIKE_OBS) msrpcfuzz_udp.o
	$(CC) -ggdb -o msrpcfuzz_udp $(SPIKE_OBS) msrpcfuzz_udp.o

dceoversmb: $(SPIKE_OBS) dceoversmb.o
	$(CC) -ggdb -o dceoversmb $(SPIKE_OBS) dceoversmb.o

cifs: $(SPIKE_OBS) cifs.o
	$(CC) -ggdb -o cifs $(SPIKE_OBS) cifs.o


sendmsrpc: $(SPIKE_OBS) sendmsrpc.o
	$(CC) -ggdb -o sendmsrpc $(SPIKE_OBS) sendmsrpc.o

oldmsrpcfuzz: $(SPIKE_OBS) oldmsrpcfuzz.o
	$(CC) -ggdb -o oldmsrpcfuzz $(SPIKE_OBS) oldmsrpcfuzz.o

do_post: $(SPIKE_OBS) do_post.o
	$(CC) -ggdb -o do_post $(SPIKE_OBS) do_post.o

ntlm_brute: $(SPIKE_OBS) ntlm_brute.o libntlm-0.21/libntlm.a
	$(CC) -ggdb -o ntlm_brute  $(SPIKE_OBS) ntlm_brute.o libntlm-0.21/libntlm.a

ntlm2: $(SPIKE_OBS) ntlm2.o libntlm-0.21/libntlm.a
	$(CC) -ggdb -o ntlm2  $(SPIKE_OBS) ntlm2.o libntlm-0.21/libntlm.a

sunrpcfuzz: $(SPIKE_OBS) sunrpcfuzz.o
	$(CC) -ggdb -o sunrpcfuzz $(SPIKE_OBS) sunrpcfuzz.o

libntlm-0.21/libntlm.a: 
	cd libntlm-0.21 && make

webmitm: webmitm.o buf.o
	$(CC) -ggdb -o webmitm webmitm.o buf.o -lssl -lcrypto
 
citrix: citrix.o $(SPIKE_OBS)
	$(CC) -ggdb -o citrix citrix.o $(SPIKE_OBS)

halflife: halflife.o $(SPIKE_OBS)
	$(CC) -ggdb -o halflife halflife.o $(SPIKE_OBS)

quake: quake.o $(SPIKE_OBS)
	$(CC)  -ggdb -o quake quake.o  $(SPIKE_OBS)

quakeserver: quakeserver.o $(SPIKE_OBS)
	$(CC)  -ggdb -o quakeserver quakeserver.o  $(SPIKE_OBS)

dltest: dltest.o dlrpc.o dlargs.o
	$(CC) -ggdb -o dltest dltest.o dlrpc.o dlargs.o $(DL_LIB)

#this next line may be less than portable
libdlrpc.so: dlrpc.o dlargs.o $(SPIKE_OBS)
	ld   -shared -soname libdlrpc.so -o libdlrpc.so -lc  dlrpc.o dlargs.o $(SPIKE_OBS)

generic_send_tcp: generic_send_tcp.o dlrpc.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o generic_send_tcp generic_send_tcp.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc

generic_listen_tcp: generic_listen_tcp.o dlrpc.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o generic_listen_tcp generic_listen_tcp.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc

generic_web_server_fuzz: generic_web_server_fuzz.o dlrpc.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o generic_web_server_fuzz generic_web_server_fuzz.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc

generic_web_server_fuzz2: generic_web_server_fuzz2.o dlrpc.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o generic_web_server_fuzz2 generic_web_server_fuzz2.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc

generic_chunked: generic_chunked.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o generic_chunked generic_chunked.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc

generic_send_udp: generic_send_udp.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o generic_send_udp generic_send_udp.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc

line_send_tcp: line_send_tcp.o dlargs.o $(SPIKE_OBS) libdlrpc.so
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	$(CC) -ggdb -o line_send_tcp line_send_tcp.o dlrpc.o dlargs.o $(SPIKE_OBS) $(DL_LIB) -L. -ldlrpc


clean:
	rm -f *~ *.bak
	rm -f include/*~ include/*.bak
	rm -f *.o $(BINS)
	cd libntlm-0.21 && make clean

realclean: clean
	rm -rf #* *.swp *~ core
	ls -al out* *.txt





