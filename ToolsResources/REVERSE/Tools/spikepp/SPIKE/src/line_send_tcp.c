/* generic_listen_tcp.c

   This file implements a very basic  TCP fuzzer. It reads
   in a SPIKE script and then sends it out to a port

    Copyright Dave Aitel, released under GPL 2.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h> /*isalpha*/
#include <unistd.h>
#include <fcntl.h> 
#include <signal.h>

#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"
#include "udpstuff.h"
#include "dlrpc.h"


struct spike * our_spike;

static int caught_sigpipe=0;

static void
sig_pipe(int signal)
{
  
  caught_sigpipe=1;

}

void
usage()
{
  printf("Usage: ./line_send_tcp host port spike_script SKIPVAR SKIPSTR\r\n");
  printf("./line_send_tcp 192.168.1.100 701 something.spk 0 0\n");
  exit(-1);
}

typedef struct mailline {
  char * originalline;
  char * fuzzline;
  int readlines;
};

int
main (int argc, char ** argv)
{
  int port;
  char *script,*script_var;
  char * host;
/*change these to skip over variables*/
  int SKIPFUZZSTR=0;
  int SKIPVARIABLES=0;
  int fuzzvarnum;
  int fuzzstrnum;
  int firstfuzz;

  if (argc!=6)
    {
      printf("argc=%d\n",argc);
      usage();
    }
  host=argv[1];
  port=atoi(argv[2]);
  script=argv[3];
  SKIPVARIABLES=atoi(argv[4]);
  SKIPFUZZSTR=atoi(argv[5]);


  if (script==NULL)
    {
      usage();
    }


  signal(SIGPIPE,sig_pipe);

  our_spike=new_spike();
  
  
  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }
  
  setspike(our_spike);
  
  
  /*very important line. don't forget it*/
  s_init_fuzzing();

  /*zeroth fuzz variable is first variable*/
  s_resetfuzzvariable();
  fuzzvarnum=0;
  fuzzstrnum=0;
  firstfuzz=1;

  printf ("Fuzzing\n");
  while (!s_didlastvariable())
    {
      s_resetfuzzstring();
  

      if (firstfuzz)
	{
	  /*zeroth fuzz string is no change*/
      /*see below for why we have this if statement and loop*/
	  if (fuzzvarnum<SKIPVARIABLES )
	    {
	      for (fuzzvarnum=0; fuzzvarnum<SKIPVARIABLES; fuzzvarnum++)
		{
		  s_incrementfuzzvariable();
		}
	    }
	  
	  /*here is another part of where we implement the ability to jump to a particular
	    place in the fuzzing*/
	  if (fuzzstrnum<SKIPFUZZSTR)
	    {
	      for (fuzzstrnum=0; fuzzstrnum<SKIPFUZZSTR; fuzzstrnum++)
		{
		  s_incrementfuzzstring();
		}
	    }
	  firstfuzz=0;
	}
      else
	{
	  /*we reset this here so every new variable gets a new count*/
	  fuzzstrnum=0;
	}

    /*zeroth fuzz string is no change*/
      while(!s_didlastfuzzstring())
	{
	  spike_clear();
	  printf("Fuzzing Variable %d:%d\n",fuzzvarnum,fuzzstrnum);
	  spike_connect_tcp(host,port);
	  s_readline();
	  printf("Read first line\n");
	  s_parse(script);

	  if (s_send_lines()<0)
	    {
	      printf("Couldn't send data!\r\n");

	      spike_close_tcp();
	      continue;
	    }
	  
	  spike_close_tcp();
	  fuzzstrnum++;
	  s_incrementfuzzstring();

	}
      fuzzvarnum++;

      s_incrementfuzzvariable();
    }/*end for each variable*/
  printf("Done.\n");
  return 0;
} /*end program*/








