/*this file contains all the stuff that allows us to connect
  and listen and stuff for TCP*/

/*here we include all sorts of headers*/
#include "tcpstuff.h"
//#include "aes.h"

/*needed for debugging*/
//#include "hdebug.h"
#define hdebug(s) 
#include <unistd.h> /*for all sorts of stuff*/
#include <errno.h>


/*
** getHostAddress
**
** Translate a hostname or numeric IP address and store the result in addrP.
**
** Return 1 on success, 0 on failure.
*/

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


int
tcpconnect(const char * host,  const unsigned short port )
{
  int sfd = -1;
  struct sockaddr_in addr;
  struct linger lingerVal;


  /* Translate hostname from DNS or IP-address form */
  
  memset(&addr, 0, sizeof(addr));
  if (!getHostAddress(host, &addr))
    {
      hdebug("can't resolve host or address.\n");
      return -1;
    }
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(port);
  
  if ((sfd = socket(AF_INET,  SOCK_STREAM, 0)) < 0)
    {
      hdebug("Could not create socket!\n");
      return -1;
    }
  
  /* Set the "don't linger on close" option */
  
  lingerVal.l_onoff = 0;
  lingerVal.l_linger = 0;
  if (setsockopt(sfd, SOL_SOCKET, SO_LINGER, (char *)&lingerVal, sizeof(lingerVal)) < 0)
    {
      hdebug("Failed to set SO_LINGER option on socket!\n");
    }
  
  /* Now connect! */
  
  if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      close(sfd);
      return -1;
    }
  
  return sfd;
}



int
write_data(int fd,uint32 size, unsigned char *inbuffer)
{
  uint32 left;
  unsigned char *p;
  int i;
  unsigned char *buffer;

  /*we don't want to use the inbuffer because we change it*/
  buffer=malloc(size);
  memset(buffer,0x00,size);
  memcpy(buffer,inbuffer,size);


  /*printf("Write_data(size=%d)\n",size);*/
  left=size;
  p=buffer;
  do {
    i=write(fd,p,left);

    /*some error checking...*/
    if (i==-1)
      {
	hdebug("Could not write to socket while trying to write_data!\n");
	return 0; /*failure*/
      }
    left-=i;
    p+=i;

  } while (left>0);
  
  return 1; /*success*/

}


int
write_uint32(int fd,uint32 data)
{

  data=htonl(data);

  if (write_data(fd,4,(char*)&data))
    return 1;
  else
    return 0;

}

int
read_data(int fd, uint32 size, unsigned char *buffer)
{
  uint32 left;
  unsigned char *p;
  int i;

  memset(buffer,0x00,size);

  /*printf("read_data(size=%d)\n",size);*/
  left=size;
  p=buffer;
  do {
    i=read(fd,p,left);
    if (i==0 && errno==EINTR)
      continue;
    if (i==0 && errno==EAGAIN)
      continue;
    /*some error checking...*/
    if (i<=0 ) /*if 0, then we also need to exit*/
      {
	hdebug("Could not read from socket while trying to read_data!\n");
	/*this means the connection was closed!*/
	hdebug("Connection closed!\n");
	exit(0);
      }
    left-=i;
    p+=i;

  } while (left>0);

  return 1; /*success*/

}


int
read_uint32(int fd,uint32 *result)
{

  unsigned char buff[4]; /*sizeof uint32 is 4 bytes*/
 
  read_data(fd,4,buff);

  memcpy(result,(unsigned char *)buff,4);
  *result=ntohl(*result); /*here we handle endianness*/
  return 1;
}

int
make_tcp_listener(unsigned short localport, int *fd)
{
  int sfd = -1;
  struct sockaddr_in addr;
  int addrLen = sizeof(addr);
  
  
  /* Create the socket */
  
  if ((sfd = socket(AF_INET, (SOCK_STREAM), 0)) < 0)
    {
      hdebug("can't create listener socket\n");
      return 0;
    }
  
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(localport);
  
  if (bind(sfd, (struct sockaddr *)&addr, addrLen) < 0)
    {
      hdebug("listener bind failed\n");
      return 0;
    }
  
#define MAX_LISTEN 5
  if (listen(sfd, MAX_LISTEN) < 0)
    {
      hdebug("listen failed\n");
      return 0;
    }

  *fd=sfd;
  return 1;

}



/* this function takes in a listenFd and returns an acceptedFd */
int
tcp_accept(int listenFd)
{
  int clientFd;
  struct sockaddr_in clientAddr;
  int addrLen;
  struct linger lingerVal;

  addrLen = sizeof(struct sockaddr_in);
  if ((clientFd = accept(listenFd,
			 (struct sockaddr *)&clientAddr,
			 &addrLen)) < 0)
    {
      /* This is always an error, looping or not */
      return -1;
    }
  
  /* Set the "don't linger on close" option */
  
  lingerVal.l_onoff = 0;
  lingerVal.l_linger = 0;
  if (setsockopt(clientFd, SOL_SOCKET, SO_LINGER,
		 (char *)&lingerVal, sizeof(lingerVal))
      < 0)
    {
      hdebug("Warning: failed to set SO_LINGER option on socket\n");
    }

  return(clientFd);
  
}
