/*All the defines for spike stuff*/

#ifndef SPIKE_H
#define SPIKE_H

#include "listener.h"



/* */
struct spike {
  /*updated whenever anything is added into an open and used block*/
  listener block_listeners[MAXLISTENERS];

  /*list of places we need to add sizes to*/
  listener size_listeners[MAXLISTENERS];
  
  /*total size of all data*/
  unsigned long datasize;
  unsigned char *databuf;
  unsigned char *endbuf;
  int fd; /*for holding socket or file information*/
  int proto; /*1 for tcp, 2 for udp*/
  struct sockaddr_in *destsockaddr;

  /*all the web fuzz stuff*/
  int didlastvariable;
  int didlastfuzzstring;
  int didlastfuzzint;
  int didlastbeforefuzzstring;
  int didlastafterfuzzstring;
  int fuzzstring;
  int fuzzvariable;
  int currentvariable;
  int firstvariable;
  int needreserved;

  /*unicode protocol stuff*/
  int do_unicode_ms;
  int terminate_unicode;

};
extern struct spike * current_spike;       

/*new and free . . . constructors and destructors*/
struct spike *
new_spike();

int
spike_free(struct spike * old_spike);

int
s_string(char * instring);

int
s_blocksize_string(char * instring, int size);

int
s_binary(char * instring);

/*
  this pushes an arbitrary byte string onto the spike stack
  and updates any listening blocksizes
*/
int
s_push(unsigned char * pushme, unsigned long size);

int
setspike(struct spike * newspike);

/*if you need a reserved source port (<1024) set this to true*/
int
s_set_needreserved(int needreserved);

int
s_block_start(char *blockname);

int
s_binary_block_size_word_bigendian(char *blockname);

int
s_block_end(char * blockname);

void
s_print_buffer();

void
s_printf_buffer();

unsigned long s_get_size();
unsigned char * s_get_databuf();

/*connects*/
int 
spike_connect_tcp(char * host, int port);

/*connects and sends*/
int
spike_send_tcp(char * host, int port);

/*just sends to already open fd*/
/*right now spike only does tcp...*/
int
spike_send();


/*pushes the word on the s_stack in big endian format*/
int
s_bigword(unsigned int aword);

/*many protocols have a "size" they want to add a certain constant to - maybe
  as a flag the way RPC does with the first word?*/
int
s_binary_block_size_word_bigendian_plussome(char *blockname,long some);

/*none of the "mult" api calls can be done from a .spk script!*/
int
s_binary_block_size_byte_mult(char * blockname, float mult);


int
s_binary_block_size_word_bigendian_mult(char *blockname, float mult);

int
s_binary_block_size_word_intel_mult_plus(char *blockname, long some, float mult);

int
s_binary_block_size_intel_halfword_mult(char *blockname,float mult);

int
s_xdr_string(unsigned char *astring);

/*clears out the bucket*/
int
spike_clear();


/*these two functions just repeat something a certain number of times*/
int
s_binary_repeat(char *instring, int times);

int
s_string_repeat(char *instring, int times);

int
s_string_lf(char * instring);


/*intel order any long . . . generates an intel byte order long from
  our long format, whatever it is. So if we're on an intel box, all
  good, and if we're on a sparc, all good.*/
unsigned long 
ioal(unsigned long myint);

/*same for shorts*/
unsigned short
ioas(unsigned short myshort);

void
intel_order(char * buffer, int length);

/*push a word in intel order*/
int
s_intelword(unsigned int aword);

/*pushes a short onto the spike in intel order*/
/*also returns the short in ioas format - who knows.*/
int
s_intelhalfword(unsigned short ashort);

/*Dark Side of the Moon*/
int
s_binary_block_size_intel_halfword(char *blockname);


int
s_binary_block_size_halfword_bigendian(char * blockname);

int
s_binary_block_size_intel_halfword_plus(char *blockname,long plus);

int
s_binary_block_size_byte(char * blockname);

int
s_binary_block_size_byte_plus(char * blockname, long plus);

/*returns -1 on error*/
int
s_tcp_accept(int listenfd);

void
spike_close_tcp();

void
s_close_udp();

int
s_fd_wait();

void
s_fd_clear();

/*if envvar is set, pushes that, else pushes realvar*/
void
s_string_or_env(unsigned char *envvar, unsigned char *realvar);

void
s_incrementfuzzstring();

void
s_string_variables(unsigned char splitchar, unsigned char *variables);

void
s_string_variable(unsigned char *variable);

void
s_resetfuzzstring();

/*true if we've done the last variable. False otherwise. Survices spike_clear()*/
int
s_didlastvariable();

/*must be called before doing any string fuzzing*/
void
s_resetfuzzvariable();

/*initializes a bunch of strings and throws them into s_fuzzstring,
  which is a global array of strings, ending with a null*/
void
s_init_fuzzing();

int
s_didlastfuzzstring();

void
s_incrementfuzzvariable();

void
s_setfirstvariable();


/*
  if ((response=s_scan_for_variable(buffer,"WWW-Authenticate: NTLM ","\r\n"))!=NULL)
  
  As you can see, s_scan_for_variable(inputbuffer, start, end) tries to match a substring in a buffer separated by two strings. It returns null on failure to match.  */

unsigned char *
s_scan_for_variable(unsigned char * buffer, unsigned char *startdelim, 
		    unsigned char * enddelim);


struct spike * 
getcurrentspike();

int
get_spike_fd();


int
spike_send_udp(char * host, int port);

int 
spike_connect_udp(char * host, int port);

//see notes in udpstuff.c
int
spike_connect_udp_ex(char * host, int port, unsigned short local_port);

int
spike_listen_udp(int port); /*1 on success, 0 on fail*/

/*UDP needs this*/
void
spike_clear_sendto_addr();

/*this is sticky - it stays over spike_clear() so you have to clear it manually*/
int
spike_set_sendto_addr(char * hostname, int destport);

/*End UDP needs*/

/*does some pretty printing after reading a packet*/
/*just a usefull utility thing*/
void
s_read_packet();

/*returns the fuzzstring associated with the index*/
unsigned char *
s_get_fuzzstring(unsigned int index);

/*return the maximum fuzz string*/
unsigned int
s_get_max_fuzzstring();

/*gets a random fuzzstring for you*/
/*returns - fuzzstring*/
unsigned char *
s_get_random_fuzzstring();

int
s_binary_block_size_intel_word(char *blockname);

int
s_binary_block_size_intel_word_plus(char *blockname,long some);

/*pushes an int of type "type"*/
void
s_push_int(int value, int type);

int
s_binary_block_size_word_halfword_bigendian_variable(char *blockname);

int
s_binary_block_size_word_bigendian_variable(char *blockname);

int
s_binary_block_size_byte_variable(char * blockname);

int
s_binary_block_size_intel_halfword_variable(char *blockname);

int
s_binary_block_size_intel_word_variable(char *blockname);

int 
s_blocksize_unsigned_string_variable(char * instring, int size);

int
s_blocksize_asciihex(char * blockname);

int
s_blocksize_asciihex_variable(char * blockname);

void
s_set_unicode(int ms);

/*not a real unicode string, but a pretend unicode string anyways*/
/*e.g. we don't handle non ascii printable characters correctly, since
  that never seems to work out for me.*/
void
s_unistring(char *instring);



/*Read a packet deliminated in a bigendian word size, then packet data*/
/*returns -1 on error, else, size of data read*/
/*puts data read into the buf, which it mallocs. Be sure to free this.*/
int
s_read_bigendian_word_packet(char **buf);

int
s_binary_block_size_intel_halfword_plus_variable(char *blockname,long plus);
int
s_binary_block_size_halfword_bigendian_mult(char * blockname, float mult);


int
s_readline();

//send one line at a time
int
s_send_lines();
/* ifdef SPIKE_H */
#endif 







