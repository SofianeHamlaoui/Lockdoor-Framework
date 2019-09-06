/*

msrpcfuzz
Copyright Dave Aitel
Covered under GPL v 2.0

*/


/*ep appears to have up to function 7. You can tell from the error
  messages when the function is invalid. You have to have netmon or
  ethereal up to actually use this. We don't get feedback from this
  directly.

  This is primitive and needs to be revised, but works well enough.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>


#include "spike.h"
#include "spike_dcerpc.h"
#include "hdebug.h"
#include "tcpstuff.h"


#include <stdlib.h>

void
usage()
{
  printf("Usage: ./msrpcfuzz target port GUID Interface Iver syntaxver function_number number_of_tries max_number_of_random_items\r\n");
  printf("Example: ./msrpcfuzz 10.25.25.15 135 e1af8308-5d1f-11c9-91a4-08002b14a0fa 8a885d04-1ceb-11c9-9fe8-08002b104860 3 2 2 10 3\n");

  printf("You're using MSRPCFUZZ written by Dave Aitel in September 2001\n");
  printf("This program protected by GPL v 2.0\n");
  printf("This program's Version = 0.9. I hope you know what you're doing.\n");
  exit(-1);

}



/*pushes one of any number of random MSRPC primatives*/
void
push_random_msrpc_thing()
{
  unsigned int r;
  unsigned int i;
  unsigned char *format1;


#define NUMBEROFTHINGS 6
  /*pick a number between 1 and 100*/
  r=1+(unsigned int) (NUMBEROFTHINGS*1.0*rand()/(RAND_MAX+1.0));

  /*set up our format string*/
  format1=malloc(5000);
  for (i=0; i<5000; i+=2)
    {
      format1[i]='%';
      format1[i+1]='n';
    }
  format1[i-1]=0;

  switch (r) {
  case 1:
    s_random_dce_string();
    break;
  case 2:
    s_intelword(rand());
    break;
  case 3:
    s_intelword(0);
    s_intelword(0);
    break;
  case 4:
    /*a guid sorta*/
    s_binary("01020304050607080102030405060708");
    break;
  case 5:
    s_dce_wordstring(format1);
    break;
  case 6:
    s_intelword(1);
    s_intelword(1);
    break;
  default:
    printf("Random thing chose a number out of scope %d!\n",r);

  }

  
  free(format1);
}


int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  int port;

  struct spike * our_spike;
  unsigned long retval;
  unsigned int number_of_tries, number_of_items;
  unsigned short function_number;
  int i,j;
  unsigned int r;

#define ARGCNEEDED 10
  if (argc!=ARGCNEEDED)
    {
      printf("Argc=%d not %d\n",argc,ARGCNEEDED);
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  function_number=atoi(argv[7]);
  printf("Hitting function number %d\n",function_number);
  number_of_tries=atoi(argv[8]);
  number_of_items=atoi(argv[9]);

  port=atoi(argv[2]);


  srand(getpid());
  printf("Initialized random with %d\n",getpid());
  for (i=0; i<number_of_tries; i++)
   {
     printf("Try Number %d ",i);
     fflush(0);
     our_spike=new_spike();

     if (our_spike==NULL)
       {
	 fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
	 exit(-1);
       }

     setspike(our_spike);

     /*i is sent in just to keep incrementing*/
     s_dce_bind(argv[3],argv[4],
		i, atoi(argv[5]),
		atoi(argv[6]));
     s_block_end("DCEFragLength");
     spike_send_tcp(target,port);

     s_fd_wait();
     /*clear the response*/
     retval=read(our_spike->fd,buffer,1500);
     if (buffer[3]!=DCE_BIND_ACK)
       {
	 printf("Warning, did not recieve bind_ack!\n");
       }

     /*assume bind success...*/
     /*send a random packet*/
     spike_clear();

     /*2nd arg is alloc hint*/
     /*s_dce_call_header(1, 5300, function_number,1,1);*/

     r=0+(unsigned int) (number_of_items*1.0*rand()/(RAND_MAX+1.0));
     printf("Pushing %d things.\n",r);
     for (j=0; j<r; j++)
       {
	 push_random_msrpc_thing();
       }

     s_block_end("DCEFragLength");

     if (spike_send()<0)
       {
	 printf("Couldn't connect to host or send data!\r\n");
	 exit(-1);
       }

     s_fd_wait();
     /*clear the response*/
     retval=read(our_spike->fd,buffer,1500);
     if (retval==-1)
       {
	 printf("Interesting - server closed socket!\n");
       }
     /*future versions will actually parse this response and report errors*/

     spike_close_tcp(); /*close the socket*/

     spike_clear();
     free(our_spike);
   }

  return 0;
}








