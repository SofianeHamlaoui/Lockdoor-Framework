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

//define this when you are ready to store off the bug!
//#define SAVEBUF
//set this to go real slow for no reason!
#define SLEEPSIZE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>


#include <unistd.h>
#include <fcntl.h> 
#include <signal.h>

#include "spike.h"
#include "spike_dcerpc.h"
#include "hdebug.h"
#include "tcpstuff.h"


#include <stdlib.h>

void
usage()
{
  printf("Usage: ./msrpcfuzz target port GUID Version VersionMinor(usually 0) function_number number_of_tries max_number_of_random_items\r\n");
  printf("Example: ./msrpcfuzz 10.25.25.15 135 e1af8308-5d1f-11c9-91a4-08002b14a0fa 3 0 2 10 3 [OBJECT UUID]\n");
  printf("use a while [ 1 ]; do ./msrpcfuzz ... ; done loop to make things continue nicely.\n");
  printf("You're using MSRPCFUZZ written by Dave Aitel in November 2001\n");
  printf("This program protected by GPL v 2.0\n");
  printf("This program's Version = 1.0 I hope you know what you're doing. :>\n");
  printf("Set RANDVAL=int if you don't want to use getpid()\n");
  printf("If you're doing that, set MSRPC_DO=int as well.\n");
  printf("Read the msrpc readme when you get a chance.\n");
  exit(-1);

}

unsigned char *format1;

static int caught_sigpipe=0;

static void
sig_pipe(int signal)
{
  
  caught_sigpipe=1;

}

void
push_pointer()
{
    /*fake pointer structure of some kind that passes through ndrpointerdemarshal*/
  s_binary("01000000");
  //here we are an ndr conformant struct
  s_binary("08000000");
  s_binary("00000000");
  s_binary("08000000");
  s_binary("ABCDABCD");
 //pointers -  does it matter what these are?
  //yes, must be valid "pointers"
  s_binary("20000000");
  s_binary("20000000");
  s_binary("20000000");
}

void
NdrConformantStructUnmarshall()
{
  //start NdrConformantStructUnmarshall
  s_intelword(0x8);
  s_intelword(0);
  s_intelword(0);
}

void
push_valid_thing()
{
  unsigned char longstring[2500];
  unsigned char shortstring[20];
  memset(longstring,0x41,sizeof(longstring));
  longstring[sizeof(longstring)]=0;

  memset(shortstring,0x41,sizeof(shortstring));
  shortstring[sizeof(shortstring)]=0;

  s_binary("01 00 00 00");
  push_pointer();
  push_pointer();
  //s_binary("00 00 00 00");
  s_dce_unistring(shortstring);
  push_pointer();
  s_binary("01000000");
  s_binary("08000000");
  NdrConformantStructUnmarshall();  
  //s_dce_unistring(longstring);
  //s_dce_wordstring(shortstring);
  //push_pointer();
  //push_pointer();

}



void
pretty_print(unsigned char * buffer2, unsigned int returnsize)
{
  unsigned int i;
  /*
    this if statement is used to control weather we do a TON of printing.
    You should only disable it  and use this function after you find a bug
    and have localized it with MSRPC_DO
  */
#ifndef SAVEBUF
  return;
#endif

  fprintf(stderr,"//Pretty print size=%u\n",returnsize);
  fprintf(stderr,"//Buffer:\n");
  for (i=0; i<returnsize; i+=4)
    {
      fprintf(stderr,"s_binary(\"%2.2x%2.2x%2.2x%2.2x\");\n",buffer2[i],buffer2[i+1],buffer2[i+2],buffer2[i+3]);
    }
  fflush(0);

}


void
domeow()
{
  s_binary("05 00 06 00  01 00 00 00  00 00 00 00  68 da 3e c5  ce 38 03 4d  b9 8e 0e d0  5d 4d 86 cb");
  s_binary("00 00 00 00  00 00 00 00  00 02 0c 00  e8 02 00 00  e8 02 00 00  4d 45 4f 57  04 00 00 00");
  s_binary("a2 01 00 00  00 00 00 00  c0 00 00 00  00 00 00 46  38 03 00 00  00 00 00 00  c0 00 00 00");
  s_binary("00 00 00 46  00 00 00 00  b8 02 00 00  b0 02 00 00  00 00 00 00  01 10 08 00  cc cc cc cc");

}


/*pushes one of any number of random MSRPC primatives*/
void
push_random_msrpc_thing()
{
  unsigned int r;
  unsigned char longstring[5000];

#define NUMBEROFTHINGS 18
  /*pick a number between 1 and 100*/
  r=1+(unsigned int) (NUMBEROFTHINGS*1.0*rand()/(RAND_MAX+1.0));

#ifdef SAVEBUF
  printf("random number of things: %d\n",r);
#endif
  /*test*/
  /*r=5;*/
  memset(longstring,0x41,sizeof(longstring));
  longstring[sizeof(longstring)]=0;

  switch (r) {
  case 1:
    s_random_dce_string();
    break;
  case 2:
    s_intelword(rand());
    break;
  case 3:
    s_intelword(0);
    break;
  case 4:
    /*a guid sorta*/
    {
      unsigned char buffer[16];
      uuid_string_to_buffer(buffer,"412f241e-c12a-11ce-abff-0020af6e7a17");
      s_push(buffer,sizeof(buffer));
    }
    break;
  case 5:
    s_dce_wordstring(s_get_random_fuzzstring());
    break;
  case 6:
    s_intelword(1);
    break;
  case 7:
    s_intelword(-1);
    break;
  case 8:
    s_intelword(2);
    break;
  case 9:
    s_intelword(255);
    break;
  case 10:
    s_intelword(0x03);
    break;
  case 11:
    push_pointer();

    break;
  case 12:
    s_dce_wordstring(longstring);
    break;
  case 13:
    //this will pass through ndrsimplestrucunmarshall - in a boring way.
    //so we need a structure that starts HERE
    //3 words of zeros seems to work...
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    break;
  case 14:
    s_dce_unistring(s_get_random_fuzzstring());
    break;
  case 15:
    s_binary("00000000");
    s_binary("00000000");
    break;
  case 16:
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");    
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    break;
  case 17:
    domeow();
    break;
  case 18:
    NdrConformantStructUnmarshall();
  break;
  default:
    printf("Random thing chose a number out of scope %d!\n",r);

  }

  

}


int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  int port;

#define SAVESLOTS 10
  unsigned char *savedbuf[SAVESLOTS];
  unsigned int savedsize[SAVESLOTS];
  int currentsaveslot;

  struct spike * our_spike;
  unsigned long retval;
  unsigned int number_of_tries, number_of_items;
  unsigned short function_number;
  int i,j,s;
  unsigned int r;
  int sent;
  int randval;
  int msrpcdo;
  unsigned char * object;

#define ARGCNEEDED 9
  if (argc<ARGCNEEDED)
    {
      printf("Argc=%d not %d\n",argc,ARGCNEEDED);
      usage();
    }

  /*last arg is the optinal object*/
  object=NULL;
  if (argc==ARGCNEEDED+1)
    object=argv[ARGCNEEDED];

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  msrpcdo=-1;
  if (getenv("MSRPC_DO"))
    {
      msrpcdo=atoi(getenv("MSRPC_DO"));
    }

  function_number=atoi(argv[6]);
  printf("Hitting function number %d\n",function_number);

  number_of_tries=atoi(argv[7]);
  number_of_items=atoi(argv[8]);

  port=atoi(argv[2]);

  for (s=0;s<SAVESLOTS; s++)
    {
      savedbuf[s]=malloc(0);
      savedsize[s]=0;
    }
  currentsaveslot=0;


  signal(SIGPIPE,sig_pipe);

  if (getenv("RANDVAL"))
    randval=atoi(getenv("RANDVAL"));
  else
    randval=getpid();

  printf("MSRPC_DO=%d and RANDVAL=%d\n",msrpcdo,randval);
  s_init_fuzzing();
  srand(randval);
  printf("Initialized random with %d\n",randval);
  fprintf(stderr,"//Initialized random with %d\n",randval);
  for (i=0; i<number_of_tries; i++)
    {
      printf("Try Number %d out of %d on procedure %d\n",i,number_of_tries,function_number);
      fflush(0);
      our_spike=new_spike();
      
      if (our_spike==NULL)
	{
	  fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
	  exit(-1);
	}

      setspike(our_spike);
      
      /*i is sent in just to keep incrementing*/
      /*hardcoded the transfer syntax and syntax version of 2...they
	are always the same*/
      s_dce_bind(argv[3], /*GUID "Interface" */
		 "8a885d04-1ceb-11c9-9fe8-08002b104860", /*syntax - always the same*/
		 i, 
		 atoi(argv[4]), /*interface version major*/
		 atoi(argv[5]), /*interface version minor, usually zero*/
		 2); /*syntax version*/
      s_block_end("DCEFragLength"); /*ALWAYS HAVE TO DO*/
      
      /*phew! :>*/
      
      sent=spike_send_tcp(target,port);
      
      if (sent==0 || caught_sigpipe)
	{
	  /*this means the server went down!*/
	  if (sent==0)
	    fprintf(stderr,"We killed it! \n");
	  else
	    {
	      fprintf(stderr,"We caught sigpipe!\n");
	      fprintf(stdout,"We caught sigpipe!\n");
	    }
	  
	  for (s=0; s<SAVESLOTS;s++)
	    {
	      fprintf(stderr,"Current Save Slot = %d\n",currentsaveslot);
	      fprintf(stderr,"Printing save slot %d\n",s);
#if 0
	      pretty_print(savedbuf[s],savedsize[s]);
#endif
	    }
	  fprintf(stderr,"Done.\n");
	  if (sent==0)
	    {
	      printf("Was not able to connect: Sleeping...\n");
	      sleep(100);
	      //sleep(59999);
	    }
	  exit(1);
	}
      else
	{
#ifdef SAVEBUF
	  /*we always malloc it with something*/
	  free(savedbuf[currentsaveslot]);
#endif
	}
      
      sleep(SLEEPSIZE);
      
      /*clear the response*/
      alarm(1);
      //fcntl(our_spike->fd, F_SETFL, O_NONBLOCK);
      retval=-1;
      if (s_fd_wait())
	{
	  retval=read(our_spike->fd,buffer,1500);
	}
      alarm(0);
      if (retval==-1 && 0) 
	{
	  printf ("couldn't read, continuing\n");
	  perror("read()");
	  spike_close_tcp();
	  spike_free(our_spike);
	  continue;
	}
      
      if (buffer[2]!=DCE_BIND_ACK)
	{
	  printf("Warning, did not recieve bind_ack! Instead got 0x%2.2x\n",buffer[2]);
	  //pause for a sec in case it's just overloaded
	  s_fd_wait();
	  s_fd_wait();
	  if (buffer[2]==0x0d)
	    printf("I believe 0x0d is provider reject - check your version number\n");
	  
	  if (buffer[2]==0x00)
	    {
	      /*this means the server went down!*/
	      fprintf(stderr,"We almost killed it! \n");
	      fprintf(stderr,"Current Save Slot = %d\n",currentsaveslot);
	      fprintf(stderr,"Printing save slot %d\n",currentsaveslot);
#if 0
	      pretty_print(savedbuf[currentsaveslot],savedsize[currentsaveslot]);
#endif
	      
	      fprintf(stderr,"Done.\n");
	      
	    }
	}
      
      /*assume bind success...*/
      /*send a random packet*/
      spike_clear();
      
      /*2nd arg is alloc hint*/
      /*s_dce_call_header(1, 5300, function_number,1,1);*/
      
      r=0+(unsigned int) (number_of_items*1.0*rand()/(RAND_MAX+1.0));
      //r=number_of_items-1;
      /*test*/
      /*r=1;*/
      //domeow();
      push_valid_thing();
      for (j=0; j<r+1; j++)
	{
	  //push_valid_thing();
	  push_random_msrpc_thing();
	}
      printf("Pushing %u things with total size of %u\n",(unsigned int)r+1,(unsigned int)s_get_size());
      
      /*save it off in our saved buffers*/
      if (currentsaveslot==SAVESLOTS-1)
	{
	  currentsaveslot=0;
	}
      else
	{
	  currentsaveslot++;
	}
#ifdef SAVEBUF
      savedbuf[currentsaveslot]=malloc(s_get_size());
      memcpy(savedbuf[currentsaveslot],s_get_databuf(),s_get_size());
      savedsize[currentsaveslot]=s_get_size();
#endif
      
      /*
	Localization of bugs:
	only actually send the packet if we eithar do not have a MSRPC_DO
	value or if our current i is less than that number
	
	Change the < to an == when you get close.
      */
      if (msrpcdo==-1 || i==msrpcdo)
	{
	  printf("Sending request\n");
	  pretty_print(s_get_databuf(),s_get_size());
	  if (!s_do_dce_call(function_number,s_get_databuf(),s_get_size(),object,
			     NULL,0,NULL))
	    {
	      printf("do_dce_call failed\n");
	      spike_free(our_spike);
	      return 0;
	    }
	}
      
      
      /*clear the response*/
      memset(buffer,0,sizeof(buffer));
      alarm(1);
      retval=-1;
      if (s_fd_wait() || s_fd_wait() || s_fd_wait())
	{
	  retval=read(getcurrentspike()->fd,buffer,1500);
	}
      alarm(0);
      if (retval==-1)
	{
	  printf("continuing\n");
	  spike_close_tcp();
	  spike_free(our_spike);
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








