/*
  webmitm.c

  HTTP / HTTPS monkey-in-the-middle.

  Copyright (c) 2000 Dug Song <dugsong@monkey.org>

  Modified (as was allowed by license) by Dave Aitel to do something 
  different and more fun.

  Previously modified by Kevin Dunn as well.

NOTES:
My modified version has been reported to not compile on Redhat 7.0. I use Redhat 7.1, and SSL
is notoriously annoying to compile. Sorry about that. :>


// D: This is so that you can type "./webmitm -i
// www.customer.com -p 80 -s 443 -L 80 -S 443 -Q 127.0.0.1 ;
// and it will redirect all output from port 80 to the
// target's port 80, and all output from port 443 to localhost
// port 443 where you have an stunnel listening to forward the
// actual ssl traffic. This will save all the stuff we send to
// eithar port in nice separate files for us, and do SSL properly
// and do Host: rewriting.

// In the end, the user has to set up webmitm with the correct IP and
// virtual host, and stunnel and then simply use their web browser and
// hit http://localhost/ and browse around. This will generate plenty
// of files that can then be used as SPIKE input quickly and easily

// Also we will have a script that will make it all painless, setting up
// the defaults correctly for the user and loading stunnel

// GOAL: 
// ./dowmmitm.sh www.customer.com # assumes port 80 and 443 and sets up stunnel
                                 # "hit enter when done" then kills stunnel 
				 # and webmitm
// netscape http://localhost/
// browse around for a bit then hit enter on dowmitm.sh
// ./odbcwebfuzz.sh www.customer.com 
                              # creates directory www.customer.com, fuzzes
			      # and produces output
// grep "ODBC" www.customer.com/ *.out
// as easy as that! 


ToDO:
Make sure we output into the files the actual stuff we send to the server
after switching for the correct Host: string.

Switch SSL stuff around so we don't do ssl writes anymore. (But we still
do reads)

Figure out how to make it compile on 7.0?

write ./dowmitm.sh





*/

#define DOVIRTUALHOST


#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
//doesn't exist in BSD?
//#include <openssl/err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h> /*for redhat 7.0 var_args*/

#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*from TCPSTUFF.H*/
#include <unistd.h> /*for all sorts of stuff*/
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> /*for gethostbyname()*/
#include <netinet/in.h> /*htonl()*/
#include <unistd.h> /*close*/
#include <stdlib.h> /*malloc*/
#include <stdio.h> /*printf*/
/*other inet_ stuff*/
#include <sys/socket.h>
#include <arpa/inet.h>
/*End FROM TCPSTUFF.H*/


#include "buf.h"
/*
#include "record.h"
#include "version.h"
*/

#define CERT_FILE       "webmitm.crt"

int      Opt_quiet = 1;
int      Opt_debug = 0;
int      Opt_read = 0;
int      Opt_write = 0;
int      Opt_dns = 1;

unsigned int      file_number=0;
unsigned int      request_number=0;

int      http_fd, https_fd;
int      client_fd, server_fd;
SSL_CTX *ssl_client_ctx, *ssl_server_ctx;
SSL     *ssl_client, *ssl_server;
struct   sockaddr_in csin, ssin;
int      do_ssl, sig_pipe[2];


/*globals from main() */
int localhttpport=80;
int localhttpsport=443;
int port=80;
char *ip;
char *listenaddr = NULL;
char *cookie = NULL;
char *virtualhost = NULL;
char *sslhost;
int sslport=1443;



extern int decode_http(char *, int, char *, int);

void
usage(void)
{
  
  fprintf(stderr,"Usage: ");
  fprintf(stderr,"This version of webmitm requires stunnel to be running for ssl.\n");
  fprintf(stderr,"Sadly, openssl programming was causing some ssl servers to fail to connect...\n");
  fprintf(stderr,"./webmitm -t targetip/hostname -p targethttpPort{80} \n");
  fprintf(stderr,"\t-s targetsslport{1443} -S targetsslhost{localhost} -v virtualhost [-d]\n");
  fprintf(stderr,"\t-l localhttpport{80} -L localhttpsport{443}\n");
  fprintf(stderr,"example: stunnel  -r www.hackthis.net:443  -c -d 127.0.0.1:2443&\n");
  fprintf(stderr,"\t./webmitm -t www.hackthis.net -s 2443\n");
  fprintf(stderr,"-v is assumed to be the same as -t if not specified\n");
  fprintf(stderr,"Check out README.webmitm\n");


  fprintf(stderr, "See DUGSONGLICENSE.txt and LICENSE.txt for license info.\n");
  exit(1);
}

void
warnx(const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
        if (fmt != NULL)
                (void)vfprintf(stderr, fmt, ap);
        (void)fprintf(stderr, "\n");
        va_end(ap);
}

int
getHostAddress(const char *host, struct sockaddr_in *addrP)
{
    struct hostent *entry = NULL;

    if ((entry = gethostbyname(host)) == NULL)
    {
        if ((addrP->sin_addr.s_addr = htonl(inet_addr(host))) == 0xffffffff)
        {

            return 0;
        }
    }
    else
    {
        memcpy(&(addrP->sin_addr), entry->h_addr, entry->h_length);
    }

    return 1;
}



static void
sig_chld(int signal)
{
        if (write(sig_pipe[1], "x", 1) < 0)
                warn("sig_chld");

}

static void
sig_int(int signal)
{
        close(http_fd);
        close(https_fd);
        /*record_close();*/
        exit(0);
}

static void
reap_child(void)
{
        pid_t pid, status;

        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
                if (Opt_debug)
                        warnx("child %d terminated with status %d",
                              pid, status);
        }
}

static void
err_ssl(int eval, char *msg)
{
        char buf[128];
//Doesn't exist in BSD?
        //ERR_error_string(ERR_get_error(), buf);
        err(eval, "%s", buf);
}



static void
cert_init(void)
{
        struct stat sb;

        /* XXX - i am cheap and dirty */

        if (stat(CERT_FILE, &sb) < 0) {
                if (system("openssl genrsa -out " CERT_FILE " 1024") != 0)
                        err(1, "system");
                if (system("openssl req -new -key " CERT_FILE " -out "
                           CERT_FILE ".csr") != 0)
                        err(1, "system");
                if (system("openssl x509 -req -days 365 -in " CERT_FILE ".csr"
                           " -signkey " CERT_FILE " -out " CERT_FILE ".new"))
                        err(1, "system");
                if (system("cat " CERT_FILE ".new >> " CERT_FILE) != 0)
                        err(1, "system");

                unlink(CERT_FILE ".new");
                unlink(CERT_FILE ".csr");

                warnx("certificate generated");
        }
}

static void
client_init()
{
        if (fcntl(client_fd, F_SETFL, 0) < 0)
                err(1, "fcntl");

        if (do_ssl) {
                ssl_client = SSL_new(ssl_client_ctx);
                SSL_set_fd(ssl_client, client_fd);

                if (SSL_accept(ssl_client) == 0)
                        err_ssl(1, "SSL_accept");
        }
}

static int
client_read(char *buf, int size)
{
  int ret;
  if (do_ssl) {
    return (SSL_read(ssl_client, buf, size));
  }
  ret = read(client_fd, buf, size);
  return ret;
}

/*this function theoretically grabs the entire client request - first line SHOULD be the GET/POST/PROPFIND . . . HTTP/1.1\r\n. And the rest should be here too*/
static int
client_request(char *buf, int size)
{
        struct buf *b, req;
        char *p;
        int i, reqlen;

        memset(&req, 0, sizeof(req));
	memset(buf,0,size); //sheesh
        req.base = buf;
        req.size = size;
        reqlen = 0;

        /* XXX - i feel cheap and dirty */
        while ((i = client_read(req.base + req.end, req.size - req.end)) > 0) {
                req.end += i;

                if (reqlen && buf_len(&req) >= reqlen) {
			warnx("Breaking 1\n");
                        break;
                }
                else if ((i = buf_index(&req, "\r\n\r\n", 4)) > 0) {
                        reqlen = i + 4;
                        b = buf_tok(&req, NULL, reqlen);
                        buf_rewind(&req);

/*original line didn't work if the
 * client was using "Content-Length: " instead of 
 * Content-length:, so we had to fix that here 
 * */
			
                        if ((i = buf_index(b, "\r\nContent-length: ", 18)) < 0)
			{
				if ((i = buf_index(b, "\r\nContent-Length: ", 18)) < 0)
				{
					warnx("Breaking since we did not find Content-length\n");
                                	break;
				}
			}

                        buf_skip(b, i + 18);
                        b = buf_getword(b, "\r\n", 2);
                        p = buf_strdup(b); buf_free(b);
                        reqlen += atoi(p); free(p);
			warnx("reqlen=%d, buf_len=%d\n",reqlen,buf_len(&req));

			/* This condition is hit when Content-Length: 0 */
			/* REVIEW: do we need to compensate for extra \r\n?
			 * */
			if (reqlen==buf_len(&req))
			{
				warnx("Breaking because of Content-Length 0\n");
				break;
			}
                }
        }
        reqlen = buf_len(&req);
	warnx("final reqlen=%d\n",reqlen);

        /*
        if (Opt_debug && reqlen > 0)
                fprintf(stderr, "%.*s", reqlen, buf);
        */
        return (reqlen);
}

static int
client_write(char *buf, int size)
{
  int ret;
  if (do_ssl) {
    return (SSL_write(ssl_client, buf, size));
  }
  ret = write(client_fd, buf, size);
  return ret;
}

static void
client_close(void)
{
        if (do_ssl) {
                SSL_free(ssl_client);
        }
        close(client_fd);
}


static void
server_init()
{
  /*
        struct buf *word, msg;
        char *vhost;
        int i;

        buf_init(&msg, buf, size);

        //
        // change to connect to command line arg -kd
        //
	// D: This way we can only proxy to one target at a time, but
	// we can do rewriting of each request as it goes through
	// to give it the correct Host: line



        if ((i = buf_index(&msg, "\r\nHost: ", 8)) < 0) {
                errx(1, "no virtual host in request");
        }
        buf_skip(&msg, i + 8);
        word = buf_tok(&msg, "\r\n", 2);
        vhost = buf_strdup(word);
    */

        memset(&ssin, 0, sizeof(ssin));
        ssin.sin_family = AF_INET;

        // ssin.sin_port = do_ssl ? htons(443) : htons(80);
        // ssin.sin_addr.s_addr = libnet_name_resolve(vhost, 1);

	if (do_ssl)
	  {
	    if (!getHostAddress(sslhost,&ssin))
	      {
		fprintf(stderr,"Could not get host address!\n");
		return;
	      }
	  }
	else
	  {
	    if (!getHostAddress(ip,&ssin))
	      {
		fprintf(stderr,"Could not get host address!\n");
		return;
	      }

	  }

	if (do_ssl)
	  ssin.sin_port = htons(sslport);
	else
	  ssin.sin_port = htons(port);

	/*   
     ssin.sin_addr.s_addr = libnet_name_resolve(ip, 0);
	*/


        //      if (ssin.sin_addr.s_addr == ntohl(INADDR_LOOPBACK))
        //errx(1, "recursion considered harmful");

        //      free(vhost);

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                err(1, "socket");

        if (connect(server_fd, (struct sockaddr *)&ssin, sizeof(ssin)) < 0)
                err(1, "connect");

	/*
        if (do_ssl) {
                ssl_server_ctx = SSL_CTX_new(SSLv23_client_method());
                ssl_server = SSL_new(ssl_server_ctx);
                SSL_set_connect_state(ssl_server);

                SSL_set_fd(ssl_server, server_fd);

                if (SSL_connect(ssl_server) < 0)
                        err_ssl(1, "SSL_connect");
        }
	*/
}

static int
server_read(char *buf, int size)
{
  /*
        if (do_ssl) {
          return (SSL_read(ssl_server, buf, size));
        }
  */
  //  warnx("in server_read\n");
        return (read(server_fd, buf, size));
}

/*messed with this so we never do ssl writes - instead we use stunnel for 
  that - it's much more reliable*/
static int
server_write(char *buf, int size)
{
  // warnx("in server_write serverfd=%d size=%d buf=\n%s\n",server_fd,size,buf);
  /*
        if (do_ssl) {
          return (SSL_write(ssl_server, buf, size));
        }
  */
        return (write(server_fd, buf, size));
}

static void
server_close(void)
{
  /*
        if (do_ssl) {
                SSL_free(ssl_server);
        }
  */
        close(server_fd);
}

void
mitm_init(int port, char *listenaddr)
{
        struct sockaddr_in sin;
        int i = 1;

        if (pipe(sig_pipe) < 0)
                err(1, "pipe");

        if ((http_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ||
            (https_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                err(1, "socket");

        if (setsockopt(http_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0 ||
            setsockopt(https_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
                err(1, "setsockopt");

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        if (listenaddr == NULL) 
          listenaddr = "0.0.0.0"; /*was 127.0.0.1 */

	if (!getHostAddress(listenaddr,&sin))
	  {
	    fprintf(stderr,"Could not get host address!\n");
	    return;
	  }

	/*
	  sin.sin_addr.s_addr = libnet_name_resolve(listenaddr, 1);
	*/

        sin.sin_port = htons(localhttpport);
        if (bind(http_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
                err(1, "bind");

        sin.sin_port = htons(localhttpsport);
        if (bind(https_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
                err(1, "bind");

        if (listen(http_fd, 3) < 0 || listen(https_fd, 3) < 0)
                err(1, "listen");

        SSL_library_init();
        SSL_load_error_strings();

        ssl_client_ctx = SSL_CTX_new(SSLv23_server_method());

        if (SSL_CTX_use_certificate_file(ssl_client_ctx, CERT_FILE,
                                         SSL_FILETYPE_PEM) == 0)
                err_ssl(1, "SSL_CTX_use_certificate_file");

        if (SSL_CTX_use_PrivateKey_file(ssl_client_ctx, CERT_FILE,
                                        SSL_FILETYPE_PEM) == 0)
                err_ssl(1, "SSL_CTX_use_PrivateKey_file");

        if (SSL_CTX_check_private_key(ssl_client_ctx) == 0)
                err_ssl(1, "SSL_CTX_check_private_key");
}

int bout_put (char **buf, int buflen, char *ins, int inslen) {
  /*
    This function is the add-on buffering dillio. It's necessary to 
    buffer requests and responses because of forking and keep-alives 
    would otherwise make a mess of stdout. -kd
   */
  if (strlen(*buf) + inslen >= buflen) {
    buflen = buflen + inslen + 1;
    *buf = (char *)realloc(*buf, (buflen * sizeof(char)));
  }
  /*originally this was strncat - we need memcpy*/
  /*hmm, actually this whole file needs to be rewritten to handle nulls*/
  strncat(*buf, ins, inslen);
  return (buflen);
}

/*this function added by Dave Aitel to be more flexible, based off of the 
  SetCookie() stuff*/
/*targetstring is "Host: " including space.
  headerstring is input
  replacewith is www.bob.com
  newString is output
  returns 
  -1 if there was no targetstring
  -1 if headerString doesn't end with a \r
  -1 if we cannot malloc a new buffer for newString
  else
  0 on success

BUGS:
Returns same error code for all errors.
*/
int SetString(char * targetstring, char  * headerString, char * replacewith, 
	      char ** newString) 
{
 char * oldString = NULL;
  char * oldStringEnd = NULL;
  int oldStringLen = 0;
  int replacewithLen = 0;
  int newStringSize = 0;

  /* See if it's there */
  oldString = strstr(headerString, targetstring);

  if (oldString == NULL) {
    return(-1);
    /* No string found... insert it! */
  } else {
    /* Change the existing cookie */
    oldString += strlen(targetstring);

    oldStringEnd = strstr(oldString, "\r\n");
    oldStringLen = oldStringEnd - oldString;

    if (oldStringEnd == NULL) {
      return(-1);
    }

    replacewithLen = strlen(replacewith);
    newStringSize = strlen(headerString) + (replacewithLen - oldStringLen);

    *newString = (char*) malloc(newStringSize + 1);
    if (*newString == NULL) {
      return(-1);
    }
    memcpy(*newString, headerString, oldString - headerString);
    memcpy(*newString + (oldString - headerString), replacewith, 
	   replacewithLen);
    memcpy(*newString + (oldString - headerString) + replacewithLen,
           oldStringEnd,
           strlen(oldStringEnd) + 1);
  }
  return(0);

}

int SetCookie(char * headerString, char * newCookie, char ** newString) {
  char * oldCookie = NULL;
  char * oldCookieEnd = NULL;
  int oldCookieLen = 0;
  int newCookieLen = 0;
  int newStringSize = 0;

  /* See if it's there */
  oldCookie = strstr(headerString, "Cookie: ");

  if (oldCookie == NULL) {
    return(-1);
    /* No cookie... insert it! */
  } else {
    /* Change the existing cookie */
    oldCookie += strlen("Cookie: ");

    oldCookieEnd = strstr(oldCookie, "\r");
    oldCookieLen = oldCookieEnd - oldCookie;

    if (oldCookieEnd == NULL) {
      return(-1);
    }

    newCookieLen = strlen(newCookie);
    newStringSize = strlen(headerString) + (newCookieLen - oldCookieLen);

    *newString = (char*) malloc(newStringSize + 1);
    if (*newString == NULL) {
      return(-1);
    }
    memcpy(*newString, headerString, oldCookie - headerString);
    memcpy(*newString + (oldCookie - headerString), newCookie, newCookieLen);
    memcpy(*newString + (oldCookie - headerString) + newCookieLen,
           oldCookieEnd,
           strlen(oldCookieEnd) + 1);
  }
  return(0);
}

/*this function writes whatever we get to a nice new file labeled
  semi-clearly for later analysis*/
void
write_new_file(unsigned char * bout, unsigned int len)
{
  u_char outfilename[8192];
  FILE *fout;

  /*first we set up the filename we're going to use*/
  if (do_ssl)
    sprintf(outfilename,"https_request-%u.%u",file_number,request_number);	  
  else
    sprintf(outfilename,"http_request-%u.%u",file_number,request_number);


  /*next we open the file*/
  fout=fopen(outfilename,"w");
  if (fout==NULL)
    {
      printf("Can't seem to open %s\n!",outfilename);
      exit(-1);
    }

  fwrite(bout,1,len,fout);

  request_number++;
  fclose(fout);
  /*easy peasy*/
}

void
mitm_child()
{
        u_char buf[81920];
	
        fd_set fds;
        int i;
        int boutlen = 1000;
        // int bout2len = 1000;
	//        char *sep;
        char *bout;
	char *bufcopy;
	int j;

        bout = (char *)malloc(boutlen * sizeof(char));

        memset(bout, '\0', boutlen * sizeof(char));
	//        memset(bout2, '\0', boutlen * sizeof(char));





	/*let's not clutter up the works with anything but the actual POSTS and GETS */
	/*
	  sep = "\n=====================\n";
	  boutlen = bout_put(&bout, boutlen, sep, strlen(sep));
	*/

        if (Opt_debug)
                warnx("new connection from %s.%d",
                      inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));

        client_init(ip, port);

	memset(buf,0,sizeof(buf));
        if ((i = client_request(buf, sizeof(buf))) < 0)
                err(1, "client_request");

        server_init();

        // replace the cookie in the client request
	/*D: Let's not and say we didn't.*/
	/*
        if (cookie != NULL) {
	// D: Theoretically this is equiv to SetString("Cookie: " ...
          if (SetCookie(buf, cookie, &bufcopy) == 0) {
            for (j = 0; j < strlen(bufcopy) + 1; j++)
              buf[j] = bufcopy[j];
          }
          free(bufcopy);
        }
	*/

        /*D: But we do need to replace virtual hosts now*/
#ifdef DOVIRTUALHOST
	if (virtualhost != NULL) 
	  {
	    if (SetString("Host: ",buf,virtualhost,&bufcopy) == 0)
	      {
		/*copy it over if there was success...*/
		for (j = 0; j < strlen(bufcopy) + 1; j++) {
		  buf[j] = bufcopy[j];
		}
		free(bufcopy);
	      }
	    i=strlen(bufcopy);
	  }
#endif
        boutlen = bout_put(&bout, boutlen, buf, i);

	write_new_file(buf,i);

	/*i is the length of the client buffer*/
        if (server_write(buf, i) != i)
                err(1, "server_write");


        for (;;) {
                FD_ZERO(&fds);
                FD_SET(client_fd, &fds);
                FD_SET(server_fd, &fds);

                i = MAX(client_fd, server_fd) + 1;
                if (select(i, &fds, 0, 0, 0) < 0) {
                        if (errno != EINTR)
                                break;
                }

                if (FD_ISSET(client_fd, &fds)) {
			/*anything else we get we send out too*/
                        i = sizeof(buf);
			/*this will read the entire client request*/
			memset(buf,0,sizeof(buf));
                        if ((i = client_request(buf, i)) <= 0)
                          break;

#ifdef DOVIRTUALHOST
        if (virtualhost != NULL)
          {
            if (SetString("Host: ",buf,virtualhost,&bufcopy) == 0)
              {
                /*copy it over if there was success...*/
                for (j = 0; j < strlen(bufcopy) + 1; j++) {
                  buf[j] = bufcopy[j];
                }
                free(bufcopy);
              }
            i=strlen(bufcopy);
          }
#endif


                        // replace the cookie in the client request
			/*ick -dave*/
			/*hmm, I think actually this does something
                          else entirely from what the above comment
                          says.*/
			/*
                        boutlen = bout_put(&bout, boutlen, sep, strlen(sep));
			*/

                        boutlen = bout_put(&bout, boutlen, buf, i);

			write_new_file(buf,i);

                        if (server_write(buf, i) != i) 
                          break;

                }
                else if (FD_ISSET(server_fd, &fds)) {
                        i = sizeof(buf);

			/*we don't do any parsing of server
                          responses. Layter this may have to
                          change...*/

                        if ((i = server_read(buf, i)) <= 0) 
                          break; 

			/*we just throw anything we get from the server to standard out*/
			/*all we care about is the client, so we don't
                          clutter up our buffer with server responses
                          any more.*/
			
			/*and the client*/
                        if (client_write(buf, i) != i)
                          break;
                }
                else err(1, "select");
        } /*end for loop*/

	/*
	  What's the point now?
	  boutlen = bout_put(&bout, boutlen, sep, strlen(sep));
	*/
	/*there CAN be zeros in the buffer...so fprintf doesn't work*/



        free(bout);
        server_close();
        client_close();
}

void
mitm_run()
{
        fd_set fds;
        int i;

        signal(SIGCHLD, sig_chld);
        signal(SIGINT, sig_int);

        if (fcntl(sig_pipe[0], F_SETFL, O_NONBLOCK) < 0 ||
            fcntl(sig_pipe[1], F_SETFL, O_NONBLOCK) < 0)
                err(1, "fcntl");

        if (fcntl(http_fd, F_SETFL, O_NONBLOCK) < 0 ||
            fcntl(https_fd, F_SETFL, O_NONBLOCK) < 0)
                err(1, "fcntl");

        for (;;) {
                FD_ZERO(&fds);

                FD_SET(http_fd, &fds);
                FD_SET(https_fd, &fds);
                FD_SET(sig_pipe[0], &fds);

                i = MAX(http_fd, https_fd);
                i = MAX(sig_pipe[0], i);

                if (select(i + 1, &fds, 0, 0, 0) < 0) {
                        if (errno != EINTR)
                                err(1, "select");
                }
                i = sizeof(csin);

                if (FD_ISSET(sig_pipe[0], &fds)) {
                        while (read(sig_pipe[0], &i, 1) == 1)
                                ; /* empty non-blocking pipe */

                        reap_child();
                        continue;
                }
                if (FD_ISSET(http_fd, &fds)) {
                        client_fd = accept(http_fd, (struct sockaddr *)&csin, &
i);
                        do_ssl = 0;
                }
                else if (FD_ISSET(https_fd, &fds)) {
                        client_fd = accept(https_fd, (struct sockaddr *)&csin, 
&i);
                        do_ssl = 1;
                }
                else errx(1, "select failure");

                if (client_fd < 0) {
                        if (errno != EINTR && errno != EWOULDBLOCK)
                                err(1, "accept");
                }

		/*we increment file number before we fork so we each
                  get a nice unique file to write to! How cool is
                  that?*/

		file_number++;

                if (fork() == 0) {
                        close(http_fd);
			
                        mitm_child();

                        exit(0);
                }
                close(client_fd);
        }
}

int
main(int argc, char *argv[])
{

  int c;
  
        if (argc == 1) {
          usage();
          exit(0);
        }
        sslhost="localhost";

        while ((c = getopt(argc, argv, "t:p:s:S:dv:l:L:")) != -1) {
                switch (c) {
                case 't': /*target ip/hostname */
                  ip = optarg;
                  break;
                case 'p': /*target http port*/
                  port = atoi(optarg);
                  break;
                case 's': /*target ssl port*/
		  sslport = atoi(optarg);
		  break;
                case 'S': /*target ssl host - usually localhost*/
   		  sslhost = optarg;
   		  break;
                case 'd': /*debug*/
                  Opt_debug = 1;
                  break;
                case 'v': /*rewrite with this virtual host*/
		  virtualhost=optarg;
  		  break; 
		case 'l': /*local http port*/
		  localhttpport=atoi(optarg);
		  break;
    		case 'L': /*local https port*/
		  localhttpsport=atoi(optarg);
		  break;
                default:
                  usage();
                }
        }
        argc -= optind;
        argv += optind;

        if (argc != 0)
                usage();

	//what's this from? -D
	//       record_init(NULL);


	//  fprintf(stderr,"-v is assumed to be the same as -t if not specified\n");
	if (virtualhost==NULL)
	  virtualhost=ip;

        cert_init();

        mitm_init(port, listenaddr);

        warnx("relaying transparently");

        mitm_run();

        exit(0);
}

