#ifndef TCPSTUFF_H

#define TCPSTUFF_H
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


#ifndef HAVEUINT32
#define HAVEUINT32
typedef unsigned int uint32;
#endif

#ifndef u1byte
typedef unsigned char u1byte;   /* an 8 bit unsigned character type */
typedef unsigned short u2byte;  /* a 16 bit unsigned integer type   */
typedef uint32 u4byte;  /* a 32 bit unsigned integer type   */
typedef char s1byte;    /* an 8 bit signed character type   */
typedef short s2byte;   /* a 16 bit signed integer type     */
typedef long s4byte;    /* a 32 bit signed integer type     */
#endif


/*some TCP stuff*/
int
getHostAddress(const char *host, struct sockaddr_in *addrP);

int
tcpconnect(const char * host,  const unsigned short port, int getreserved );

int
tcpread(int fd, uint32 size, unsigned char *buffer);

int
tcpwrite(int fd,uint32 size, unsigned char *inbuffer);

int
make_tcp_listener(unsigned short localport, int *fd);

void
setlistenip(char * host);

/* this function takes in a listenFd and returns an acceptedFd */
int
tcp_accept(int listenFd);


#endif
