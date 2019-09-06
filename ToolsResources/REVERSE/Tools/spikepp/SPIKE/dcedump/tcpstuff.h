/*tcpstuff.h*/ 

#include <string.h> /*for memcpy and such*/

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

#include "portability.h" /*for uint32*/

int
getHostAddress(const char *host, struct sockaddr_in *addrP);

int
tcpconnect(const char * host,  const unsigned short port );

/*this actually just calls read_data - but it also does the ntohl()*/
int
read_uint32(int fd,uint32 *result);


/*write some data to the network. Also does encryption*/
int
write_data(int fd,uint32 size, unsigned char *inbuffer);

/*reads some data from the network. Also does encryption.*/
int
read_data(int fd, uint32 size, unsigned char *buffer);

/*makes a listener - returns into fd, 0 if fail, 1 if success*/
int
make_tcp_listener(unsigned short localport, int *fd);

/*writes a uint32 to the network in network byte order*/
int
write_uint32(int fd,uint32 data);

int
tcp_accept(int listenFd);
