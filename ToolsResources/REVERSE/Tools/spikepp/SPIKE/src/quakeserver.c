/* quakeserver.c - the beginnings of a quake fuzzer SERVER

"would also be good to do a spike quake server"

Here it is!

First you send a "challengeResponse"
then a "connectResponse"
you can also send "print"
then you are sending gamestate packets!
These are little endian word counted

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
  printf("Usage: ./quakeserver clienthost port\r\n");
  printf("./quakeserver 192.168.1.102 27960\n");
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
  int port;
  unsigned int index; 
  
  if (argc!=3)
    {
      usage();
    }

  port=atoi(argv[2]);
  
  our_spike=new_spike();
  
  
  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }
  
  setspike(our_spike);
  
  
  if (spike_listen_udp(port)==0)
    {
      printf("Couldn't listen!\n");
      exit(-1);
    }

  
  printf("fd=%d\n",our_spike->fd);
  read_packet(); 
  /*initial packet*/
  spike_clear();
  s_binary("ff ff ff ff");
  s_string("challengeResponse 59776159");
  //s_string_repeat("5",5000); 

 if (!spike_set_sendto_addr(argv[1],port))
     {
       printf("Couldn't resolve hostname for spike_set_sendto_addr!\n");
       exit(-1);
     }

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  
  printf("sent Challenge Response packet\n");
  read_packet();
  /*print packet*/

  spike_clear();
  s_binary("ff ff ff ff"); //for print, connectResponse
  s_string("print"); 
  s_binary("0a");
  s_string("I own you!\n");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent print packet\n");
  read_packet();

 
  /*connectResponse packet*/
  spike_clear();
  s_binary("ff ff ff ff"); 
  s_string("connectResponse");


  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  printf("Sent connectResponse packet\n");

  read_packet();

  for (index=1; index<0xffffffff; index++)
  {
  /*gamestate packet*/
    spike_clear();
    s_intelword(index);
    
    /*
      //original message:
      s_binary("aa 7f"
      "87 00 54 95 65 ce 28 89  9b 52 f0 fd 63 97 64 0b"
      "f3 b8 87 76 48 96 30 8b  fb 68 cc 94 99 17 09 ed"
      "68 20 93 62 16 13 6d 6e  30 91 e6 f6 69 43 f1 fa"
      "06 14 c3 7d d8 35 14 94  af 4b 41 d9 30 34 8b 73"
      "df 16 6d eb 71 ff 57 66  19 61 6c 91 f1 d6 0e 1d"
      "69 5f 56 52 d1 73 db 46  cb 3b 21");
    */
    
    
    //fuzzy message
    s_binary("ac 14" 
  "87 ff 54 95 65 ce 28 89  9b 52 f0 fd 63 97 64 0b"  
  "f3 b8 87 76 48 96 30 8b  fb 68 cc 94 99 17 09 ed" 
  "68 20 93 62 16 13 6d 6e  30 91 e6 f6 69 43 f1 fa ");
    s_string_repeat("\xff", 1500);
    s_binary(
  "06 14 c3 7d d8 35 14 94  af 4b 41 d9 30 34 8b 73"
  "df 16 6d eb 71 ff 57 66  19 61 6c 91 f1 d6 0e 1d"
  "69 5f 56 52 d1 73 db 46  cb 3b 21");
    
    if (spike_send()<0)
      {
	printf("Couldn't send data!\r\n");
	exit(-1);
      }
    printf("Sent gamestate packet\n");
    
    //read_packet();
  }
  printf ("Done fuzzing!\n");
    
  return 0;
}







