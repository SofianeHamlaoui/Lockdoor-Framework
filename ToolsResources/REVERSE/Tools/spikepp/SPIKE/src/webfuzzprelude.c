#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>


#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"
void
usage()
{
  fprintf(stderr,"Usage: ./webfuzz target port \r\n");
  exit(-1);
}

int
main (int argc, char ** argv)
{
  int first;
  char * target;
  char buffer[1500000];
  int port;
  char * optional;
  struct spike * our_spike;
  unsigned long retval;
  int notfin;

  if (argc!=3)
    {
      usage();
    }



  /*sheesh.*/
  signal(SIGPIPE,SIG_IGN);

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);
  
  if (argc>3)
    optional=argv[3];

  our_spike=new_spike();
  s_init_fuzzing();

  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1); 
    }

  setspike(our_spike);
  
  /*during s_variable push, if fuzzstring is == currentfuzzstring
    then set didlastfuzzstring. If fuzzvariable is == current
    variable, set didlastfuzzvariable*/

  /*zeroth fuzz variable is first variable*/
  s_resetfuzzvariable();
  while (!s_didlastvariable())
    {
      s_resetfuzzstring();
      /*zeroth fuzz string is no change*/

      while(!s_didlastfuzzstring())
	{
	  spike_clear();

	  



