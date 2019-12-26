/*udpstuff.h*/

#ifndef UDPSTUFF_H
#define UDPSTUFF_H
/*returns 0 on success and -1 on failure*/
int
make_udp_listener(int local_port, int *fd);

//An extended udpconnect to connect FROM a certain port. However, if the server closes
//the connection, you will start getting connection refused errors.
//see: http://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq-3.html#ss3.6
int
udpconnect_ex(const char * host,  const unsigned short port, unsigned short local_port );

/*returns fd on success, -1 on failure*/
int
udpconnect(const char * host,  const unsigned short port );

/*returns 1 on success, 0 on failure*/
int
udp_write_data(int sock,struct sockaddr_in *toAddrp,
	       uint32 size, unsigned char *buffer);

#endif
