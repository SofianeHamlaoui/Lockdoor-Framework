/*

msrpcfuzz
Copyright Dave Aitel
Covered under GPL v 2.0

*/


/*
 You can tell from the error messages when the function is
  invalid. You have to have netmon or ethereal up to actually use
  this. We don't get feedback from this directly.

  This is primitive and needs to be revised, but works well enough.
*/

#define SLEEPSIZE 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <fcntl.h>

#include "spike.h"
#include "spike_dcerpc.h"
#include "hdebug.h"
#include "tcpstuff.h"


#include <stdlib.h>

void
usage()
{
  printf("Usage: ./sendmsrpc target port function\r\n");
  exit(-1);
 
}
    
unsigned char *format1;

void
push_valid_things()
{
  unsigned char buff[0x2710];

  /*a long string of 3c*/
  memset(buff,0x00,sizeof(buff));
  memset(buff,0x3c,sizeof(buff)-1);
  
//#include "/home/dave/spikePRIVATE/crashep4_test.c"
}   


    
   
void
pretty_print(unsigned char * buffer2, unsigned int returnsize)
{
  unsigned int i;
  printf("Pretty print size=%u\n",returnsize);
  printf("Buffer:\n");
  for (i=0; i<returnsize; i+=4)
    {
      fprintf(stdout,"\"%2.2x%2.2x%2.2x%2.2x\")\n",buffer2[i],buffer2[i+1],buffer2[i+2],buffer2[i+3]);
    }
}

int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  int port;
  unsigned char *savedbuf;
  unsigned int savedsize;
  struct spike * our_spike;
  unsigned long retval;
  unsigned int number_of_tries;
  unsigned short function_number;
  int i;


#define ARGCNEEDED 4
  if (argc!=ARGCNEEDED)
    {
      printf("Argc=%d not %d\n",argc,ARGCNEEDED);
      usage();
    }


  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  function_number=atoi(argv[3]);
  printf("Hitting function number %d\n",function_number);
  number_of_tries=1;


  port=atoi(argv[2]);

  savedsize=0;
  savedbuf=malloc(0);

  srand(getpid());
//  printf("Initialized random with %d\n",getpid());
  for (i=0; i<number_of_tries; i++)
   {
   //  printf("Try Number %d ",i);
     fflush(0);
     our_spike=new_spike();

     if (our_spike==NULL)
       {
	// fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
	 exit(-1);
       }
     s_init_fuzzing();
     setspike(our_spike);

     /*i is sent in just to keep incrementing*/
     /*hardcoded the transfer syntax and syntax version of 2...they
       are always the same*/

     s_dce_bind("000001a0-0000-0000-c000-000000000046", /*GUID "Interface" */
		"8a885d04-1ceb-11c9-9fe8-08002b104860", /*syntax - always the same*/
		i, 
		0, /*interface version major*/
		0, /*interface version minor, usually zero*/
		2); /*syntax version (always 2)*/
     s_block_end("DCEFragLength"); /*ALWAYS HAVE TO DO*/

     /*phew! :>*/
     if (spike_send_tcp(target,port)==0)
       {
	 /*this means the server went down!*/
	 printf("Could not connect to server on that port. Exiting.\n");
	 exit(1);
       }

     s_fd_wait();
     /*clear the response*/
     fcntl(our_spike->fd, F_SETFL, O_NONBLOCK);
     retval=read(our_spike->fd,buffer,1500);
     if (retval==-1 && 0) 
       {
	 printf ("couldn't read, continuing\n");
	 perror("read()");
	 spike_close_tcp();
	 continue;
       }
     //sleep(1);
     s_fd_wait();
     if (buffer[2]!=DCE_BIND_ACK)
       {
	 printf("Warning, did not recieve bind_ack! Instead got 0x%x\n",buffer[2]);
	 if (buffer[2]==0x0d)
	   printf("I believe 0x0d is provider reject - check your version number\n");
       }

     /*assume bind success...*/
     /*send a random packet*/
     spike_clear();

     /*here we create the data we're going to send out*/

     push_valid_things();
     printf("Pushing things with a total size of %u\n",(unsigned int)s_get_size());

     if (!s_do_dce_call(function_number,s_get_databuf(),s_get_size(),NULL,NULL,0,NULL))
       {
	 printf("do_dce_call failed\n");
	 return 0;
       }
//     function_number++; 

     s_fd_wait();
     /*clear the response*/
     memset(buffer,0,sizeof(buffer));
     retval=read(getcurrentspike()->fd,buffer,1500);
     if (retval==-1)
       {
	 printf("continuing\n");
	 spike_close_tcp();
	 continue;
       }
     if (buffer[2]==DCE_FAULT)
       {
	 printf("Recieved DCE_FAULT packet. As expected.\n");
       }
     else
       {
	 printf("Recived response packet with type %x\n",buffer[2]);
	 printf("Type 0x0c is BIND_ACK\n");
       }
     
     if (retval==-1)
       {
	 printf("dce call: Interesting - server closed socket!\n");
	 //return 0;
       }
     
     
     spike_close_tcp(); /*close the socket*/
     
     spike_clear();
     spike_free(our_spike);
   }
  
  return 0;
}








