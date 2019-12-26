#include "spike_oncrpc.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>


#define EXIT_FAILURE 0
#define EXIT_SUCCESS 1
#define DEFAULT_ITERATIONS 10
#define DEFAULT_MAXPROC	200
#define DEFAULT_MINPROC	1
#define DEFAULT_NUMBER_OF_ITEMS 5
#define ARRAY_SIZE 32
#define BIG_BUF 65535

int fuzzprog(char*, struct onc_endpoint, uint, uint, uint, uint);

static int caught_sigpipe=0;

static void
sig_pipe(int signal)
{
  
  caught_sigpipe=1;

}

void usage(char* name)
{
	printf("\nUsage:\n");
	printf("%s -h <target host> <-s and/or -a> [optional args]\n", name);
	printf("\t-s <n>\tTest a specific RCP program 'n' [requires -v, and -p]\n");
	printf("\t-a\tTest all registered RPC programs\n");
	printf("\nWARNING: avoid running with -a or -s 100000 against\n");
	printf("localhost--doing so will probably register a bunch of bogus\n");
	printf("RPC programs with the local portmapper.\n");
	printf("\nOptional Arguments:\n");
	printf("\t-v <program version>\n");
	printf("\t-p <protocol number, 17 for UDP or 6 for TCP>\n");
	printf("\t-i <n>\tDo 'n' fuzzed messages per procedure\n");
	printf("\t-l <n>\t'n' is last procedure to test\n");
	printf("\t-f <n>\t'n' is first procedure to test\n");
	printf("\t-r <n>\tPush 'n' random xdr items onto the SPIKE\n\n");
}

int
main(int argc, char** argv)
{
	char*	host		= NULL;
	int	c		= 0;
	uint	portmapdump	= 0;
	uint	portmapgetport	= 0;
	uint	iterations	= DEFAULT_ITERATIONS;
	uint	maxprocnum	= DEFAULT_MAXPROC;
	uint	minprocnum	= DEFAULT_MINPROC;
	struct onc_endpoint	ep;
	struct onc_endpoint	eparray[ARRAY_SIZE];
	uint	arraysize	= 0;
	uint	i		= 0;
	uint    number_of_items = DEFAULT_NUMBER_OF_ITEMS;
	int 	randval		= 0;
	uint	throttle	= 0;
	ep.prognum = 0;
	ep.progver = 0;
	ep.proto   = 0;
	ep.port    = 0;

	while ((c = getopt(argc, argv, "h:v:p:as:i:l:f:r:t")) != -1) 
	{
		switch (c) 
		{
		case 'h':
		  host = optarg;
		  break;
		case 'v':
		  ep.progver = atol(optarg);
		  break;
		case 'p':
		  ep.proto = atol(optarg);
		  break;
		case 'a':
		  portmapdump = 1;
		  break;
		case 's':
		  ep.prognum = atol(optarg);
		  portmapgetport = 1;
		  break;
		case 'i':
		  iterations = atol(optarg);
		  break;
		case 'l':
		  maxprocnum = atol(optarg);
		  break;
		case 'f':
		  minprocnum = atol(optarg);
		  break;
		case 'r':
		  number_of_items=atol(optarg);
		  break;
		case 't':
		  throttle = 1;
		  break;

		default:
		  usage(argv[0]);
		  return (EXIT_FAILURE);
		  break;
		}
	}
	if (host == NULL || (!portmapgetport && !portmapdump)) 
	{
		usage(argv[0]);
		return (EXIT_FAILURE);
	}
	if (portmapgetport && (!ep.prognum || !ep.progver || !ep.proto)) {
		usage(argv[0]);
		return (EXIT_FAILURE);
	} 

	signal(SIGPIPE,sig_pipe);

	if (getenv("RANDVAL"))
	  randval=atoi(getenv("RANDVAL"));
	else
	  randval=getpid();
	
	srand(randval);
	printf("Initialized random with %d\n",randval);


	if (portmapgetport) {
		s_onc_do_portmap_getport(host, &ep);
		if (ep.port != 0) {
			fuzzprog(host, ep, iterations, minprocnum, maxprocnum, number_of_items);
		}
		else {
			printf("Could not query port for RPC Program: %u\n",
					ep.prognum);
			printf("Version: %u\tProtocol: %u\n", 
					ep.progver, ep.proto);
		}
	}
	if (portmapdump) {
		arraysize = s_onc_do_portmap_dump(host, eparray, ARRAY_SIZE);
		for (i = 0; i < arraysize; i++) {
			fuzzprog(host, eparray[i], iterations, minprocnum, maxprocnum, number_of_items);
		}
	}

	return (EXIT_SUCCESS);
}


/* Fuzz an RPC program.  This function creates 'iterations' of
 * malformed or bogus RPC call messages for each procedure from 0 to
 * 'maxprocnum'.  It supports sending RPC messages over UDP and TCP,
 * but only receives and parses the responses to the TCP messages out
 * of sheer laziness ;> See the comments in the code for specific
 * message creation/corruption logic.  This ifunction takes the target
 * host name, a completed onc_endpoint structure for the RPC program,
 * the iteration count, and the maximum procedure number to test (from
 * 0 to maxprocnum).  RETURNS: 0 - failure (though it currently won't
 * fail) 1 - success of some sort
 */
int
fuzzprog(char* host, struct onc_endpoint ep, uint iterations, uint minprocnum, uint maxprocnum, uint number_of_items)
{
	uint	i	= 0;
	uint	j	= 0;
	struct spike* 	our_spike;
#ifdef READREPLY
	unsigned char 	responsebuf[BIG_BUF], responsedata[BIG_BUF];
#endif

	uint 	responsebufsize = BIG_BUF;
        uint	responsedatasize = BIG_BUF;
        uint	xid 	= 0;
	char	ourhost[256] = {'\0'};
	char	bogushost[512] = {'\0'};
	uint	gids	= 0;

	/* initialize SPIKE context */
	s_init_fuzzing();
	our_spike = new_spike();
	setspike(our_spike);

	/* get our host name for AUTH_SYS purposes */
	gethostname(ourhost, 255);
	/* create our bogus host string on the off chance that there is an overflow or
	 * or format string handling problem with host name
	 */
	for (i = 0; i < 255; i++) {
	  strcat(bogushost + (i * 2), "%n");
	}

	/* loop through each program procedure */
	for (i = minprocnum; i <= maxprocnum; i++) {
	  /* loop through iteration count */
	  for (j = 0; j < iterations; j++) {
	    /* reset the SPIKE context so the buffers are clear */
            spike_clear();
	    /* reset our buffer size variables */
	    responsebufsize = BIG_BUF;
	    responsedatasize = BIG_BUF;

	    /* generate a new random XID for this RPC message */
	    xid = (uint)rand();
	    /* if the protocol  is TCP (6), then we need to add 'record framing' for
	     * this message
	     */
	    if (ep.proto == 6) {
		/* 4 out of 5 iterations we provide valid record frame lengths */
		if (j % 5) {
	    	  s_binary_block_size_onc_recordfrag("ONCRecordFrag");
	    	  s_block_start("ONCRecordFrag");
		}
		/* but on occasion we lie ;> */
		else {
		  s_bigword(0x80000000 | (uint)(rand() % 65536));
		}
	    }
	    /* create a valid RPC Call message header for the program
	       we are testing */
	    s_onc_call_header(xid, ep.prognum, ep.progver, i);
	    /* we switch back forth between AUTH_NONE and AUTH_SYS RPC
	     * authentication types
	     */
	    if (j % 2) {
	    	s_onc_opaqueauth_none();
	    }
	    else {
	        /* we create the AUTH_SYS authenticator with
		 * more-or-less valid information from the user
		 * context the program is being run under.  You are
		 * pretty dumb if you are using this code for
		 * malicious purposes.
		 */
		s_onc_opaqueauth_sys((uint)rand(),
				     j == 2 ? bogushost : ourhost,
				     getuid(),
				     getgid(),
				     &gids,
				     1);
	    }
	    /* we always use VERIFIER_NONE, since it is appropriate
	     * for AUTH_NONE and AUTH_SYS
	     */
	    s_onc_verifier_none();

	    /* 3 out of 4 itereations we put up to number_of_items of random things as
	     * the procedure arguments.  1 out of 4 iterations we put 0 to 99 random
	     * bytes as the procedure arguments.
	     */
	    if (j % 4) {
	        int q;
	        unsigned int r;

	        r = 0 + (unsigned int) (number_of_items*1.0*rand()/(RAND_MAX+1.0));
	        for (q = 0; q < r+1; q++) {
		  //push_valid_thing();
		  s_onc_push_random_xdr_thing();
	        }
	        printf("Pushing %u things with total size of %u\n",
		       (unsigned int)r+1,
		       (unsigned int)s_get_size());
	    }
	    else {
	        s_onc_push_random_bytes(rand() % 100);
	    }

	    /* if this TCP and we are not lying about the record framing length, then have
	     * SPIKE update the length uint in the message header
	     */
	    if (ep.proto == 6 && j % 5) {
	    	s_block_end("ONCRecordFrag");
	    }
	    printf("\nIteration number: %d\n",j);
	    printf("XID: %u\tFuzzing RPC Program: %u\n", xid, ep.prognum);
	    printf("Version: %u\tProcedure: %u\n", ep.progver, i);
	    printf("Protocol: %u\tPort: %u\n", ep.proto, ep.port);
	    if (ep.proto == 17) {
	    	if (!spike_send_udp(host, ep.port)) {
	          printf("UDP send failed!\n");
		  perror("");
		  s_close_udp();
		  sleep(1);
	    	}
		else
		{
		  s_close_udp();
		}
	    }
	    else if (ep.proto == 6) {
	    	if (!spike_send_tcp(host, ep.port)) {
	          printf("TCP send failed!\n");
		  sleep(1);
		  continue;
	    	}

		printf("Sent data successfully\n");
#ifdef READREPLY
	    	s_onc_read_reply(our_spike, responsebuf, &responsebufsize);
	    	responsedatasize =
	    	s_onc_parse_response(responsebuf,
			             responsebufsize,
				     responsedata,
				     responsedatasize);
#endif
	    	spike_close_tcp();
	    }
	  }
	}
	return 1;
}
