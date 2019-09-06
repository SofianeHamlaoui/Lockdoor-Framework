/* Start webfuzzprelude.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"

//#define RTSP
//#define RTSP_DESCRIBE
//#define BODY
#define USEARGS
//#define WEBADMIN
#define BASIC_AUTH

/*change these to skip over variables*/
//#define SKIPVARIABLES 0
//#define SKIPFUZZSTR 0

void
usage()
{

  fprintf(stderr,"Example: ./closed localhost 80 POST /_vti_bin/ shtml .exe 0 0\n");
fprintf(stderr,"Don't forget the period before the extention.\n");
  fprintf(stderr,"Common Extentions are: .asa .asp .cdx .cer .htw .htr .ida .idc .idq .shtm .shtml .stm .printer .jsp .jhtml .bat .exe .com .gif .jpg .mpg .rma .wma .cfm .pl .py .pike, etc\n");
  fprintf(stderr,"Common methods are OPTIONS, TRACE, GET, HEAD, DELETE, PUT, POST, COPY, MOVE, MKCOL, PROPFIND, PROPPATCH, LOCK, UNLOCK, SEARCH, BROWSE\n");
  fprintf(stderr,"Try with both existing and NON existing files. Also try .dll with .exe and vice versa\n");
  fprintf(stderr,"Try ALL the extentions and go into your server settings windows and try all methods and all directories as well. If it crashes, ask for a refund.\n");
  fprintf(stderr,"Anything that says, 500 Server Error, also indicates a buggy web server. Might even be exploitable.\n");
  fprintf(stderr,"Likewise to anything like Error (not 0x80040e14, which MS uses for 'not an overflow' for some reason) caught while processing query. These are signs of serious problems in the web server.\n");
  fprintf(stderr,"Happy bug hunting!\n");
  fprintf(stderr,"contact Dave Aitel if you find anything cool. :> \n");

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
  char * file;
  char * directory;

  struct spike * our_spike;
  unsigned long retval;
  int notfin;
  char * extention;
  char * method;
  int firstfuzz;
  int fuzzvarnum,fuzzstrnum; /*for fuzz variable count*/
  int SKIPVARIABLES,SKIPFUZZSTR;

  if (argc!=9)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);

  method = argv[3];
  
  directory = argv [4];
  file = argv[5];

  extention=argv[6];

  SKIPVARIABLES=atoi(argv[7]);
  SKIPFUZZSTR=atoi(argv[8]);

  fuzzvarnum=0;
  fuzzstrnum=0;

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
  
  /*during s_variable push, if fuzzstring is == currentfuzzstring
    then set didlastfuzzstring. If fuzzvariable is == current
    variable, set didlastfuzzvariable*/


  
  /*zeroth fuzz variable is first variable*/
  s_resetfuzzvariable();
  /*
  fuzzvarnum=0;
  fuzzstrnum=0;
  */
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

	  /*reset this*/
	  /*controls when we put an ampersand or not*/
	  s_setfirstvariable();	  

          s_string_variable(method);
	  s_string(" ");
	  s_string(directory);
	  s_string_variable(file);
	  s_string(extention);

	  /*url arguments*/
#ifdef USEARGS
	  s_string("?");
	  s_string_variable("View");
	  s_string_variable("=");
	  s_string_variable("Logon");
#endif
#ifdef RTSP 
	  s_string(" RTSP/1.0");
#else
	  s_string(" HTTP/1.1");
#endif
          s_string_variable("");
	  s_string("\r\n");
#ifdef RTSP
	  //a simple per-request sequence number
	  s_string("CSeq: ");
	  s_string_variable("1");
	  s_string("\r\n");

	  s_string("Session: ");
	  s_string_variable("260778254-1");
	  s_string("\r\n");

	  s_string("PlayerStats: ");
	  s_string_variable("Stat3");
	  s_string(":");
	  s_string_variable("331");
	  s_string("|");
	  s_string_variable("0");
	  s_string("|");
	  s_string_variable("STOP");
	  s_string("|");
	  s_string(";][");
	  s_string_variable("Stat4");
	  s_string(":");
	  s_string_variable("0");
	  s_string(" ");
	  s_string("0 0 0");
	  s_string("|");
	  s_string_variable("0");
	  s_string("|");
	  s_string_variable("0");
	  s_string("|");
	  s_string(" 0 2]\r\n");

#endif
#ifndef RTSP
	  s_string("Referer: http://localhost/");
	  s_string_variable("bob");
	  s_string("\r\n");
	  s_string("Content-Type: ");
	  s_string_variable("application/x-www-form-urlencoded");
#ifdef XML
	  s_string_variable("application/xml");
#endif
	  s_string("\r\n");

	  s_string("Connection: ");
	  s_string_variable("close");
#ifdef WEBDAV
	  s_string_variable("TE");
#endif

	  s_string("\r\n");
#ifdef WEBDAV
	  s_string("TE: ");
	  s_string_variable("trailers");
	  s_string("\r\n");

	  s_string("Depth: ");
	  s_string_variable("0");
	  s_string("\r\n");
#endif
	  //Cookie: JSESSIONID=2CB3ED5F0D71E3C6CD504705BAFD67E0.tomcatinstance1 
	  s_string("Cookie: ");
#ifdef WEBADMIN
	  s_string_variable("User");
	  s_string("=");
	  s_string_variable("bob");
	  s_string("; Lang=");
	  s_string_variable("en");
	  s_string("; Theme=standard");
#endif
#ifdef TOMCAT
  	  s_string_variable("JSESSIONID");
	  s_string("=");
	  s_string_variable("B3ED5F0D71E3C6CD504705BAFD67E0");
	  s_string(".");
	  s_string_variable("tomcatinstance1");
#endif
	  s_string("\r\n");

#ifdef BASIC_AUTH
          s_string("Authorization: ");
          s_string_variable("Basic"); 
          s_string(" ");
          s_string_variable("QWxhZGRpbjpvcGVuIHNlc2FtZQ");
          s_string("==\r\n");
#endif

	  s_string("User-Agent: ");
	  s_string_variable("Mozilla/4.76 [en] (X11; U; Linux 2.4.2-2 i686)");
	  s_string("\r\n");
	  s_string_variable("Variable");
	  s_string(": ");
	  s_string_variable("result");
	  s_string("\r\n");
          s_string_variable(""); 
	  s_string("Host: ");
	  s_string_variable("localhost");
	  s_string("\r\n");
#endif

#ifdef BODY
	  s_string("Content-length: ");
	  s_string_variable("");
	  s_blocksize_unsigned_string_variable("post",7);
	  s_string("\r\n");
#endif

#ifdef RTSP
	  s_string("Accept: application/sdp");
#else
	  s_string("Accept: ");
	  s_string_variable("image/");
	  s_string_variable("gif");
	  s_string(", image/x-xbitmap, image/jpeg, image/pjpeg, image/png");
#endif
	  s_string("\r\n");
#ifdef RTSP
#ifdef RTSP_DESCRIBE
	  s_string("Bandwidth: ");
	  s_string_variable("393216");
	  s_string("\r\n");

	  s_string("ClientID: ");
	  s_string_variable("WinNT_5.1_6.0.11.868_RealPlayer_RN10PD_e-us_UNK");
	  s_string("\r\n");

	  s_string("RegionData: ");
	  s_string_variable("10034");
	  s_string("\r\n");
	  s_string("Require: ");
	  s_string_variable("com.real.retain-entity-for-setup");
	  s_string("\r\n");

	  s_string("SupportsMaximumASMBandwidth: ");
	  s_string_variable("1");
	  s_string("\r\n");

	  s_string("ClientChallenge: ");
	  s_string_variable("deee2996aca6c64db4ff59e0e3fb386f");
	  s_string("\r\n");

	  s_string("CompanyID: ");
	  s_string_variable("nB9UbGcLzuKoS++5MTGHIg");
	  s_string("==\r\n");

	  s_string("GUID: ");
	  s_string_variable("00000000-0000-0000-0000-000000000000");
	  s_string("\r\n");

	  s_string("Pragma: ");
	  s_string_variable("initiate-session");
	  s_string("\r\n");
#endif
#endif
#ifndef RTSP
		  
	  s_string("Accept-Encoding: ");
	  s_string_variable("gzip");
	  s_string("\r\n");
	  s_string("Accept-Language: ");
	  s_string_variable("en");
	  s_string("\r\n");
	  s_string("Accept-Charset: ");
	  s_string_variable("iso-8859-1,*,utf-8");
	  s_string("\r\n");

#endif
	  s_string("\r\n");
	  /*Done with Headers*/
	  s_block_start("post");
	  /*begin POST block*/
	  s_setfirstvariable();
#ifdef BODY
	  s_string_variables('&',"User=bob&Password=foo&languageselect=en&Theme=Heavy&Logon=Sign+In\r\n\r\n ");
#endif
#ifdef XML
	  s_string("<?xml version=\"1.0\"?>\n");
	  s_string("<g:searchrequest xmlns:g=\"DAV:\">\n");
	  s_string("<g:sql>\n");
	  s_string("SELECT \"DAV:");
	  s_string_variable("");
	  s_string("displayname\" from scope()\n");
	  s_string("</g:sql");
	  s_string_variable("");
	  s_string(">\n");
	  s_string("</g:searchrequest>");
#endif

	  /*
	  s_string("username=");
	  s_string_variable("");
	  s_string_repeat("A",500);
	  */
	  s_block_end("post");
	  /* Start webfuzzpostlude.c */ 


	  if (spike_send_tcp(target,port)==0)
	    {
	      /*this whole block is a bit wrong. Really we
		need to exit or something.*/
	      printf("Couldn't connect to host or send data!\r\n");
	      spike_close_tcp();
	      if (fuzzstrnum==s_get_max_fuzzstring())
		{
		  break;
		}

	      fuzzstrnum++;
	      s_incrementfuzzstring();
	      //sleep(5);
	      continue;
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
	  spike_close_tcp();
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


