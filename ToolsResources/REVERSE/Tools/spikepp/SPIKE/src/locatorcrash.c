/*

msrpcfuzz
Copyright Dave Aitel
Covered under GPL v 2.0

*/


/*ep appears to have up to function 7. You can tell from the error
  messages when the function is invalid. You have to have netmon or
  ethereal up to actually use this. We don't get feedback from this
  directly.

  This is primitive and needs to be revised, but works well enough.
*/

//define this when you are ready to store off the bug!
//#define SAVEBUF

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
  printf("Usage: ./dceoversmb target pipe GUID Version VersionMinor(usually 0) function_number number_of_tries max_number_of_random_items\r\n");
  printf("Example: ./dceoversmb 10.25.25.15 \\\\pipe\\\\srvsvc e1af8308-5d1f-11c9-91a4-08002b14a0fa 3 0 2 10 3 [login password]\n");
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
push_valid_thing()
{

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



/*pushes one of any number of random MSRPC primatives*/
void
push_random_msrpc_thing()
{
  unsigned int r;
  unsigned char longstring[5000];

#define NUMBEROFTHINGS 14
  /*pick a number between 1 and 100*/
  r=1+(unsigned int) (NUMBEROFTHINGS*1.0*rand()/(RAND_MAX+1.0));

  /*test*/
  /*r=5;*/
  memset(longstring,0x41,sizeof(longstring));

  switch (r) {
  case 1:
    s_random_dce_string();
    break;
  case 2:
    s_intelword(rand());
    break;
  case 3:
    s_intelword(0);
    break;
  case 4:
    /*a guid sorta*/
    s_binary("01020304050607080102030405060708");
    break;
  case 5:
    s_dce_wordstring(s_get_random_fuzzstring());
    break;
  case 6:
    s_intelword(1);
    break;
  case 7:
    s_intelword(-1);
    break;
  case 8:
    s_intelword(2);
    break;
  case 9:
    s_intelword(255);
    break;
  case 10:
    s_intelword(rand());
    break;
  case 11:
    /*fake pointer structure of some kind that passes through ndrpointerdemarshal*/
  s_binary("01000000");
  s_binary("08000000");
  s_binary("00000000");
  s_binary("08000000");
  s_binary("ABCDABCD");
 //pointers -  does it matter what these are?
  //yes, must be valid "pointers"
  s_binary("ff000000");
  s_binary("fe000000");
  s_binary("fd000000");

    break;
  case 12:
    s_dce_wordstring(longstring);
    break;
  case 13:
    //this will pass through ndrsimplestrucunmarshall - in a boring way.
    //so we need a structure that starts HERE
    //3 words of zeros seems to work...
    s_binary("00000000");
    s_binary("00000000");
    s_binary("00000000");
    break;
  case 14:
    s_dce_unistring(s_get_random_fuzzstring());

  default:
    printf("Random thing chose a number out of scope %d!\n",r);

  }

  

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
  unsigned int number_of_tries, number_of_items;
  unsigned short function_number;
  int i,j,s;
  unsigned int r;
  int sent;
  int randval;
  int msrpcdo;
  char * login = NULL, * password = NULL;

#define ARGCNEEDED 9
  if (argc < ARGCNEEDED)
    {
      printf("Argc=%d not %d\n",argc,ARGCNEEDED);
      usage();
    }

  target=argv[1];
  printf("Target is %s\r\n",argv[1]);

  msrpcdo=-1;
  if (getenv("MSRPC_DO"))
    {
      msrpcdo=atoi(getenv("MSRPC_DO"));
    }

  function_number=atoi(argv[6]);
  printf("Hitting function number %d\n",function_number);

  number_of_tries=atoi(argv[7]);
  number_of_items=atoi(argv[8]);
  
  if(argc > ARGCNEEDED)
  {
  if(argc != ARGCNEEDED + 2)
  {
   usage();
  }
  
  login = argv[9];
  password = argv[10];
  }

  port=445;

  pipename=argv[2];

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
  for (i=0; i<number_of_tries; i++)
   {
     printf("Try Number %d on procedure %d\n",i,function_number);
     fflush(0);
     our_spike=new_spike();

     if (our_spike==NULL)
       {
	 fprintf(stderr,"Malloc failed trying to allocate a spike.\r\n");
	 exit(-1);
       }
     /*this really only gets called once*/
     s_init_fuzzing();
     setspike(our_spike);

     /*do unicode the MS way*/
     s_set_unicode(1);
     
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
     retval=read(getcurrentspike()->fd,buffer,1500);
     alarm(0);
     if (retval==-1)
       {
	 printf("couldn't smb negotiate continuing\n");
	 spike_close_tcp();
	 spike_free(our_spike);
	 continue;
       }
     
     if (buffer[9]!=0x00) {
       printf("Error value when trying to negotiate an SMB connection: %x\n",buffer[10]);
       spike_close_tcp();
       spike_free(our_spike);
       continue;
     }

     spike_clear();


     /*do the smb session startup*/
     s_smb_session_setup(login, password);
     spike_send();
     s_fd_wait();
     
     memset(buffer,0,sizeof(buffer));
     alarm(1);
     retval=read(getcurrentspike()->fd,buffer,1500);
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
     retval=read(getcurrentspike()->fd,buffer,1500);
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
     retval=read(getcurrentspike()->fd,buffer,1500);
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
	 }
       else 
	 {
	   if (0xac0000c0==ntohl(*(unsigned int *)(buffer+9))) {
	     printf("Error returned was Pipe Not Available\n");

	   }
	   if (0x340000c0==ntohl((*(unsigned int *)(buffer+9)))) {
	     printf ("Error returned was File Not Found.\n");
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

     /*send first packet*/
     spike_clear();
     s_smb_dce_call(4,buffer,0,fid,uid,tid);
     s_smbdce_doreadx(fid,uid,tid,buffer,sizeof(buffer));

     /*assume bind success...*/
     /*send a random packet*/
     spike_clear();

     /*2nd arg is alloc hint*/
     /*s_dce_call_header(1, 5300, function_number,1,1);*/

     r=0+(unsigned int) (number_of_items*1.0*rand()/(RAND_MAX+1.0));
     

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
     
     s_intelword(0x64);
     s_intelword(0x1c20);
     


     /*test*/
     /*r=1;*/
     for (j=0; j<r+1; j++)
       {
	 //push_valid_thing();
	 //push_random_msrpc_thing();
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
	 if (!s_smb_dce_call(function_number,s_get_databuf(),s_get_size(),fid,uid,tid))
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








