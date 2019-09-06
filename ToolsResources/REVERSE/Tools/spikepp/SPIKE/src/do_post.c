
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>


#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"

#define HOSTSTRING "../../../../../../../../../../../../etc/hosts%00"

void
usage()
{
  fprintf(stderr,"Usage: ./post_spike target port optional\r\n");
  exit(-1);
}


int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  char *url,*host;
  int port;
  char * optional;

  struct spike * our_spike;
  unsigned long retval;
  int i;

  if (argc!=4)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);
  
  if (argc>3)
    optional=argv[3];

  our_spike=new_spike();


  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }

  setspike(our_spike);

  memset(buffer,0x41,sizeof(buffer));
  buffer[sizeof(buffer)]=0;

  
  for (i=0; i<500; i+=4)
    {
      memcpy(buffer+i,"%25s",4);
    }

  /*  
Allow: OPTIONS, TRACE, GET, HEAD, DELETE, PUT, COPY, MOVE, PROPFIND, PROPPATCH, SEARCH, SUBSCRIBE, UNSUBSCRIBE, POLL, BDELETE, BCOPY, BMOVE, BPROPPATCH, BPROPFIND, LOCK, UNLOCK
  */
 
  buffer[140000]=0;

  printf("Buffer size = %d\r\n",strlen(buffer));

  host=strdup("target.com");
  url=strdup("/bob.jsp");
  /*takes a url, host, and posts it*/
  s_string("POST ");
  s_string(url);
  //  s_string_repeat("%n",15000);

  s_string(" HTTP/1.1\r\n");
  s_string("Host: ");
  s_string(host);
  s_string("\r\n");
  s_string("Authorization: Basic AWa1aaaabraaaaaaZaNz\r\n");
  s_string("Cookie: WebLogicSession=O41clZpkwpdYKbR0V3j37TOo4wjw9Pv7Qiswl3eZH3ZjxBYs2qxk|-6489024939146873433/-1408236330/6/7001/7001/7002/7002/7001/-1|659794130148480037/-1408236350/6/7001/7001/7002/7002/7001/-1\r\n");
  s_string("Referer: http://target.com/emailafriend.jsp\r\n");
  s_string("Content-type: application/x-www-form-urlencoded\r\n");
  s_string("Connection: Keep-Alive\r\n");
  s_string("User-Agent: Mozilla/4.76 [en] (X11; U; Linux 2.4.2-2 i686)\r\n");
  s_string("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\n");
  s_string("Accept-Encoding: gzip\r\n");
  s_string("Accept-Language: en\r\n"); 
  s_string("Accept-Charset: iso-8859-1,*,utf-8\r\n");
  s_string("Content-length: ");

  s_blocksize_string("post",7);
  s_string("\r\n\r\n");

  s_block_start("post");

  /*start post here*/


  /*END OF POST*/

  s_block_end("post");

  /*
  s_print_buffer();
  s_printf_buffer();
  */
  printf("Sending to %s on port %d\r\n",target,port);
  if (spike_send_tcp(target,port)<0)
    {
      printf("Couldn't connect to host or send data!\r\n");
      exit(-1);
    }

  printf("sleeping\r\n");
  sleep(2);
  printf("reading\r\n");

 retval=1;
  while(retval) 
  {
    memset(buffer,0x00,sizeof(buffer));

    retval=read(our_spike->fd,buffer,2500);
    if (retval)
      {
	printf("%s",buffer);
	sleep(2);
      }
  }
  return 0;
}







