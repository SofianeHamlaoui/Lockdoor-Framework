/*helper routines for UDP*/

#include <sys/types.h> /*added for *BSD*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> /*for memcpy*/
#include <errno.h>
#include <stdio.h> /*close()*/

#include "tcpstuff.h" /*for getHostAddress*/
#include "udpstuff.h"
#include "hdebug.h"

/*returns 0 on success and -1 on failure*/
int
make_udp_listener(int local_port, int *fd)
{
  int udp_socket;
  struct sockaddr_in sa;

  hdebug("calling: make_udp_listener()\n");

  udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (udp_socket<0)
    {
      hdebug("Failed to listen on udp socket\n");
      return -1;
    }

  memset(&sa, 0x00, sizeof sa);

  sa.sin_family=AF_INET;
  sa.sin_addr.s_addr=INADDR_ANY;
  sa.sin_port=htons(local_port);

  if (bind(udp_socket, (struct sockaddr *)&sa, sizeof(sa))==-1)
    {
      hdebug("udp bind failed\n");
      return -1;
    }

  hdebug("Success in make_udp_listener\n");
  *fd=udp_socket;
  return 0; /*success*/

}

//An extended udpconnect to connect FROM a certain port. However, if the server closes
//the connection, you will start getting connection refused errors.
//see: http://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq-3.html#ss3.6
int
udpconnect_ex(const char * host,  const unsigned short port, unsigned short local_port )
{
  int sfd = -1;
  struct sockaddr_in addr;
  /* Translate hostname from DNS or IP-address form */


  memset(&addr, 0, sizeof(addr));  
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=INADDR_ANY;
  addr.sin_port=htons(local_port);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr))==-1)
    {
      hdebug("udp bind failed\n");
      return -1;
    }
  

  if (!getHostAddress(host, &addr))
    {
      hdebug("can't resolve host or address.\n");
      return -1;
    }
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(port);
  
  if ((sfd = socket(AF_INET,  SOCK_DGRAM, 0)) < 0)
    {
      hdebug("Could not create socket!\n");
      return -1;
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
udpconnect(const char * host,  const unsigned short port )
{
  int sfd = -1;
  struct sockaddr_in addr;
  /* Translate hostname from DNS or IP-address form */

  memset(&addr, 0, sizeof(addr));
  if (!getHostAddress(host, &addr))
    {
      hdebug("can't resolve host or address.\n");
      return -1;
    }
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(port);
  
  if ((sfd = socket(AF_INET,  SOCK_DGRAM, 0)) < 0)
    {
      hdebug("Could not create socket!\n");
      return -1;
    }
  
  /* Now connect! */
  
  if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      close(sfd);
      return -1;
    }
  
  return sfd;

}

/*returns 1 on success, 0 on failure*/
int
udp_write_data(int sock,struct sockaddr_in *toAddrp,
	       uint32 size, unsigned char *buffer)
{
  int err;
  /*  struct sockaddr_in toAddr;*/
  /*0 for flags*/
  hdebug("udp_write_data():\n");

  /*getpeername will fail on a udp sock. Sucks. So we do this instead.*/
  if (toAddrp==NULL)
    {
      hdebug("toAddrp == NULL, resorting to write()\n");
      err=write(sock,buffer,size);
      if (err!=-1)
	return 1;
      return 0;
    }

  err=sendto(sock,buffer,size,0,(struct sockaddr *)toAddrp,sizeof(struct sockaddr_in));
  if (err==-1)
    {
      if (errno!=EINTR)
	{
	  hdebug("udpstuff.c: sendto failed\n");
	  return 0; /*failure*/
	}
    }

  return 1; /*success*/

}
