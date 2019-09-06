/* quake.c - the beginnings of a quake fuzzer

would also be good to do a spike quake server


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
  printf("Usage: ./quake target port\r\n");
  printf("./quake 192.168.1.102 27960\n");
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
  s_string("getchallenge");
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  
  printf("sent getchallenge packet\n");
  read_packet();
  
  /*connect packet*/
  spike_clear();
  s_binary("ff ff ff ff");
  //s_string("connect ");
  s_string("connect ");
  s_binary(" 01 5d  3a 8c b1 88 21 6c 76 4c   ");

  s_string_repeat("%n",50); 
  s_binary("c5 0e cc e9 c3 1c 98 0c  ab 60 24 2c 80 fd 7d d8"  
  "d6 2e 8e 89 43 61 ef 38  05 2e 2e 4b e1 5c e7 ac" 
  "23 7d 69 ac 2f 6a 35 61  fb 6a 51 4f 01 23 e5 30"
  "18 3c 62 bd 5d aa cf e2  8f a6 71 ce 12 6e c7 63"
  "1a 61 4c 62 bd 03 2c af  a7 cf a1 da fd f7 7a 97"
  "c7 4d 8f 7e 51 09 be 88  94 5b d4 2f c4 4e 46 b8"
  "fc bd 09 97 40 65 20 dc  a4 45 01 5e a5 79 f2 ca"
  "a8 70 a1 7e 14 56 3a 3b  77 98 14 0e e1 4a a8 0e"
  "0b 27 2f c1 6d 39 53 80  9c 13 ac 58 03 33 9f 61"
  "81 ba 87 b7 bb bc ec ca  f9 3a 5e 14 df d8 db ba"
  "57 a6 4f 78 82 b5 c3 f2  f8 c1 64 c5 89 95 89 20"
  "e9 40 a1 62 fd c7 72 b2  84 58 f9 d3 ae de f5 f1"
  "47 16 c9 d1 c9 f8 36 43  7d b1 e2 43 bf b5 f1 f7"
  "d3 8b a2 db 57 8b 75 93  e2 a7 bd a7 75 f2 1c d9"
  "f3 cf 67 08 8e 8b cc 71  fb c6 de 5f d2 13 3f cb"
  "4e 3a 03");
  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent connect packet\n");
  /*second little packet we send*/
  read_packet();


  printf ("Done fuzzing!\n");
    
  return 0;
}







