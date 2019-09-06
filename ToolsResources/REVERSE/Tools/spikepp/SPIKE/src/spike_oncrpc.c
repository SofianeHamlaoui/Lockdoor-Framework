/* spike_oncrpc.c
 * Dominique Brezinski, 2002
 * License: GPL v2.0
 *
 * This supports sunrpcfuzz and provides useful ONC-RPC functions
 */

#include <stdlib.h>
#include <sys/types.h>
#include "spike.h"
#include "spike_oncrpc.h"

#include <string.h> /*for memset, memcpy, etc*/
#include <stdlib.h> /*for rand()*/
#include <unistd.h> /*for read()*/
#include <errno.h>  /*for errno*/
#include <fcntl.h>
#include <signal.h>

#define BIG_BUF 65535
/* #define DEBUG */

typedef enum {FALSE, TRUE} bool;
bool timeout = FALSE;

/* signal handler to handle read timeouts
 */
void
alarmhandler(int x)
{
	timeout = TRUE;
}

/* builds an ONC-RPC Call message header using the provided information
 * RETURNS:
 * none
 */
void
s_onc_call_header(uint xid, uint prognum, uint progver, uint procnum)
{
	s_bigword(xid); /* unique ID for this message */
	s_bigword(0);   /* this is a call message */
	s_bigword(2);   /* ONC-RPC version 2 */
	s_bigword(prognum); /* RPC program number */
	s_bigword(progver); /* program version */
	s_bigword(procnum); /* procedure number */
}

/* builds an ONC-RPC AUTH_NONE opaque header structure
 * RETURNS:
 * none
 */
void
s_onc_opaqueauth_none()
{
	s_bigword(0); /* AUTH_NONE */
	s_bigword(0); /* length 0 */
}

/* builds an ONC-RPC AUTH_SYS opaque header structure using the provided
 * information.
 * RETURNS:
 * none
 */
void
s_onc_opaqueauth_sys(uint stamp,		/* time stamp usually */
		     unsigned char* hostname, 	/* client's hostname */
		     uint uid, 			/* client's user id */
		     uint gid, 			/* client's group id */
		     uint* gids, 		/* array of client's add. GIDs*/
		     uint gidsnum)		/* number of add. GIDs */
{
	int i = 0;

	s_bigword(1); /* AUTH_SYS */
	s_binary_block_size_word_bigendian("ONCOpaqueAuthLength");
	s_block_start("ONCOpaqueAuthLength");
	s_bigword(stamp);
	s_xdr_string(hostname);
	s_bigword(uid);
	s_bigword(gid);
	s_bigword(gidsnum);
	for (i = 0; i < gidsnum; i++) {
		s_bigword(gids[i]);
	}
	s_block_end("ONCOpaqueAuthLength");
}

/* builds ONC-RPC VERIFIER_NONE header
 * RETURNS:
 * none
 */
void
s_onc_verifier_none()
{
	s_bigword(0); /* AUTH_NONE--used for AUTH_NONE and AUTH_SYS */
	s_bigword(0); /* length 0 */
}

int
s_onc_push_random_bytes(uint size)
{
	uint		i = 0;
	unsigned char* 	buffer = NULL;

	buffer = malloc(size + (4 - (size % 4)));
	if (buffer == NULL) {
		return 0;
	}

	for (i = 0; i < size; i += 4) {
		*(uint*)(buffer + i) = (uint)rand();
	}

	s_push(buffer, size);
	free(buffer);
	return 1;
}

void
s_onc_push_random_xdr_thing()
{
	unsigned int r;

#define NUMBEROFTHINGS 13
	/*pick a number between 1 and 100*/
	r=1+(unsigned int) (NUMBEROFTHINGS*1.0*rand()/(RAND_MAX+1.0));

	switch (r) {
		 case 1:
			s_bigword(0);
			break;
		case 2:
			s_bigword(rand());
			break;
		case 3:
			s_bigword(0);
			s_bigword(0);
			break;
		case 4:
			s_binary("01020304050607080102030405060708");
			break;
		case 5:
			s_xdr_string(s_get_random_fuzzstring());
			break;
		case 6:
			s_bigword(1);
			s_bigword(1);
			break;
		case 7:
			s_bigword(-1);
			s_bigword(-1);
			break;
		case 8:
			s_bigword(2);
			s_bigword(2);
			break;
		case 9:
			s_bigword(255);
			s_bigword(255);
			break;
		case 10:
			s_bigword(rand());
			s_bigword(rand());
			break;
		case 11:
			s_binary("01000000");
			s_binary("08000000");
			s_binary("00000000");
			s_binary("08000000");
			s_binary("ABCDABCD");
			s_binary("ff000000");
			s_binary("fe000000");
			s_binary("fd000000");
			break;
		case 12:
			s_xdr_string("localhost");
			break;
		case 13:
			s_binary("00000000");
			s_binary("00000000");
			s_binary("00000000");
			break;
		default:
			printf("Random thing chose a number out of scope %d!\n",r);
	}
}

/* creates ONC-RPC record fame header that will tally up the size and
 * add the right bit for the final frame indicator.
 * RETURNS:
 * when s_block_end() is called for this block the result will be a
 * properly formatted network byte-order uint with the MSB set
 */
int
s_binary_block_size_onc_recordfrag(char* blockname)
{
	return s_binary_block_size_word_bigendian_plussome(blockname, 
			                                   0x80000000);
}

/* does a Portmap GETPORT against the provided host for the specified
 * program (and version, protocol) provided in the onc_endpoint structure.
 * The results of the query will be put into the port member of the structure.
 * RETURNS:
 * 1 on success, 0 on failure
 */
int
s_onc_do_portmap_getport(char* host, struct onc_endpoint* endpoint)
{
	struct spike* old_spike;
	struct spike* onc_spike;
	unsigned char responsebuf[BIG_BUF], responsedata[BIG_BUF];
	uint responsebufsize = BIG_BUF, responsedatasize = BIG_BUF, xid = 0;

	old_spike = getcurrentspike();
	onc_spike = new_spike();
	setspike(onc_spike);

	xid = (uint)rand();

	s_binary_block_size_onc_recordfrag("ONCRecordFrag");
	s_block_start("ONCRecordFrag");
	s_onc_call_header(xid, 100000, 2, 3);
	s_onc_opaqueauth_none();
	s_onc_verifier_none();
	s_bigword(endpoint->prognum);
	s_bigword(endpoint->progver);
	s_bigword(endpoint->proto);
	s_bigword(0);
	s_block_end("ONCRecordFrag");

	printf("\nXID: %u\tPORTMAP_GETPORTv2\nProg: %u Ver: %u\n", 
		xid, endpoint->prognum, endpoint->progver);
	if (!spike_send_tcp(host, 111)) {
		return 0;
	}

	s_onc_read_reply(onc_spike, responsebuf, &responsebufsize);
	responsedatasize =
	s_onc_parse_response(responsebuf, 
			     responsebufsize, 
			     responsedata, 
			     responsedatasize);

	if (responsedatasize >= 4) {
		endpoint->port = ntohl(*(uint*)responsedata);
	}
	else {
		endpoint->port = 0;
	}

	spike_close_tcp();
	setspike(old_spike);
	spike_free(onc_spike);

	return 1;
}

/* executes a Protmap DUMP request against the specified host and puts
 * the results in onc_endpoint structures in the provided array.  It will
 * write at most arraysize number of endpoint elements into the array, so
 * provide an array large enough to hold all the expected endpoints (32-64
 * elements should be sufficient for most hosts).
 * RETURNS:
 * the number of endpoint elements written to the provided array
 */
int
s_onc_do_portmap_dump(char* host, struct onc_endpoint* eparray, uint arraysize)
{
	struct spike* old_spike;
	struct spike* onc_spike;
	unsigned char responsebuf[BIG_BUF], responsedata[BIG_BUF];
	uint responsebufsize = BIG_BUF, responsedatasize = BIG_BUF, xid = 0;
	uint i = 0, processed = 0;

	old_spike = getcurrentspike();
	onc_spike = new_spike();
	setspike(onc_spike);

	xid = (uint)rand();

	s_binary_block_size_onc_recordfrag("ONCRecordFrag");
	s_block_start("ONCRecordFrag");
	s_onc_call_header(xid, 100000, 2, 4);
	s_onc_opaqueauth_none();
	s_onc_verifier_none();
	s_block_end("ONCRecordFrag");

	printf("\nXID: %u\tPORTMAP_DUMPv2\n", xid);

	if (!spike_send_tcp(host, 111)) {
		return 0;
	}

	s_onc_read_reply(onc_spike, responsebuf, &responsebufsize);
	responsedatasize =
	s_onc_parse_response(responsebuf, 
			     responsebufsize, 
			     responsedata, 
			     responsedatasize);

	for (i = 0; i < arraysize; i++) {
		uint more = 0;
		if (processed + 4 <= responsedatasize) {
			more = ntohl(*(uint*)(responsedata + processed));
			processed += 4;
		}
		else {
			break;
		}
		if (!more) {
			break;
		}
		if (processed + 4 <= responsedatasize) {
			eparray[i].prognum = 
				ntohl(*(uint*)(responsedata + processed));
			processed += 4;
		}
		else {
			break;
		}
		if (processed + 4 <= responsedatasize) {
			eparray[i].progver = 
				ntohl(*(uint*)(responsedata + processed));
			processed += 4;
		}
		else {
			break;
		}
		if (processed + 4 <= responsedatasize) {
			eparray[i].proto =
				ntohl(*(uint*)(responsedata + processed));
			processed += 4;
		}
		else {
			break;
		}
		if (processed + 4 <= responsedatasize) {
			eparray[i].port = 
				ntohl(*(uint*)(responsedata + processed));
			processed += 4;
		}
		else {
			break;
		}
	}

	spike_close_tcp();
	setspike(old_spike);
	spike_free(onc_spike);

	return (i);
}

/* reads responses from socket and puts them in mesgbuf.  On success,
 * mesgbufsize will contain the number of bytes in mesgbuf.
 * RETURNS:
 * 1 on success, 0 on failure
 */
int
s_onc_read_reply(struct spike* cur_spike, 
		 unsigned char* mesgbuf, 
		 uint* mesgbufsize)
{
	unsigned char buffer[BIG_BUF];
	int ret = 0, size = 0;
	uint total = 0, fragsize = 0, finalfrag = 0;
#ifdef DEBUG
	int i = 0;
#endif

	s_fd_wait(); 
	memset(buffer,0x00,sizeof(buffer));
	alarm(5);
	signal(SIGALRM, alarmhandler);
	fcntl(cur_spike->fd, F_SETFL, O_NONBLOCK);
	while ((size < fragsize + 4) && !timeout) {
		ret = read(cur_spike->fd, 
			    buffer + total + size, 
			    BIG_BUF - (total + size));
		if (ret >= 0) {
			size += ret;
		}
		else {
			switch (errno) {
				case EINTR:
					timeout = TRUE;
					break;
				case EAGAIN:
					continue;
					break;
				default:
					timeout = TRUE;
					break;
			}
		}
		if (size >= 4 && fragsize == 0) {
			fragsize = ntohl(*(uint*)(buffer + total)) 
					 & 0x7FFFFFFF;
			finalfrag = (buffer[total] & 0x80) >> 7;
		}
		if (size >= fragsize + 4) {
			total += fragsize + 4;
			if (!finalfrag) {
				size -= (fragsize + 4);
				fragsize = 0;
				if (size >= 4) {
					fragsize = 
					ntohl(*(uint*)(buffer + total))
					      & 0x7FFFFFFF;
					finalfrag = (buffer[total] & 0x80) >> 7;
				}
			}
		}
	}
	fcntl(cur_spike->fd, F_SETFL, 0);
	alarm(0);
	*mesgbufsize = total < *mesgbufsize ? total:*mesgbufsize;
	memcpy(mesgbuf, buffer, *mesgbufsize);
#ifdef DEBUG
	for (i = 0; i < total; i++) {
		printf("[%2.2x]",buffer[i]);
	}
	printf("\n");
#endif
	timeout = FALSE;
	
	return 1;
}

/* this code is horrible--I hacked it together.  It is code for testing, so
 * no claims that it is even remotely safe.
 * don't touch it if you don't have to
 * RETURNS:
 * number of procedure response data bytes--this has the record framing
 * and RPC header data removed
 */
int
s_onc_parse_response(unsigned char* responsebuf, uint responsebufsize,
		     unsigned char* responsedata, uint responsedatasize)
{
	uint fragsize = 0, finalfrag = 0, processed = 0, tmp = 0;
	uint remaining = responsebufsize, xid = 0, rdprocessed = 0;
	uint tmpuint = 0;
	unsigned char* buffer = responsebuf;

	if (responsebuf == NULL || responsedata == NULL) {
		return 0;
	}

	while (remaining) {
		tmp = processed;
		tmpuint = 0;
		if (remaining > 4) {
			fragsize = ntohl(*(uint*)(buffer + processed)) 
				   & 0x7FFFFFFF;
			finalfrag = (buffer[processed] & 0x80) >> 7;
			processed += 4;
		}
		else {
			printf("Reply too small to be a valid RPC response\n");
			return 0;
		}
		if (xid) {
			tmpuint = fragsize < (responsedatasize - rdprocessed) ?
				  fragsize : (responsedatasize - rdprocessed);
			memcpy(responsedata + rdprocessed,
			       buffer + processed,
			       tmpuint);
			rdprocessed += tmpuint;
			processed += fragsize;
			remaining = responsebufsize - processed;
			if (finalfrag) {
				xid = 0;
			}
			continue;
		}
		xid = ntohl(*(uint*)(buffer + processed));
		printf("XID: %u\n", xid);
		processed += 4;
		switch (ntohl(*(uint*)(buffer + processed))) {
			case 1:
			  processed += 4;
			  switch (ntohl(*(uint*)(buffer + processed))) {
				case 0:
				  printf("REPLY STATUS: ACCEPTED\n");
				  processed += 4;
				  printf("VERIFIER FLAVOR: %u\n",
					 ntohl(*(uint*)(buffer + processed)));
				  processed += 4;
				  /* varible length opaque auth data */
				  tmpuint = ntohl(*(uint*)(buffer + processed));
				  processed += tmpuint + 4;
				  tmpuint = ntohl(*(uint*)(buffer + processed));
				  printf("ACCEPT STATUS: %u\n", tmpuint);
				  processed += 4;
				  if (tmpuint == 2) {
					  processed += 8;
				  }
				  tmpuint = (fragsize + 4) - (processed - tmp);
				  tmpuint = tmpuint <
					    (responsedatasize - rdprocessed) ?
					     tmpuint :
					     (responsedatasize - rdprocessed);
				  memcpy(responsedata + rdprocessed,
					 buffer + processed,
					 tmpuint); 
				  rdprocessed += tmpuint;
				  processed += (fragsize + 4) - 
					       (processed - tmp);
				  remaining = responsebufsize - processed;
				  break;
				case 1:
				  printf("REPLY STATUS: DENIED\n");
				  processed += 4;
				  printf("REJECT REASON: %u\n", 
					 ntohl(*(uint*)(buffer + processed)));
				  processed += (fragsize + 4) -
					       (processed - tmp);
				  remaining = responsebufsize - processed;
				  break;
				default:
				  printf("Invalid reply type!\n");
				  processed += (fragsize + 4) -
					       (processed - tmp);
				  remaining = responsebufsize - processed;
				  break;
			  }
			  break;
		  	case 0:
			  printf("Is a RPC Call Message!\n");
			  processed += (fragsize + 4) -
				       (processed - tmp);
			  remaining = responsebufsize - processed;
			  continue;
			  break;
		  	default:
			  printf("Is an invalid RPC Message!\n");
			  processed += (fragsize + 4) -
				       (processed - tmp);
			  remaining = responsebufsize - processed;
			  continue;
			  break;
		} /* switch() */

		if (finalfrag) {
			xid = 0;
		}
	} /* while() */

	return (rdprocessed);
}
