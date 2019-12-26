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
#include "dlrpc.h"
#define TOTALTOSEND 10
struct spike * our_spike;


void
usage()
{
  printf("Usage: ./gsu target port file.spk startvariable startfuzzstring startvariable startstring totaltosend\r\n");
  printf("./gsu 192.168.1.104 80 file.spk 0 0 5000\n");
  exit(-1);
}


int
main (int argc, char ** argv)
{
  char * target;
  int port;
  char * spkfile;
  /*change these to skip over variables*/
  int SKIPFUZZSTR=0;
  int SKIPVARIABLES=0;
  int fuzzvarnum;
  int fuzzstrnum;
  int firstfuzz;
  int totalsent,totaltosend;
  int i;
  
  if (argc!=7)
    {
      printf("argc=%d\n",argc);
      usage();
    }
  
  target=argv[1];
  printf("Target is %s\r\n",argv[1]);
  
  port=atoi(argv[2]);
  spkfile=(argv[3]); 
  SKIPVARIABLES=atoi(argv[4]);
  SKIPFUZZSTR=atoi(argv[5]);


  totalsent=1;
  totaltosend=atoi(argv[6]);

  our_spike=new_spike();
  
  s_init_fuzzing();

  if (SKIPFUZZSTR>s_get_max_fuzzstring())
    {
      printf("Your start fuzzstring is greater than %d, which is the maximum\n",s_get_max_fuzzstring());
      exit(1);
    }

    
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
 /*zeroth fuzz variable is first variable*/
  s_resetfuzzvariable();
  fuzzvarnum=0;
  fuzzstrnum=0;
  firstfuzz=1;

  while (!s_didlastvariable())
    {
      s_resetfuzzstring();
      /*zeroth fuzz string is no change*/

      
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
      while(!s_didlastfuzzstring())
	{
	  printf("Fuzzing Variable %d:%d\n",fuzzvarnum,fuzzstrnum);
	  
	  spike_clear();
	  s_parse(spkfile);
/*
	  printf("spike size is 0x%x\n",(unsigned int)our_spike->datasize);
	  for (i=0; i<our_spike->datasize; i++) {
	    printf("0x%2.2x ",our_spike->databuf[i]);
	    
	  }
	  */
	  
	  if (spike_send()<0)
	    {
	      printf("Couldn't send data!\r\n");
	      exit(-1);
	    }
	  
	  //s_read_packet();
	  //

	  if (totalsent==totaltosend) 
	  {
		  printf("\nReached maximum packets to send (%d).\n",totaltosend);
		exit(0);
	  }
	  totalsent++;
	  
	  fuzzstrnum++;
	  s_incrementfuzzstring();
	  //spike_close_tcp();
	  /*Use this for testing against netcat*/
	  /*
	    sleep(1);
	  */
	}/*end for each fuzz string*/
      fuzzvarnum++;
      s_incrementfuzzvariable();
    }/*end for each variable*/
  
  
  printf ("\nDone fuzzing!\n");
    
  return 0;
}







