/* form-fuzz.c  -  Useful for testing the forms accepting implementation
of a particular web server or application server.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>


#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"


void
usage()
{
  fprintf(stderr,"Usage: ./post_spike target port\r\n");
  exit(-1);
}


int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  char *url,*host;
  int port;

  struct spike * our_spike;
  unsigned long retval;
  int i;

  if (argc!=3)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);

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

  host=strdup("10.3.8.25");
  url=strdup("/exchange/bob/Drafts/No%20Subject-4.EML/?Cmd=addattach&Embedded=0");
  /*takes a url, host, and posts it*/
  s_string("POST ");
  s_string(url);
  //  s_string_repeat("%n",15000);

  s_string(" HTTP/1.1\r\n");
  s_string("Host: ");
  s_string(host);
  s_string("\r\n");
  s_string("Authorization: Basic Ym9iOmJvYg==\r\n");
  //s_string("Cookie: sessionid=b1c4d131-9439-4a7b-894f-7185ca8c6695,0x9\n");
  
  s_string("Cookie: sessionid=03570e97-62ab-4372-bd98-f7faac2e2619,0x409; sessionid=374642e5-8427-4be5-9bb2-1d90c01ed28c,0x409; ASPSESSIONIDQQGQQNOC=OIJHFNJBFLCAAPHINMAJGHJM\r\n");

  s_string("Referer: http://10.3.8.25/exchange/bob/Drafts/No%20Subject-4.EML?Cmd=editattach\r\n");
  s_string("Connection: Keep-Alive\r\n");
  s_string("User-Agent: Mozilla/4.76 [en] (X11; U; Linux 2.4.2-2 i686)\r\n");
  s_string("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\n");
  s_string("Accept-Encoding: gzip\r\n");
  s_string("Accept-Language: en\r\n");
  s_string("Accept-Charset: iso-8859-1,*,utf-8\r\n");
  s_string("Content-type: multipart"); 

  s_string("/form-data; boundary=---------------------------42423833519577477931714636915");
  
  s_string("\r\n");

  s_string("Content-length: ");

  s_blocksize_string("post",7);
  s_string("\r\n\r\n");

  s_block_start("post");

  /*start post here*/
  s_string("-----------------------------42423833519577477931714636915");
  s_string("\r\n");
  s_string("Content-Disposition: ");
  s_string_repeat("A",500000);  
  s_string("form-data; name=\"attachFile");

  

  s_string("\"; filename=\"/tmp/asdf");
  s_string("\"\r\n");
  s_string("Content-Type: ");

  s_string("application/octet-stream\r\n");

  s_string("Content-Transfer-Encoding: ");

  s_string("binary\r\n");
  s_string("\r\nDAVEROCKS\r\n");
  s_string("-----------------------------42423833519577477931714636915--\r\n");
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
  sleep(4);
  printf("reading\r\n");
  memset(buffer,0x00,sizeof(buffer));

  retval=read(our_spike->fd,buffer,1500);
  printf("Buffer[%d]=*%s*\r\n",retval,buffer);
  return 0;
}







