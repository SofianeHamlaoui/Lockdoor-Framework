/* 
   ss_spike.c

   Include routines for spiking the RV streaming server

   This never got completed. But maybe someday it will. The
   important thing is that you can do binary protocols over HTTP
   quite nicely in SPIKE

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"



void
usage()
{
  fprintf(stderr,"Usage: ss_spike target\n");
  exit(-1);
}

int
main (int argc, char ** argv)
{
  char * target;
  char buffer[500000];

  struct spike * our_spike;
  unsigned long retval;
  int fd;

  if (argc!=2)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\n",argv[1]);

  our_spike=new_spike();


  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\n");
      exit(-1);
    }

  setspike(our_spike);

  memset(buffer,0x41,sizeof(buffer));
  buffer[sizeof(buffer)]=0;

  s_string("POST / HTTP/1.0\n");
  s_string("Content-type: application/x-www-form-urlencoded\n");
  s_string("Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.\n");
  s_string("User-Agent: Mozilla/4.0 (compatible; MSIE 5.0; Win32)");
  s_string("Host: ");
  s_string(target);
  s_string("\n");
  s_string("Content-Length: ");
  s_blocksize_string("contents",20);
  s_string("\n");
  s_string("Connection: Keep-Alive\n");
  s_string("\n");
  s_block_start("contents");
  s_binary_block_size_word_bigendian("packet");
  s_block_start("packet");
  s_binary("9955eeaa");
  s_binary("06");
  s_string("mtype");
  s_string(buffer);
  s_binary("0008024a0004");
  s_string("sub");
  s_binary("00017a0000003d");
  s_binary("9955eeaa");
  s_binary("04");
  s_block_end("packet");
  s_block_end("contents");
  /* s_print_buffer(); */
  /* s_printf_buffer(); */

  if (spike_send_tcp(target,80)<0)
    {
      printf("Couldn't connect to host or send data!\n");
      exit(-1);
    }
  retval=tcpread(fd,1000,buffer);
  printf("Buffer[%d]=*%s*\n",(int)retval,buffer);
  return 0;
}







