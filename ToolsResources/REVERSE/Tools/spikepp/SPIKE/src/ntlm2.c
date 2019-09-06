/*
   this is NTLM-brute - a generic password guesser.
   It's just a shell for now. Eventually I'll get it working.
   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/*for memset */
#include <sys/types.h>
#include <sys/socket.h>
#include <ntlm.h>
#include <signal.h>


#include "spike.h"
#include "base64.h"
#include "hdebug.h"
#include "tcpstuff.h"

#define METHOD "POST"

/*global variables */
char *url, *host;

void
usage ()
{
  fprintf (stderr,
	   "Usage: ./ntlm_brute target port username[@domain] password url\r\n");
  fprintf (stderr,
	   "If you get a nice message from the server saying Use Localhost only, then you got the right password. Use a shell script to spam this througha word list or something. \n");
  exit (-1);
}

void do_body ();

int
send_ntlm_packet (unsigned char *packet, char *method)
{

  /*takes a url, host, and posts it */
  s_string (method);
  s_string (" ");
  s_string (url);

  s_string (" HTTP/1.1\r\n");
  s_string ("Host: ");
  s_string (host);
  //s_string ("localhost");
  s_string ("\r\n");
  s_string ("Authorization: NTLM ");
  s_string (packet);
  s_string ("\r\n");

  s_string ("Connection: Keep-Alive\r\n");
  //s_string ("User-Agent: Mozilla/4.76 [en] (X11; U; Linux 2.4.2-2 i686)\r\n");
  s_string
    ("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\n");
  //s_string ("Accept-Encoding: \r\n");
  s_string ("Accept-Language: en\r\n");
  s_string ("Accept-Charset: iso-8859-1,*,utf-8\r\n");
  s_string ("Content-Type: application/x-www-form-urlencoded\r\n");
  s_string ("Content-Length: ");
  s_blocksize_string ("post", 7);
  s_string ("\r\n\r\n");
  s_block_start ("post");
  do_body ();			/*here is the magic body */
  s_block_end ("post");
  if (spike_send () == 0)	/*failure */
    {
      printf ("Couldn't connect to host or send data!\r\n");
      return 0;
    }

  printf("sent: \n%s\n",getcurrentspike()->databuf);

  return 1;
}


/*global spike thingy*/
struct spike *our_spike;

/* does both calls*/
int
get_ntlm_page (char *url, char *user, char *password, char *domain,
	       char *responsebuf)
{
  tSmbNtlmAuthRequest request;
  char buf[15000], *response;
  unsigned char buf2[5000];
  unsigned long retval;
  int notfin;
  memset((char*)&request,0,sizeof(request));
  memset(buf,0,sizeof(buf));
  memset(buf2,0,sizeof(buf2));
  memset(&request,0x00,sizeof(request));

  /*first we request to authorize via NTLM */
  buildSmbNtlmAuthRequest ((tSmbNtlmAuthRequest *) & request, user, domain);
  /*go to base64 - need to integrate this into SPIKE proper */
  to64frombits (buf, (unsigned char *) &request, SmbLength (&request));
  /*throw that in to NTLM Auth: */

  send_ntlm_packet (buf, METHOD);
  spike_clear ();
  memset(buf,0,sizeof(buf));

  printf ("reading server response\n");

  /*now we should have gotten a valid response from the server */
  notfin = 1;
  retval = 1;
  response=NULL;
  while (retval && notfin)
    {
      memset (buf, 0x00, sizeof (buf));
      notfin = s_fd_wait ();
      if (!notfin)
	break;
      retval = read (our_spike->fd, buf, 2500);
      if (retval)
	{
	  /*here we look for the string "WWW-Authenticate: NTLM " followed
	     *             by a base64 encoded value which is deliminated by a \r\n" */
	  if (response == 0)
	    {
	      if ((response = s_scan_for_variable (buf,
						   "WWW-Authenticate: NTLM ",
						   "\r\n")) != NULL)
		{
		  /*found our string */
		  printf ("Found our WWW-auth string\n");
		  //break;
		}
	      /*printf("%s",buffer); */
	    }
	}
      else
	{
	  break;
	}
    }				/*end while read loop */
  if (response == NULL)
    {
      printf ("Couldn't find WWW-Authenticate string!\n");
      return (0);

    }

  /*Grab the NTLM AUTH: response */
  from64tobits (buf, response);
  buildSmbNtlmAuthResponse ((tSmbNtlmAuthChallenge *) buf,
			    (tSmbNtlmAuthResponse *) buf2, user, password);
  to64frombits (buf, buf2, SmbLength ((tSmbNtlmAuthResponse *) buf2));
  /*send that out */
  send_ntlm_packet (buf, METHOD);
  spike_clear ();

  /*now we should have gotten a valid response from the server */
  /*hopefully this will have 200 Ok */
  responsebuf[0] = 0;		/*clear this buffer */
  notfin = 1;
  retval = 1;
  while (retval && notfin)
    {
      memset (buf, 0x00, sizeof (buf));
      notfin = s_fd_wait ();
      if (!notfin)
	break;
      retval = read (our_spike->fd, buf, 2500);
      buf[2500] = 0;
      if (retval)
	{
          if (strlen(responsebuf)+strlen(buf)>2500)
		  return 1;
	  strcat (responsebuf, buf);
	}
    }				/*end while read loop */

  return 1;
}

void
do_body ()
{
/*put your body to the BEAT baby!*/
 s_setfirstvariable ();
}

int
main (int argc, char **argv)
{
  char *target;
  char buffer[1500000];

  int port;

  unsigned char *user, *domain;
  unsigned char *password;

  if (argc != 6)
    {
      usage ();
    }

  target = argv[1];
  printf ("Target is %s\r\n", argv[1]);

  port = atoi (argv[2]);

  our_spike = new_spike ();
  s_init_fuzzing ();
  /*sheesh. */
  signal (SIGPIPE, SIG_IGN);




  if (our_spike == NULL)
    {
      fprintf (stderr, "Malloc failed trying to allocate a spike.\r\n");
      exit (-1);
    }

  setspike (our_spike);
  host = strdup ("localhost");
  /*url=strdup("/iisadmin/iis.asp"); */
  url = strdup (argv[5]);
  memset (buffer, 0x41, sizeof (buffer));
  buffer[sizeof (buffer)] = 0;

  buffer[140000] = 0;

  user = strdup (argv[3]);
  domain = NULL;		/*set domain with user@domain */
  password = strdup (argv[4]);

  s_resetfuzzvariable ();
  while (!s_didlastvariable ())
    {
      s_resetfuzzstring ();
      /*zeroth fuzz string is no change */

      while (!s_didlastfuzzstring ())
	{
	  spike_clear ();
	  spike_connect_tcp (target, port);
	  printf ("Connected.\n");
	  memset(buffer,0x00,sizeof(buffer));

	  printf("Getting page %s as %s:%s@%s\n",url,user,password,domain);
	  if (!get_ntlm_page (url, user, password, domain, buffer))
	    printf ("Couldn't get ntlm page\n");
	  else
	    {
	      printf ("Reponse: %s\n", buffer);
	      printf ("\nEnd of response\n");
	    }
	  printf("Closing socket\n");
	  spike_close_tcp ();
	 // sleep(5);
s_incrementfuzzstring();

	  

	}
        s_incrementfuzzvariable();
    }
  return 0;
}
