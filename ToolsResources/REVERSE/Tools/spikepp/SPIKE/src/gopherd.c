/* gopherd.c - finding that pesky IE hole

   This file implements a very basic listening TCP server. It's a good
   demo, if you're looking for one. Otherwise, you just connect to it with
   IE (gopher://bob:port/<request>). <request> can be eithar nothing, 
   or dictated by http://www.faqs.org/rfcs/rfc1436.html,
    0   Item is a file
    1   Item is a directory
    2   Item is a CSO phone-book server
    3   Error
    4   Item is a BinHexed Macintosh file.
    5   Item is DOS binary archive of some sort.
        Client must read until the TCP connection closes.  Beware.
    6   Item is a UNIX uuencoded file.
    7   Item is an Index-Search server.
    8   Item points to a text-based telnet session.
    9   Item is a binary file!
        Client must read until the TCP connection closes.  Beware.
    +   Item is a redundant server
    T   Item points to a text-based tn3270 session.
    g   Item is a GIF format graphics file.
    I   Item is some kind of image file.  Client decides how to display.

    Hence, gopher://bob:7007/2 generates an interesting error, and so
    on. For 7, you are going to want to do something like gopher
    /7?asdfasdf. And so on and so forth. 

    TO USE: edit the line where I marked "edit this" (if you want to bother)
      	then connect to the server with your IE, and hit reload until your
	fingers bleed.

    I was actually unable to find the IE "overflow," although IE jumped into
    infinite loops quite a number of times.


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

struct spike * our_spike;

void
usage()
{
  printf("Usage: ./gopherd port\r\n");
  printf("./gopherd 70\n");
  exit(-1);
}



int
main (int argc, char ** argv)
{
  int port;
  int result,listenfd,acceptfd;
  
  if (argc!=2)
    {
      usage();
    }
  
  port=atoi(argv[1]);
  
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
	  
/*change any of the s_string's into s_string_variable for extra
 * fun */
	  s_read_packet(); /*read a 0d 0a */
	  s_string("2"); /*EDIT THIS*/
	  //s_string_variable("");
	  s_string_repeat("asdf",5);
	  s_string("\t");
	  s_string("2"); /*AND THIS*/
	  s_string("/");
	  s_string_repeat("asdf",5);
	  s_string_variable("");
	  s_string("\t");
	  s_string("localhost");
	  s_string(".");
	  s_string("localdomain");
	  s_string(".");
	  s_string("com");
	  s_string("\t");
	  s_string("70");
	  s_string("\t");
	  s_string("-");
	  s_string("\r\n");
	  s_string(".\r\n");
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








