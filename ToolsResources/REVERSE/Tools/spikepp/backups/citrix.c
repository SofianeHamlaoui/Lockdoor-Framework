/* Citrix.c - the beginings of a Citrix ICA fuzzer. Currently
demonstrates some weirdness in the TCP stack with flushing...maybe
a bug in SPIKE?
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>


#include "spike.h"
#include "hdebug.h"
#include "tcpstuff.h"


void
usage()
{
  printf("Usage: ./citrix target port\r\n");
  printf("./citrix 192.168.1.101 1494\n");
  
  exit(-1);
}


int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  int port;

  struct spike * our_spike;
  unsigned long retval;
  int i;

  if (argc!=3)
    {
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  port=atoi(argv[2]);

  our_spike=new_spike();


  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }

  setspike(our_spike);

  memset(buffer,0x41,sizeof(buffer));
  buffer[sizeof(buffer)]=0;

  
  for (i=0; i<500; i+=4)
    {
      memcpy(buffer+i,"%25s",4);
    }

  /*  
Allow: OPTIONS, TRACE, GET, HEAD, DELETE, PUT, COPY, MOVE, PROPFIND, PROPPATCH, SEARCH, SUBSCRIBE, UNSUBSCRIBE, POLL, BDELETE, BCOPY, BMOVE, BPROPPATCH, BPROPFIND, LOCK, UNLOCK
  */
 
  buffer[140000]=0;

  printf("Buffer size = %d\r\n",strlen(buffer));


  if (spike_connect_tcp(target,port)<0)
    {
      printf("Couldn't connect!\n");
      exit(-1);
    }

  s_fd_wait();
  retval=read(our_spike->fd,buffer,1500);

  /*initial packet*/
  spike_clear();
  s_binary("7f 7f 49 43 41 00");

  /*
  s_print_buffer();
  s_printf_buffer();
  */
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  s_fd_wait();
  printf("reading packet\r\n");
  memset(buffer,0x00,sizeof(buffer));
  retval=read(our_spike->fd,buffer,1500);

  printf("Done with read\n");
  /*that's the 24 byte packet*/
  spike_clear();

  s_fd_wait();
  s_binary("01 64 00 64 00 0f  00 01 04 77 66 69 63 61
  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   
  00 08 29 12 73 ec 3e 0f  00 24 00 00 00 00 00 00   
  00 01 fc 00 00 00 00 00  00 00 00 00 00 00 00 00   
  00 00 00 00 00 00 00 8c  03 58 00 58 00 0c 00 00   
  00 01 00 00 00 00 00 00  00 00 00 00 00 8c 70 fc   
  00 95 69 fe 02 9b 67 9b  67");
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  sleep(1);
  spike_clear();
  /*second little packet we send*/
  s_binary("01 3c 00 3c 00 0e  05 01 02 54 43 50 00 00  
  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 b4 05 02 00 04 a9 c0   
  a8 01 64 a2 01 00 00 08  50 34 c1 00 00 00 00 00   
  00         ");
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  sleep(1);


  /*Next packet*/
  /*PDRFRAME packet!*/
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_byte("first");
  s_binary("04 01 02"); /*figure out what these are later*/
  s_block_start("first");
  s_string("PDRFRAME.DLL");
  s_binary("00");
  s_block_end("first");
  s_string("PDRFRAME");
  s_binary("00 08 29 62 92 00 00 00  00 09 00 00 00 "); /*whatever.*/
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  
  sleep(1);

  /*Next packet*/
  /*PDCRYPT packet*/
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_byte("first");
  s_binary("04 01 01"); /*figure out what these are later*/
  s_block_start("first");
  s_string("PDCRYPT1.DLL");
  s_binary("00");
  s_block_end("first");
  s_string("PDCRYPT1");
  s_binary("00 08 29 17 73 64 19 00  00 0b 00 00 00 01 00 00 00");
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);


  /*Next packet*/
  /*WDICA*/
  /*CTXTW, CTXCLIP packet.*/
  printf("WDICA CTXTW packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");

  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary(" 0b 02 01 09 49 43 41 20 33 
  2e 30 00 00 00 00 00 00  57 44 49 43 41 00 00 00 
  00 08 29 62 92 00 00 00  00 f6 d0 5a 86 16 09 d1 
  07 13 0c 27 24 0f 00 00  00 00 00 00 00 00 08 06 
  00 06 00 ff ff ff ff 51  00 00 00 00 00 09 00 58 
  00 00 00 00 00 02 00 18  00 b2 00 00 00 43 54 58 
  54 57 20 20 00 09 00 43  54 58 43 4c 49 50 00 11 
  00 43 54 58 43 44 4d 20  00 0a 00 43 54 58 43 50 
  4d 20 00 04 00 43 54 58  43 41 4d 20 00 0c 00 43 
  54 58 43 4d 20 20 00 0d  00 43 54 58 43 43 4d 20 
  00 08 00 43 54 58 54 57  49 00 00 0f 00 43 54 58 
  5a 4c 46 4b 00 12 00 00  02 04 00 04 00 05 00 10 
  00 06 00 01 00 14 00 02  00 00 00 fa 00 96 00 ");
  s_block_end("CitrixPacket");

  
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

 /*Next packet*/
  /*Thinwire3.0 packet*/
  printf("Thinwire3.0 packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  /*real packet is 01 b0 00 b0 00- we are 01 b3 00 b3 00 
    so we need to change that...*/
  /*subtract and additional 3*/
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  0a 03 01 04 54 68 69 6e 77   
  69 72 65 33 2e 30 00 00  76 64 74 77 33 30 00 00 
  00 08 29 62 92 00 00 00  00 00 02 00 00 00 00 00 
  00 00 00 00 00 00 00 00  80 00 00 30 00 01 01 00 
  00 01 00 00 00 14 00 01  00 80 02 e0 01 00 00 00 
  80 00 00 30 00 01 01 00  00 0f 00 00 00 14 00 01  
  00 ff 7f ff 7f 00 00 00  00 00 00 00 00 00 00 00  
  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  
  00 00 00 00 00 00 00 00  00 00 80 00 00 00 00 30  
  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  
  00 00 00 00 00 00 00 00  00 15 00 0e 00 aa a2 02  
  00 00 00 00 00");
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

 /*Next packet*/
  printf("clipboard packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  /*clipboard packet*/
  s_binary("09  03 02 02 43 6c 69 70 62 
  6f 61 72 64 00 00 00 00  00 00 00 00 00 00 00 00  
  00 08 29 62 92 00 00 00  00 00 00 02 00 00 00 00  
  00 00 00 00 00  ");


  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

  /*ClientDrive packet*/
/*Next packet*/
  printf("ClientDrive packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  08  03 01 04 43 6c 69 65 6e
  74 44 72 69 76 65 00 00  00 00 00 00 00 00 00 00 
  00 08 29 62 92 00 00 00  00 00 04 00 00 00 03 00 
  00 84 18 16 04 00 00 00  00 00 00 58 02 00 00 00 
  00 00 00 00 00 00 00 00  00");

  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);
 /*ClientPrinte packet*/
  /*Next packet*/
  printf("ClientPrinte packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  07 03 04 06 43 6c 69 65  6e   
  74 50 72 69 6e 74 65 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 10 00 00 00 00 01 00   
  00 00 04 00 04 00 00 01  00 00 00 00 00
");

  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

  /*VDCAM.DLL packet*/
  /*Next packet*/
  printf("VDCAM.DLL packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);

  s_binary("
  06  03 01 01 56 44 43 41 4d   
  2e 44 4c 4c 00 00 00 00  49 43 41 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 00 10 00 00 00 00 00   
  00 00 00 00 00 ff 07 01  00
");

  s_block_end("CitrixPacket");

  
  if (spike_send()<0)
      {
	printf("Couldn't send data!\r\n");
	exit(-1);
      }

  sleep(1);
  /*ClientAudCvt packet*/
  printf("ClientAudCvt packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  05 09 01 01 43 6c 69 65 6e   
  74 41 75 64 43 76 74 00  00 00 00 00 00 00 00 00
  00 08 29 62 92 00 00 00  00
");
  
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);
  /*ADPCM_Module packet*/
  printf("ADMPC_Module packet\n");
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  04 09 01 01 41 44 50 43 4d   
  5f 4d 6f 64 75 6c 65 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00  
");
  
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

  /*ClientManage ICA packet*/
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("

  03  03 01 01 43 6c 69 65 6e   
  74 4d 61 6e 61 67 65 00  49 43 41 00 00 00 00 00  
  00 08 29 62 92 00 00 00  00 00 20 00 00 00 00 00  
  00 00 00 00 00 ff 07 38  00 01 00 00 00 01
");
  
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);



  /*ClientComm packet*/
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  02 03 04 04 43 6c 69 65 6e   
  74 43 6f 6d 6d 00 00 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 00 01 00 00 00 01 00   
  00 00 04 00 04 00 00 00  00 00 00 00 00


");
  
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

  /*VDTWIN.DLL packet*/
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary("
  01 03 01 01 56 44 54 57 49 
  4e 2e 44 4c 4c 00 00 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 00 80 00 00 00 00 00   
  00 00 00 00 00           
");
  
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);
  
  /*VDFON30W.DLL packet*/
  spike_clear();
  s_block_start("CitrixPacket");
  s_binary("01");
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  s_binary_block_size_intel_halfword_plus("CitrixPacket",-3);
  /*  Original block*/

  /*s_binary("
  00 03 01 01 56 44 46 4f 4e   
  33 30 57 2e 44 4c 4c 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 00 00 04 00 00 00 00   
  00 00 00 00 00        
  ");*/

  s_binary("
  00 03 01 01 56 44 46 4f 4e   
  33 31 57 2e 44 4c 4c 00  00 00 00 00 00 00 00 00   
  00 08 29 62 92 00 00 00  00 00 00 04 00 00 00 00   
  00 00 00 00 00        
  ");
  
  s_block_end("CitrixPacket");

  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }

  sleep(1);

  /*ACK server's type 0x02 packet*/
  spike_clear();
  s_binary("04 00 00");
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  /*server sends us null packet with 02 00 02 00*/

  /*ACK server's type null packet*/
  spike_clear();
  s_binary("03 00 04 00 f4"); /*whatever is this?*/
  if (spike_send()<0)
    {
      printf("Couldn't send data!\r\n");
      exit(-1);
    }
  sleep(1);



  printf ("Done fuzzing citrix!\n");
  sleep(1);
  return 0;
}







