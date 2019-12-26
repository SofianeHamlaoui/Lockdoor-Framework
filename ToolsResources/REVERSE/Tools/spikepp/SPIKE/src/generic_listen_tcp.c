/* generic_listen_tcp.c

   This file implements a very basic listening TCP server. It reads
   int a SPIKE script and then 

    Copyright Dave Aitel, released under GPL 2.0
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


struct spike * our_spike;

void
usage()
{
  printf("Usage: ./generic_listen_tcp port spike_script\r\n");
  printf("./generic_listen_tcp 70 gopherd.spk\n");
  exit(-1);
}



int
main (int argc, char ** argv)
{
  int port;
  int result,listenfd,acceptfd;
  char *script;


  if (argc!=3)
    {
      printf("argc=%d\n",argc);
      usage();
    }
  
  port=atoi(argv[1]);
  script=argv[2];
  if (script==NULL)
    {
      usage();
    }
  
  our_spike=new_spike();
  
  
  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }
  
  setspike(our_spike);
  
  
  result=make_tcp_listener(port,&listenfd);
  if (result!=1)
    {
      printf("Failed to listen on that port: %d\n",port);
      usage();
    }

  /*very important line. don't forget it*/
  s_init_fuzzing();

  /*zeroth fuzz variable is first variable*/
  s_resetfuzzvariable();
  
  while (!s_didlastvariable())
    {
      s_resetfuzzstring();
      /*zeroth fuzz string is no change*/
      while(!s_didlastfuzzstring())
	{
	  spike_clear();

	  /*now loop on connections to that port*/
	  acceptfd=s_tcp_accept(listenfd);
	  if (acceptfd==-1)
	    {
	      printf("Accept failed for some reason!\n");
	      continue;
	    }


	  s_parse(script);
	  

	  if (spike_send()<0)
	    {
	      printf("Couldn't send data!\r\n");
	      spike_close_tcp();
	      continue;
	    }
	  spike_close_tcp();
	  
	  s_incrementfuzzstring();
	}
      s_incrementfuzzvariable();
    }/*end for each variable*/
  printf("Done.\n");
  return 0;
} /*end program*/








