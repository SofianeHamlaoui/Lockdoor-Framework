/*

cifs.c
Copyright Dave Aitel
Covered under GPL v 2.0

*/

#define SLEEPSIZE 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*for memset*/
#include <sys/types.h>
#include <sys/socket.h>


#include <unistd.h>
#include <fcntl.h> 
#include <signal.h>

#include "spike.h"
#include "spike_dcerpc.h"
#include "hdebug.h"
#include "tcpstuff.h"


#include <stdlib.h>

void
usage()
{
  printf("Usage: ./cifs target pipe GUID Version VersionMinor(usually 0) function_number [login password skipvar skipstring]\r\n");
  printf("Example: ./cifs 192.168.1.105 \\\\pipe\\\\srvsvc e1af8308-5d1f-11c9-91a4-08002b14a0fa 3 0 2 bob bob 0 0 \n");
  printf("Read the msrpc readme when you get a chance.\n");
  exit(-1);

}

unsigned char *format1;

static int caught_sigpipe=0;

static void
sig_pipe(int signal)
{
  
  caught_sigpipe=1;

}




void
pretty_print(unsigned char * buffer2, unsigned int returnsize)
{
  unsigned int i;
  /*
    this if statement is used to control weather we do a TON of printing.
    You should only disable it  and use this function after you find a bug
    and have localized it with MSRPC_DO
  */
#ifndef SAVEBUF
  return;
#endif

  fprintf(stderr,"Pretty print size=%u\n",returnsize);
  fprintf(stderr,"Buffer:\n");
  for (i=0; i<returnsize; i+=4)
    {
      fprintf(stderr,"s_binary(\"%2.2x%2.2x%2.2x%2.2x\");\n",buffer2[i],buffer2[i+1],buffer2[i+2],buffer2[i+3]);
    }
  fflush(0);

}


int
main (int argc, char ** argv)
{
  char * target;
  char buffer[1500000];
  int port;
  char *pipename;
  unsigned char uid[2];
  unsigned char tid[2];
  unsigned char fid[2];
#define SAVESLOTS 5
  unsigned char *savedbuf[SAVESLOTS];
  unsigned int savedsize[SAVESLOTS];
  int currentsaveslot;

  struct spike * our_spike;
  unsigned long retval;
  unsigned short function_number;
  int i,j,s;
  unsigned int r;
  int sent;
  int randval;
  int msrpcdo;
  char * login = NULL, * password = NULL;
  unsigned char *object;
  object=NULL;
  int SKIPFUZZSTR=0;
  int SKIPVARIABLES=0;
  int fuzzvarnum;
  int fuzzstrnum;
  int firstfuzz;

#define ARGCNEEDED 7
  if (argc < ARGCNEEDED)
    {
      printf("Argc=%d not %d\n",argc,ARGCNEEDED);
      usage();
    }


  /*last arg is the optinal object*/
  object=NULL;
  if (argc==ARGCNEEDED+1)
    object=argv[ARGCNEEDED];


  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  msrpcdo=-1;
  if (getenv("MSRPC_DO"))
    {
      msrpcdo=atoi(getenv("MSRPC_DO"));
    }

  function_number=atoi(argv[6]);
  printf("Hitting function number %d\n",function_number);

  if(argc > ARGCNEEDED)
  {
    if(argc != ARGCNEEDED + 4)
      {
	usage();
      }
    
    login = argv[7];
    password = argv[8];
    printf("Login = %s password=%s\n",login,password);
    SKIPVARIABLES=atoi(argv[9]);
    SKIPFUZZSTR=atoi(argv[10]);
  }


  port=445;

  pipename=argv[2];

  printf("Pipename=%s\n",pipename);
  for (s=0;s<SAVESLOTS; s++)
    {
      savedbuf[s]=malloc(0);
      savedsize[s]=0;
    }
  currentsaveslot=0;

  signal(SIGPIPE,sig_pipe);

  if (getenv("RANDVAL"))
    randval=atoi(getenv("RANDVAL"));
  else
    randval=getpid();

  printf("MSRPC_DO=%d and RANDVAL=%d\n",msrpcdo,randval);

  srand(randval);
  printf("Initialized random with %d\n",randval);
  fprintf(stderr,"Initialized random with %d\n",randval);

  /*very important line. don't forget it*/
  s_init_fuzzing();
  printf("Yo2\n");


  our_spike=new_spike();
  
  
  if (our_spike==NULL)
    {
      fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
      exit(-1);
    }
  
  setspike(our_spike);
  
  /*do unicode the MS way*/
  s_set_unicode(1);
  /*zeroth fuzz variable is first variable*/
  //must have a spike set before we do this.
  s_resetfuzzvariable();
  printf("Yo3\n");
  fuzzvarnum=0;
  fuzzstrnum=0;
  firstfuzz=1;

  printf ("Fuzzing\n");
  while (!s_didlastvariable())
    {
      printf("Resetting Fuzz String\n");
      s_resetfuzzstring();
  

      if (firstfuzz)
	{
	  /*zeroth fuzz string is no change*/
      /*see below for why we have this if statement and loop*/
	  if (fuzzvarnum<SKIPVARIABLES )
	    {
	      for (fuzzvarnum=0; fuzzvarnum<SKIPVARIABLES; fuzzvarnum++)
		{
		  s_incrementfuzzvariable();
		}
	    }
	  
	  /*here is another part of where we implement the ability to jump to a particular
	    place in the fuzzing*/
	  if (fuzzstrnum<SKIPFUZZSTR)
	    {
	      for (fuzzstrnum=0; fuzzstrnum<SKIPFUZZSTR; fuzzstrnum++)
		{
		  s_incrementfuzzstring();
		}
	    }
	  firstfuzz=0;
	}
      else
	{
	  /*we reset this here so every new variable gets a new count*/
	  fuzzstrnum=0;
	}
      printf("yo\n");
    /*zeroth fuzz string is no change*/
      while(!s_didlastfuzzstring())
	{
	  printf("Clearing SPIKE\n");
	  spike_clear();
	  printf("Fuzzing Variable %d:%d\n",fuzzvarnum,fuzzstrnum);
	  //our_spike=new_spike();
	  
     
	  s_smb_negotiate(login, password);
	  sent=spike_send_tcp(target,port);
	  
	  if (sent==0 || caught_sigpipe)
	    {
	 /*this means the server went down!*/
	      if (sent==0)
		fprintf(stderr,"We killed it! \n");
	      else
		{
		  fprintf(stderr,"We caught sigpipe!\n");
		  fprintf(stdout,"We caught sigpipe!\n");
		}
	      
	      for (s=0; s<SAVESLOTS;s++)
		{
		  fprintf(stderr,"Current Save Slot = %d\n",currentsaveslot);
		  fprintf(stderr,"Printing save slot %d\n",s);
#if 0
		  pretty_print(savedbuf[s],savedsize[s]);
#endif
		}
	      fprintf(stderr,"Done.\n");
	      if (sent==0)
		{
		  //printf("Was not able to connect: Sleeping...\n");
		  //sleep(100);
		  //sleep(59999);
		}
	      //exit(1);
	    }
	  else
	    {
#ifdef SAVEBUF
	      /*we always malloc it with something*/
	      free(savedbuf[currentsaveslot]);
#endif
	    }



	  s_fd_wait();
	  memset(buffer,0,sizeof(buffer));
	  alarm(1);
	  retval=-1;
	  if (s_fd_wait()) {
	    retval=read(getcurrentspike()->fd,buffer,1500);
	  }
	  alarm(0);
	  if (retval==-1)
	    {
	      printf("couldn't smb negotiate continuing\n");
	      //spike_close_tcp();
	      //spike_free(our_spike);
	      //continue;
	    }
     
	  if (buffer[9]!=0x00) {
	    printf("Error value when trying to negotiate an SMB connection: %x\n",
		   buffer[10]);
	    
	    //spike_free(our_spike);
	    //printf("After Free\n");
	    //continue;
	  }
	  printf("Negotiated SMB Connection\n");
	  spike_close_tcp();
	  fuzzstrnum++;
	  s_incrementfuzzstring();
	  
	}
      fuzzvarnum++;
      
      s_incrementfuzzvariable();
    }/*end for each variable*/
	
  {
      while (0) {
	  spike_clear();
	  
	  
	  /*do the smb session startup*/
	  s_smb_session_setup(login, password);
	  spike_send();
	  s_fd_wait();
     
	  memset(buffer,0,sizeof(buffer));
	  alarm(1);
	  retval=-1;
	  if (s_fd_wait())
	    {
	      retval=read(getcurrentspike()->fd,buffer,1500);
	    }
	  alarm(0);
	  if (retval==-1)
	    {
	      printf("couldn't smb session startup continuing\n");
	      spike_close_tcp();
	      spike_free(our_spike);
	      continue;
	    }
	  
	  if (*(unsigned int *)(buffer+9)!=0x00000000) {
	    printf("Error value when trying to start up an SMB session: %8.8x\n",*(unsigned int *)(buffer+9));
	    if (0x00080001==(*(unsigned int *)(buffer+9))) {
	      printf ("Error returned was out of memory.\n");
	    }
	    
	    spike_close_tcp();
	    spike_free(our_spike);
	    printf("Continuing\n");
	    continue;
	  }
	
     

	  memcpy(uid,buffer+32,2);
	  printf("Uid is %x%x\n",uid[0],uid[1]);

	  
	  /*done with smb session startup*/

	  spike_clear();
	  
	  /*start IPC$ connection */
	  
	  s_smb_ipc_connect(uid);
	  spike_send();
	  s_fd_wait();
	  
	  memset(buffer,0,sizeof(buffer));
	  alarm(1);
	  retval=-1;
	  if (s_fd_wait()) {
	    retval=read(getcurrentspike()->fd,buffer,1500);
	  }
	  alarm(0);
	  if (retval==-1)
	    {
	      printf("couldn't smb session IPC$ startup continuing\n");
	      spike_close_tcp();
	      spike_free(our_spike);
	      continue;
	    }
	  
	  if (*(unsigned int *)(buffer+9)!=0x00000000) {
	    printf("Error value when trying to start up an SMB IPC$ session: %8.8x\n",*(unsigned int *)(buffer+9));
	    spike_close_tcp();
	    spike_free(our_spike);
	    continue;
	  }

	  memcpy(tid,buffer+32-4,2);
	  printf("Tree id is %x%x\n",uid[0],uid[1]);
	  
	  /*done with IPC$ connection*/
	  
	  /* do the NT Create AndX Request */
	  spike_clear();
	  
	  s_nt_createandx(tid,uid,pipename);
	  spike_send();
	  
	  printf ("Sent NT AndX request\n");
	  s_fd_wait();
	  
	  memset(buffer,0,sizeof(buffer));
	  alarm(1);
	  if (s_fd_wait()) 
	    {
	      retval=read(getcurrentspike()->fd,buffer,1500);
	    }
	  alarm(0);
	  if (retval==-1)
	    {
	      printf("couldn't NT Create AndX -  continuing\n");
	      spike_close_tcp();
	      spike_free(our_spike);
	      continue;
	    }
	  
	  if (*(unsigned int *)(buffer+9)!=0x00000000) {
	    printf("Error value when trying to start up an NT AndX: %8.8x\n",ntohl(*(unsigned int *)(buffer+9)));
	    if (0x220000c0==ntohl(*(unsigned int *)(buffer+9)))
	      {
		printf("Error returned was Access Denied\n");
		exit(1);
	      }
	    else 
	      {
		if (0xac0000c0==ntohl(*(unsigned int *)(buffer+9))) {
		  printf("Error returned was Pipe Not Available\n");
		  exit(1);
		}
		if (0x340000c0==ntohl((*(unsigned int *)(buffer+9)))) {
		  printf ("Error returned was File Not Found.\n");
		  exit(1);
		}
	      }
	    
	    spike_close_tcp();
	    spike_free(our_spike);
	    continue;
	  }
	  memcpy(fid,buffer+42,2);
	  printf("File id is %x%x\n",uid[0],uid[1]);
	  /*done with the NT Create AndX Request*/       
	  
	  spike_clear();
	  
	  /*Send the Netbios and SMB header for the DCE-RPC packet*/
	  
	  s_smbheader(fid,uid,tid);
	  
	  /*i is sent in just to keep incrementing*/
	  /*hardcoded the transfer syntax and syntax version of 2...they
	    are always the same*/
	  s_dce_bind(argv[3], /*GUID "Interface" */
		     "8a885d04-1ceb-11c9-9fe8-08002b104860", /*syntax - always the same*/
		     i, 
		     atoi(argv[4]), /*interface version major*/
		     atoi(argv[5]), /*interface version minor, usually zero*/
		     2); /*syntax version*/
	  s_block_end("DCEFragLength"); /*ALWAYS HAVE TO DO*/

	  /*phew! :>*/
	  /*had to add this for SMB block*/
	  s_block_end("smbblock");
	  s_block_end("bytecount");
	  sent=spike_send();
	  
     

	  
	  
	  s_fd_wait();
	  /*clear the response*/
	  alarm(1);
	  fcntl(our_spike->fd, F_SETFL, O_NONBLOCK);
	  retval=read(our_spike->fd,buffer,1500);
	  if (retval==-1 && 0) 
	    {
	      printf ("couldn't read, continuing\n");
	      perror("read()");
	      spike_close_tcp();
	      spike_free(our_spike);
	      continue;
	    }
	  alarm(0);
	  /*the offset of the DCE within the SMB stuff*/
#define DCEOFFSET 60
	  
	  if (buffer[2+DCEOFFSET]!=DCE_BIND_ACK)
	    {
	      printf("Warning, did not recieve bind_ack! Instead got 0x%x\n",buffer[2+DCEOFFSET]);
	      if (buffer[2+DCEOFFSET]==0x0d)
		printf("I believe 0x0d is provider reject - check your version number\n");
	      
	      if (buffer[2+DCEOFFSET]==0x00)
		{
		  /*this means the server went down!*/
		  fprintf(stderr,"We almost killed it! \n");
		  fprintf(stderr,"Current Save Slot = %d\n",currentsaveslot);
		  fprintf(stderr,"Printing save slot %d\n",s);
#if 0
		  pretty_print(savedbuf[currentsaveslot],savedsize[currentsaveslot]);
#endif
		  
		  fprintf(stderr,"Done.\n");
		  
		}
	    }
	  
#if 0
	  /*send first packet*/
	  spike_clear();
	  s_smb_dce_call(4,buffer,0,fid,uid,tid,object);
	  s_smbdce_doreadx(fid,uid,tid,buffer,sizeof(buffer));
#endif
	  
	  /*assume bind success...*/
	  /*send a random packet*/
	  spike_clear();
	  
	  /*2nd arg is alloc hint*/
	  /*s_dce_call_header(1, 5300, function_number,1,1);*/
	  
#ifdef CRASHLOCATOR
	  // r=number_of_items;
	  
	  /*EntryNameSyntax*/
	  s_intelword(3);
	  /*I think this is a "pointer"*/
	  s_binary("70 b7 23 00");
	  /*EntryName*/
     //s_dce_unistring("/.:/bob/bob");
	  s_dce_unistring(s_get_random_fuzzstring());
	  //s_dce_unistring("/.:/6");
	  
	  /*some null pointers*/
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  /*
	    push_random_msrpc_thing();
	    push_random_msrpc_thing();
	  */
	  /*
	    s_intelword(0x64);
	    s_intelword(0x1c20);
	  */
	  
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);
	  s_intelword(0);


	  
#endif
	  
	  /*test*/
	  /*r=1;*/
	  for (j=0; j<r+1; j++)
	    {
	      //push_valid_thing();
#ifndef CRASHLOCATOR
	      //push_random_msrpc_thing();
#endif
	    }
	  printf("Pushing %u things with total size of %u\n",(unsigned int)r+1,(unsigned int)s_get_size());
	  
	  /*save it off in our saved buffers*/
	  if (currentsaveslot==SAVESLOTS-1)
	    {
	      currentsaveslot=0;
	    }
	  else
	    {
	      currentsaveslot++;
	    }
#ifdef SAVEBUF
	  savedbuf[currentsaveslot]=malloc(s_get_size());
	  memcpy(savedbuf[currentsaveslot],s_get_databuf(),s_get_size());
	  savedsize[currentsaveslot]=s_get_size();
#endif
	  
	  /*
	    Localization of bugs:
	    only actually send the packet if we eithar do not have a MSRPC_DO
	    value or if our current i is less than that number
	    
	    Change the < to an == when you get close.
	  */
	  if (msrpcdo==-1 || i==msrpcdo)
	    {
	      printf("Sending request\n");
	      /*read from 4 first...*/
	      
	      pretty_print(s_get_databuf(),s_get_size());
	      if (!s_smb_dce_call(function_number,s_get_databuf(),s_get_size(),fid,uid,tid,object))
		{
		  printf("do_dce_call failed\n");
		  spike_free(our_spike);
		  return 0;
		}
	    }
	  
	  s_fd_wait();
	  /*clear the response*/
	  memset(buffer,0,sizeof(buffer));
	  //alarm(1);
	  //s_fd_wait();
	  //retval=read(getcurrentspike()->fd,buffer,1500);
	  s_smbdce_doreadx(fid,uid,tid,buffer,sizeof(buffer));
	  
	  //s_fd_wait();
  //retval=read(getcurrentspike()->fd,buffer,1500);
	  alarm(0);
	  if (retval==-1)
	    {
	      printf("continuing\n");
	      spike_close_tcp();
	      spike_free(our_spike);
	      continue;
	    }
	  if (buffer[2+DCEOFFSET]==DCE_FAULT)
	    {
	      printf("Recieved DCE_FAULT packet. As expected.\n");
	    }
	  else
	    {
	      printf("Recived response packet with type %x\n",buffer[2+DCEOFFSET]);
	      printf("Type 0x0c is BIND_ACK\n");
	    }
	  
	  if (retval==-1)
	    {
	      printf("dce call: Interesting - server closed socket!\n");
	      //return 0;
	    }
	  
	  
	  spike_close_tcp(); /*close the socket*/
	  
	  spike_clear();
	  spike_free(our_spike);
	}

      return 0;
    }
  return 1;
} /*end main()*/









