/*Spike_dce_rpc.c*/
/*Dave Aitel, 2001*/
/*License: GPL v2.0 */
/*this supports msrpcfuzz, and provides a few useful dce-rpc functions
in case someone else wants them*/
/*this will even do ltlm authentication - eventually*/
/*TODO

  Fix the string stuff so it's a real DCE string.
  Get multi-fragment queries working better

*/

#include <stdlib.h>
#include <sys/types.h>

#include "spike.h"
#include "spike_dcerpc.h"


#include <string.h> /*for memset, memcpy, etc*/
#include <unistd.h>/*for read()*/

//#define FUZZINGDCE
//Don't comment this out if you want it to work
#define DOWRITEX

char *
string_from_buffer(unsigned char *buf,int length)
{
  unsigned char *p;
  unsigned char * retstring;
  int i;
  static char * memleak=NULL;

  retstring=malloc(length*2+1);
  /*lame, I know*/
  if (memleak!=NULL)
	  {free(memleak);}
  memleak=retstring;

  memset(retstring,0x00,length*2+1);

  p=buf;
  for (i=0; i<length; i++,p++)
    {
      sprintf(retstring+i*2,"%2.2x",*p);
    }

  /*oooh, the memory leak is nice*/
  return retstring;
}

unsigned char
hex_nib_from_hbyte(unsigned char byte)
{
  int c;
  int b;
  c=byte;
  if ((c >= '0') && (c <= '9'))
    b=c-'0';
  else if ((c >= 'a') && (c <= 'f'))
    b=c-'a'+10;
  else if ((c >= 'A') && (c <= 'F'))
    b=c-'A'+10;

  return b;
}

int
hexstring_to_buffer(char * hexstring,char * buffer)
{
  int i;

  /*
  printf("hexstring_to_buffer %d = %s\n",strlen(hexstring),hexstring);
  */
  /*for each byte of the buffer*/
  for (i=0; i<strlen(hexstring)/2; i++)
    {
      unsigned char nib1;
      unsigned char nib2;
      /*debug hex crap*/
      /*printf("hexstring[i*2]=%c i*2+1=%c\n",hexstring[i*2],hexstring[i*2+1]);*/
      nib1=hex_nib_from_hbyte(hexstring[i*2]);
      nib2=hex_nib_from_hbyte(hexstring[i*2+1]);
      buffer[i]=(nib1<<4)|(nib2<<0);
      /*debug hex crap*/
      /*
      printf("nib1=%x, nib2=%x buffer[i]=%2x\n",nib1,nib2,buffer[i]);
      */
    }
 
  return i;
}

/*takes it in raw off the wire, in wierd intel order*/
/*always 16 bytes long*/
int guid_rawbuffer_to_string(char * buffer, char * mystring)
{
  /*16 total*/
  char tbuffer[4];
  char tbuffer2[2];
  char tbuffer3[2];
  char tbuffer4[2];
  char tbuffer5[6];

  memcpy(tbuffer,buffer,4); /*don't mess with original buffer*/
  memcpy(tbuffer2,buffer+4,2); /*don't mess with original buffer*/
  memcpy(tbuffer3,buffer+6,2); /*don't mess with original buffer*/
  memcpy(tbuffer4,buffer+8,2); /*don't mess with original buffer*/
  memcpy(tbuffer5,buffer+10,6); /*don't mess with original buffer*/
  
  /*swap the bytes that need to be swapped*/
  intel_order(tbuffer,4);
  intel_order(tbuffer2,2);
  intel_order(tbuffer3,2);
  sprintf(mystring,"%s-%s-%s-%s-%s",
	  string_from_buffer(tbuffer,sizeof(tbuffer)),
	  string_from_buffer(tbuffer2,sizeof(tbuffer2)),
	  string_from_buffer(tbuffer3,sizeof(tbuffer3)),
	  string_from_buffer(tbuffer4,sizeof(tbuffer4)),
	  string_from_buffer(tbuffer5,sizeof(tbuffer5)));
  
  return 1; /*always works because we're so cool*/
}


/*hmm. we need to do the intel-reversing on this thing*/
int
uuid_string_to_buffer(unsigned char * buffer, char *uuid)
{
  char tempbuffer[400];
  int err;

  memset(tempbuffer,0x00,sizeof(tempbuffer));
  /*uuids look like <4bytes>-<2bytes>-<2bytes>-<2bytes>-<6bytes>*/
  /*all this stuff is just to get rid of the dashes. Sucks, but sue me.*/
  memcpy(tempbuffer,uuid,8);
  memcpy(tempbuffer+8,uuid+9,4);
  memcpy(tempbuffer+12,uuid+14,4);
  memcpy(tempbuffer+16,uuid+19,4);
  memcpy(tempbuffer+20,uuid+24,12);

  /*  printf("uuid_string_to_buffer:%s from %s\n",tempbuffer,uuid);*/
  err=hexstring_to_buffer(tempbuffer,buffer);
  if (err)
    {
      intel_order(buffer,4);
      intel_order(buffer+4,2);
      intel_order(buffer+6,2);
      /*intel_order(buffer+8,2);*/ /*this doesn't happen eithar for some reason*/
      /*for some reason the last 8 bytes are in network byte order.*/
      /*printf("returning %d\n",err);*/
      return err;
    }
  else
    {
      return 0;
    }
}


/*returns 1 on success, 0 on failure*/
/*I think callid is actually ignored by MSRPC servers. Pass something in though.*/
int
s_dce_bind(char * uuid,char * transfer_syntax,
	   unsigned int callid, unsigned int interfaceversionmajor, unsigned int interfaceversionminor,
	   unsigned int syntaxversion)
{

  unsigned char uuidbuffer[400]; 
  unsigned int uuidlength;


  /*why does the DCEFrag start here? - but it does*/
  s_block_start("DCEFragLength");  
  /*version*/
  s_binary("05");

  /*minor version*/
  s_binary("00");
  
  s_binary(BINDPACKET);
  /*binds are always just one packet*/
  /*  s_binary((unsigned char) 0x00 | LASTFRAG | FIRSTFRAG);*/
  s_binary("0x03");



  /*now set Data Rep, whatever that is */
  s_intelword(0x00000010);

  /*now we have 2 bytes of frag length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength");
#else
  s_binary_block_size_intel_halfword("DCEFragLength");
#endif

  /*auth length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEAuthLength");
#else
  s_binary_block_size_intel_halfword("DCEAuthLength");
#endif


  /*call ID */
  s_intelword(callid);

  /*buffer xmit frag*/
  s_binary("d016");
  /*  s_binary("d016");*/

  /*buffer max recv frag*/
  s_binary("d016");
  /*  s_binary("d016");*/

  /*assoc Group*/
  s_binary("00000000");
  /*s_binary("00000000");*/

  /*num ctx items - ethereal incorrect reports this as 1 byte*/
  s_intelword(1);
  
  /*context ID*/
  s_binary("0x0000");

  /*num trans items*/
  /*0x0100*/
  s_intelhalfword(0x01);


  /*Interface UUID*/
  uuidlength=uuid_string_to_buffer(uuidbuffer,uuid);
  s_push(uuidbuffer,uuidlength);

  /*this is actually 2 intel-ordered shorts, a major and then a minor version*/
  /*0x3000 0000*/ /*major of 3, minor of zero*/
  s_intelhalfword(interfaceversionmajor);
  s_intelhalfword(interfaceversionminor);

  /*now do syntax*/
  uuidlength=uuid_string_to_buffer(uuidbuffer,transfer_syntax);
  s_push(uuidbuffer,uuidlength);
  
  /*0x200000000*/
  s_intelword(syntaxversion);

  s_block_start("DCEAuthLength");
  s_block_end("DCEAuthLength");
  /*caller must end this explicitly*/

  s_block_end("DCEFragLength");  

  return 1;
}

/*I have no idea which it is!*/
/*actually, according to Luke kennith Casington, it's neithar.
But we'll fudge it because it seems to work until we have
time to fix it*/
int 
s_dce_wordstring(unsigned char * mystring)
{
 unsigned int size;
  unsigned int size2;
  unsigned int i;
  
  size=strlen(mystring);
  if (size > (unsigned int)0xffffffff-(unsigned int)12)
    printf("Warning, dce_wordstring() length overflowed!\n");
  size2=size;
 /*page 51 of DCE-RPC book*/ 
  s_intelword(size2);
  s_intelword(0);
  s_intelword(size2);
  for (i=0; i< size2; i++)
    s_push(mystring+i,1);

  while (size % 4 != 0)
    {
      s_binary("00");
      size++;
    }

  return size+12; /*2 for intel word */
}

int
s_dce_unistring(unsigned char *mystring)
{

  unsigned int size;
  unsigned int size2;
  //  unsigned int i;
  
  //+1 for ending null
  size=strlen(mystring)+1;
  if (size > (unsigned int)0xffffffff-(unsigned int)12)
    printf("Warning, dce_wordstring() length overflowed!\n");
  size2=size;
  /*page 51 of DCE-RPC book*/ 
  s_intelword(size2);
  s_intelword(0);
  s_intelword(size2);

  s_unistring(mystring);
  //ending null
  s_binary("00 00 ");
  //printf("Size is %d\n",size);
  /*add the padding to make us a word boundary*/
  if (size % 2 !=0)
    {
      //printf ("Padding with 00 00\n");
      s_binary("00 00 ");
      size+=1;
    }
  /*
  while (size % 4 != 0)
    {
      s_binary("00");
      size++;
    }
  */

  
  return size*2+12; 
}

int 
s_dce_string(unsigned char * mystring)
{
  unsigned short size;
  unsigned short size2;
  unsigned int i;
  
  size=strlen(mystring);
  size2=size;
  
  s_intelhalfword(size2);
  for (i=0; i< size2; i++)
    s_push(mystring+i,1);

  while (size % 4 != 0)
    {
      s_binary("00");
      size++;
    }

  return size+2; /*2 for intel half word */
}

int
s_random_dce_string()
{

  /*pick a random length properly*/

  //just in case
  s_init_fuzzing();

#define TOPLENGTH 0xffff
  //top=s_get_max_fuzzstring();
  
  //picked=1+(unsigned short) (top*1.0 *rand()/(RAND_MAX+1.0));

  //length=1+(unsigned short) (top*1.0 *rand()/(RAND_MAX+1.0));

  //randstring=malloc(length+1);
  //memset(randstring,0x41,length);
  //randstring[length]=0;

  /*not sure which one is correct!*/
  s_dce_wordstring(s_get_random_fuzzstring());
  /*s_dce_wordstring(randstring);*/


  //free(randstring);
  return 1;
}

#define TRYNEWBUG

/*requires caller to also use s_block_end("DCEFragLength")*/
int
s_dce_call_header(int callid, int allochint, unsigned short opnum,
		  int first, int last, unsigned char * object)
{

  unsigned char flags;
  /*DCE HEADER*/
  s_block_start("DCEFragLength");

  /*version*/
  s_binary("05");

  /*minor version*/
  s_binary("00");
  
  s_binary(REQUESTPACKET);

  /*set up the flags. DCERPC is pretty simple. FIRST and LAST packets
    are marked with flags, and anything in the middle has 0s*/
  flags=0;
#define OBJECTFLAG 0x80
  if (object!=NULL)
    flags|=OBJECTFLAG;
  if (first)
    flags|=FIRSTFRAG;
  if (last)
    flags|=LASTFRAG;
  s_push(&flags,1);

  /*now set Data Rep, whatever that is. Check dcetest for more info, I
    think. */
  s_intelword(0x00000010);

  /*now we have 2 bytes of frag length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength");
#else
  s_binary_block_size_intel_halfword("DCEFragLength");
#endif

  /*auth length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEAuthLength");
#else
  s_binary_block_size_intel_halfword("DCEAuthLength");
#endif

  /*call ID */
  s_intelword(callid);

  /*alloc hint*/
  s_intelword(allochint);

  /*context ID*/
  s_binary("0000");

  s_intelhalfword(opnum);
  if (object!=NULL)
    { 
      unsigned char buffer[16];
      uuid_string_to_buffer(buffer,object);
      s_push(buffer,sizeof(buffer));
    }
  /*END OF HEADER*/

  s_block_start("DCEAuthLength");
  s_block_end("DCEAuthLength");
  /*caller must end this explicitly*/
  s_block_start("DCEFragLength");

  return 1;
}



/*requires caller to also use s_block_end("DCEFragLength")*/
int
s_dce_call_header_udp(int callid, int allochint, unsigned short opnum,
		      int first, int last, unsigned char * object,
		      unsigned char * ifid, unsigned int ifid_ver, unsigned char * activity_id, 
		      unsigned int fragnum,int idempotent,int nofack)
{
  unsigned char buffer[16];
  unsigned char flags;
  /*DCE HEADER*/

  /*version*/
  s_binary("04");

  s_binary(REQUESTPACKET);

  /*set up the flags. DCERPC is pretty simple. FIRST and LAST packets
    are marked with flags, and anything in the middle has 0s*/
  flags=0;
#define UDPFRAGMENTFLAG 0x04
#define UDPLASTFRAGMENTFLAG 0x02
#define IDEMPOTENTFLAG 0x20
#define NOFACKFLAG 0x08

  /*UDP is a bit different from TCP here*/
  if (!(first && last))
    {
      if (first)
	{
	}
      flags|=UDPFRAGMENTFLAG;
      if (last)
	flags|=UDPLASTFRAGMENTFLAG;
    }

   if (idempotent) {
	flags|=IDEMPOTENTFLAG;
   }
   if (nofack) {
	flags|=NOFACKFLAG;
   }
  s_push(&flags,1);
  s_binary("00"); //flags2

  /*now set Data Rep, whatever that is. Check dcetest for more info, I
    think. */
  s_binary("10 00 00");

  s_binary("00"); //seriel high
    
  if (object!=NULL)
    { 

      uuid_string_to_buffer(buffer,object);
      s_push(buffer,sizeof(buffer));
    }
  else
    {
      //null object ID
      s_binary("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
    }


  uuid_string_to_buffer(buffer,ifid);
  s_push(buffer,sizeof(buffer));

  uuid_string_to_buffer(buffer,activity_id);
  s_push(buffer,sizeof(buffer));
  
  s_binary("00 00 00 00"); //server boot time

  s_intelword(ifid_ver);

  s_intelword(0); //sequence number

  s_intelhalfword(opnum);

  s_intelhalfword(0xffff); //interface hint
  s_intelhalfword(0xffff); //activity hint

  /*now we have 2 bytes of frag length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength");
#else
  s_binary_block_size_intel_halfword("DCEFragLength");
#endif

  s_intelhalfword(fragnum);

  s_binary("0x00"); //auth proto (0)

  s_binary("0x00"); //seriel (0)


  /*END OF HEADER*/

  /*caller must end this explicitly*/
  s_block_start("DCEFragLength");

  return 1;
}



/*requires caller to also use s_block_end("DCEFragLength")*/
int
s_smbdce_call_header(int callid, int allochint, unsigned short opnum,
		  int first, int last, unsigned char * object)
{

  unsigned char flags;
  /*DCE HEADER*/

  /*BUGBUG:???

  For some reason, the DCEFragLength for an SMB DCE call INCLUDES the header information, wheras with a normal DCE over TCP call, it does not!?!?!

  */
  /*caller must end this explicitly*/
  s_block_start("DCEFragLength");

  /*version*/
  s_binary("05");

  /*minor version*/
  s_binary("00");
  
  s_binary(REQUESTPACKET);

  /*set up the flags. DCERPC is pretty simple. FIRST and LAST packets
    are marked with flags, and anything in the middle has 0s*/
#if 1
  flags=0;
#else
  flags=0xff;
#endif

  if (object != NULL)
    {
      flags|=OBJECTFLAG;
    }
  if (first)
    flags|=FIRSTFRAG;
  if (last)
    flags|=LASTFRAG;
  s_push(&flags,1);

  /*now set Data Rep, whatever that is. Check dcetest for more info, I
    think. */
  s_intelword(0x00000010);

//1 to work
#if 1
  /*now we have 2 bytes of frag length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength");
#else
  s_binary_block_size_intel_halfword("DCEFragLength");
#endif
#else
  s_binary("ff 00");
#endif
      
#if 1
  /*auth length*/
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEAuthLength");
#else
  s_binary_block_size_intel_halfword("DCEAuthLength");
#endif
#else
  s_binary("ff ff");
#endif

  /*call ID */
  s_intelword(callid);

#if 1
  /*alloc hint*/
  s_intelword(allochint);
#else
  s_binary("00 00 00 00");
#endif

  /*context ID*/
  s_binary("0000");

  s_intelhalfword(opnum);
  if (object!=NULL)
    { 
      unsigned char buffer[16];
      uuid_string_to_buffer(buffer,object);
      s_push(buffer,sizeof(buffer));
    }
  /*END OF HEADER*/

  s_block_start("DCEAuthLength");
  s_block_end("DCEAuthLength");


  return 1;
}


int
s_send_dce_fragment(unsigned char * buffer, unsigned int length,int first, int last,
		    unsigned short function_number,int callid, unsigned long allochint,int issmb,
		    unsigned char *fid,unsigned char *uid,unsigned char *tid, unsigned char * object, 
		    unsigned char * ifid, unsigned int ifid_ver, unsigned char * activity_id, 
		    unsigned int fragnum)
{
 int idempotent=1;
 int nofack=1;
  spike_clear();
  
  if (issmb)
    {
#ifdef DOWRITEX
      //actually works
      s_smb_writex_header(first,fid,uid,tid);      
#else
      //do a transx header instead of a writex
      //you may want to do this if you are trying to crash the server.
      s_smbheader(fid,uid,tid);
#endif
    }
  
  /*2nd arg is alloc hint*/
  if (issmb)
    {
      s_smbdce_call_header(callid, allochint, function_number,first,last,object);
    }
  else
    {
      if (current_spike->proto==1)
	{
	  //tcp
	  s_dce_call_header(callid, allochint, function_number,first,last,object);
	}
      else if (current_spike->proto==2)
	{
	  //udp
	  s_dce_call_header_udp(callid, allochint, function_number,first,last,object,ifid,ifid_ver,activity_id,fragnum,idempotent,nofack);
	}
    }



  /*
  printf("pushing length %d\n",length);
  */
  s_push(buffer,length);
  s_block_end("DCEFragLength");

  if (issmb)
    {
      /*had to add this for SMB block*/
      s_block_end("smbblock");
      s_block_end("bytecount");
    }
  /*
  printf("Current TCP socket fd,size: %d %d\n",get_spike_fd(),s_get_size());
  */
  if (spike_send()<0)
    {
      printf("Couldn't send dce fragment!\r\n");
      exit(-1);
    }

  /*
  printf("After spike_send\n");
  */
/*sadly, flushes arn't working, so we will use this to flush instead*/
  fflush(0);
  //usleep(500);
  /*future versions will actually parse this response and report errors*/
  return 1;
}

/*takes in a buffer and funnels all that data down to it*/
/*assumes bind has already been done*/
int
s_do_dce_call(unsigned short function_number,unsigned char * buffer, unsigned int length,unsigned char * object,
	      //following for UDP only
	      unsigned char * ifid, unsigned int ifid_ver, unsigned char * activity_id )
{
  struct spike * old_spike;
  struct spike * dce_spike;
  unsigned int left;
  int sendlength;
  unsigned char *p;
  int first, last;
  int done;
  static int callid=140;
  unsigned int fragnum=0;
  
  old_spike=getcurrentspike();
  dce_spike=new_spike();
  setspike(dce_spike);
  /*we assume this is already bound to DCE port*/
  dce_spike->fd=(old_spike->fd);
  dce_spike->proto=old_spike->proto;

  /*now we need to set up the calls*/
  left=length;
  first=1;
  last=0;
  sendlength=MAXDCEFRAGLENGTH;
  p=buffer;
  done=0;

  callid++;

  /*we do a max of 5000 bytes in a call, the windows stack does like
    5480 or something, but whatever*/
  /*now we do 5480, just in case that matters - hmm. freedce seems to want it all part of one tcp packet for some reason. We can do that, it doesn't seem to really matter*/
  while (!done)
    {
      /*send this fragment*/
      if (left<=MAXDCEFRAGLENGTH)
	{
	  if (!first)
	    printf("Doing last\n");
	  /*last fragment*/
	  last=1;
	  sendlength=left;
	}
      
      /*error returns 0*/
      if (!s_send_dce_fragment(p,sendlength,first,last,function_number,
			       callid,left,0,NULL,NULL,NULL,object,
			       ifid,ifid_ver,activity_id,fragnum))
	{
	  printf("Error sending dce fragment\n");
	  return 0;
	}

      /*clumsy, but should work*/
      if (left>MAXDCEFRAGLENGTH)
      {
	left-=MAXDCEFRAGLENGTH;
	p+=MAXDCEFRAGLENGTH;
	first=0;
      }
      else
      {
	/*done with all the fragments*/
	done=1;
      }
      fragnum++;
    }

  setspike(old_spike);
  spike_free(dce_spike);

  return 1;
}




int 
s_smb_negotiate(char * login, char * password)
{
 if(login != NULL && password != NULL)
 
  s_binary(
	"00 00 00 89 FF 53 4D 42 72 00"
	"00 00 00 18 01 20 00 00"
	"00 00 00 00 00 00 00 00"
	"00 00 00 00 00 28 00 00"
	"00 00 00 66 00 02 50 43"
	"20 4E 45 54 57 4F 52 4B"
	"20 50 52 4F 47 52 41 4D"
	"20 31 2E 30 00 02 4D 49"
	"43 52 4F 53 4F 46 54 20"
	"4E 45 54 57 4F 52 4B 53"
	"20 31 2E 30 33 00 02 4D"
	"49 43 52 4F 53 4F 46 54"
	"20 4E 45 54 57 4F 52 4B"
	"53 20 33 2e 30 00 02 4c"
	"41 4e 4d 41 4e 31 2e 30"
	"00 02 4c 4d 31 2e 32 58"
	"30 30 32 00 02 53 61 6d"
	"62 61 00");
	
else
  s_binary(
	"00 00 00 a4 ff 53  4d 42 72 00 00 00 00 08"   
  	"01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00"   
  	"01 26 00 00 01 00 00 81  00 02 50 43 20 4e 45 54"   
  	"57 4f 52 4b 20 50 52 4f  47 52 41 4d 20 31 2e 30"   
  	"00 02 4d 49 43 52 4f 53  4f 46 54 20 4e 45 54 57"   
  	"4f 52 4b 53 20 31 2e 30  33 00 02 4d 49 43 52 4f"  
  	"53 4f 46 54 20 4e 45 54  57 4f 52 4b 53 20 33 2e"   
  	"30 00 02 4c 41 4e 4d 41  4e 31 2e 30 00 02 4c 4d"  
  	"31 2e 32 58 30 30 32 00  02 53 61 6d 62 61 00 02"   
  	"4e 54 20 4c 41 4e 4d 41  4e 20 31 2e 30 00 02 4e"   
  	"54 20 4c 4d 20 30 2e 31  32 00");

  return 1;
}

int
s_smb_session_setup(char * login, char * password)
{
   if(login != NULL && password != NULL)
   {
   char *native_os = "Unix";
   char *native_lanmanager = "SPIKE";
   char *domain = "MYGROUP";
   int extra = strlen(native_os) + strlen(native_lanmanager) + strlen(domain) + 3;
   int len = strlen(login) + strlen(password) + 57 + extra;
   int bcc = 2 + strlen(login) + strlen(password) + extra;
   int len_hi = len / 256;
   int len_lo = len % 256;
   int bcc_hi = bcc / 256;
   int bcc_lo = bcc % 256;
   int pass_len = strlen(password) + 1;
   int pass_len_hi = pass_len / 256;
   int pass_len_lo = pass_len % 256;
   char req[] = {0x00, 0x00,
   		 len_hi, len_lo, 0xFF, 0x53, 0x4D, 0x42, 0x73, 0x00,
		 0x00, 0x00, 0x00, 0x18, 0x01, 0x20, 0x00, 0x00,
	  	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00,
		 0x00, 0x00, 0x0A, 0xFF, 0x00, 0x00, 0x00, 0x04,
	  	 0x11, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	 0x00, pass_len_lo,  pass_len_hi, 0x00, 0x00, 0x00, 0x00, bcc_lo,
	  	 bcc_hi};
		 
   char * t = malloc(len + 4);
   int off = 0;
   
   bzero(t, len + 4);
   memcpy(t, req, sizeof(req)); off += sizeof(req);
   memcpy(t+off, password, pass_len); off += pass_len;
   memcpy(t+off, login,    strlen(login) + 1); off += strlen(login) + 1;
   memcpy(t+off, domain,   strlen(domain) +1); off += strlen(domain) + 1;
   memcpy(t+off, native_os, strlen(native_os) + 1); off += strlen(native_os) + 1;
   memcpy(t+off, native_lanmanager, strlen(native_lanmanager) + 1); off += strlen(native_lanmanager) + 1;
   
    
    s_push(t, len + 4);
    free(t);         
  }
  else
  {		     		     
  s_binary("00 00 00 58 ff 53  4d 42 73 00 00 00 00 08"   
  "01 c0 00 00 00 00 00 00  00 00 00 00 00 00 00 00"  
  "8d 2b 00 00 01 00 0d ff  00 00 00 ff ff 02 00 8d"  
  "2b 00 00 00 00 00 00 00  00 00 00 00 00 54 00 00"   
  "00 1b 00 00 00 00 00 00  55 00 6e 00 69 00 78 00"  
  "00 00 53 00 61 00 6d 00  62 00 61 00 00 00");
  }
  return 1;
}


int
s_smb_ipc_connect(unsigned char *uid)
{

  s_binary(" 00 00 00 3a ff 53  4d 42 75 00 00 00 00 08"   
  "01 c0 00 00 00 00 00 00  00 00 00 00 00 00 00 00"   
  "8d 2b");
  s_push(uid,2);
  s_binary(" 01 00 04 ff  00 00 00 00 00 01 00 0f"   
  "00 00 49 00 50 00 43 00  24 00 00 00 49 50 43 00");
  return 1;
}

int
s_nt_createandx(unsigned char *tid,unsigned char *uid,char * pipe)
{
#if 1
  /*type Session message*/
  s_binary("00");
  /*pad?*/
  s_binary("00");
#endif

#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_halfword_bigendian_variable("smbblock");
#else
  s_binary_block_size_halfword_bigendian("smbblock");
#endif

#else 
  s_binary("f0 00");
#endif
  s_block_start("smbblock");
  s_binary(" ff 53  4d 42 a2 00 00 00 00 08  01 c0 00 00 00 00 00 00  00 00 00 00 00 00  ");
  s_push(tid,2);
  s_binary("8d 2b ");
  s_push(uid,2);
  s_binary("01 00");
  s_binary("18 ff  00 00 00 00 0e 00 00 00"   
  "00 00 00 00 00 00 9f 01  02 00 00 00 00 00 00 00"   
  "00 00 00 00 00 00 03 00  00 00 01 00 00 00 00 00"  
  "00 00 02 00 00 00 00");
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("pipename");
#else
  s_binary_block_size_intel_halfword("pipename");
#endif
#else
  s_binary("00 00");
#endif
  s_block_start("pipename");
  s_binary("00"); /*wish I knew why this pad byte was here...*/
  s_unistring(pipe);
  s_block_end("pipename");
  s_block_end("smbblock");
  return 1;
}



/* sends a netbios and smb header*/
int
s_smbheader(unsigned char *fid,unsigned char *uid,unsigned char *tid)
{
/*type Session message*/
  s_binary("00");
  /*pad?*/
  s_binary("00");
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_halfword_bigendian_variable("smbblock");
#else
  s_binary_block_size_halfword_bigendian("smbblock");
#endif

#else
  s_binary("0001");
#endif
  s_block_start("smbblock");

  s_binary(" ff 53  4d 42 25 00 00 00 00 08  01 c0 00 00 00 00 00 00  00 00 00 00 00 00  ");
  s_push(tid,2);
  s_binary("8d 2b ");
  s_push(uid,2);
  s_binary("01 00");
  s_binary(" 10 00  00 ");
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength"); 
#else
  s_binary_block_size_intel_halfword("DCEFragLength"); 
#endif
#else
  s_binary("00 00");
#endif
  s_binary("00 00");
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength");
#else
  s_binary_block_size_intel_halfword("DCEFragLength");
#endif
#else
  s_binary("0000");
#endif
  s_binary("00 00 00 00 00 00 00 00 00  00 00 00 52 00 ");
  
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("DCEFragLength");
#else
  s_binary_block_size_intel_halfword("DCEFragLength");
#endif
#else
  s_binary(" 00 01");
#endif
  s_binary("52 00 02 00");
  /*SMB PIPE PROTOCOL*/
  s_binary("26 00"); /*transact pipe*/
  s_push(fid,2);
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_variable("bytecount");
#else
  s_binary_block_size_intel_halfword("bytecount");
#endif

#else 
  s_binary("00 01");
#endif

  s_block_start("bytecount");
  /*this pad byte is here because \PIPE must be 4 byte aligned to the front of the SMB packet*/
  /*see page 19 of Luke's book*/
  s_binary("00");
#if 0
  s_unistring("C:\\");
#else
  s_unistring("\\PIPE\\");
#endif


  return 1;
  
}


void
s_smb_writex_header(int first, unsigned char *fid,unsigned char *uid,unsigned char *tid)
{

  /*type Session message*/
#if 1
  s_binary("00");
#else
  s_binary("ff");
#endif

  /*("Add this to size?")*/
#if 1
  s_binary("00");
#else
  s_binary("ff");
#endif

#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_halfword_bigendian_variable("smbblock");
#else
  s_binary_block_size_halfword_bigendian("smbblock");
#endif

#else
  s_binary("0001");
#endif

  s_block_start("smbblock");

  s_binary("ff 53 4d 42 2f 00 00 00 00 18 07 c800 00 40 6d 4e f4 8c 6e 13 7b 00 00");
  /*tree id*/
  s_push(tid,2);
  /*process id*/
  s_binary("ff fe");
  s_push(uid,2);
  /*multiplex id*/
  s_binary("00 01");
  s_binary("0e ff 00 de de ");
  s_push(fid,2);
  s_binary(" 00 00 00 00 ff ff ff ff");
  if (first) {
    s_binary("08 00");
  }
  else
    {
      s_binary("00 00 ");
    }
#if 1
#ifdef FUZZINGDCE
  s_binary_block_size_intel_halfword_plus_variable("bytecount",-1);
#else
  s_binary_block_size_intel_halfword_plus("bytecount",-1);
#endif
#else
  s_binary("00 40");
#endif
  s_binary("0000 ");

#if 1
  s_binary_block_size_intel_halfword_plus("bytecount",-1);
#else 
  s_binary("00 01");
#endif

  /*data offset*/
#if 1
  s_binary("40 00 00 00 00 00 ");
#else
  s_binary("ff ff ff ff  ff ff");
#endif

#if 1
  s_binary_block_size_intel_halfword("bytecount");
#else 
  s_binary("0001");
#endif
  s_block_start("bytecount");
  /*padding*/
  s_binary("ee");
}


int
s_smb_dce_call(unsigned short function_number,unsigned char * buffer, unsigned int length,
	       unsigned char *fid,unsigned char *uid,unsigned char *tid, unsigned char * object)
{
  struct spike * old_spike;
  struct spike * dce_spike;
  unsigned int left;
  int sendlength;
  unsigned char *p;
  int first, last;
  int done;
  static int callid=140;
  unsigned char * ifid;
  unsigned int ifid_ver;
  unsigned char * activity_id;
  unsigned int fragnum;

  old_spike=getcurrentspike();
  dce_spike=new_spike();
  setspike(dce_spike);
  /*do unicode the MS way*/
  s_set_unicode(1);
     
  /*we assume this is already bound to DCE port*/
  dce_spike->fd=(old_spike->fd);
  dce_spike->proto=old_spike->proto;

  /*now we need to set up the calls*/
  left=length;
  first=1;
  last=0;
  sendlength=MAXSMBFRAGLENGTH;
  p=buffer;
  done=0;

  callid++;

  /*we do a max of 5000 bytes in a call, the windows stack does like
    5480 or something, but whatever*/
  /*now we do 5480, just in case that matters - hmm. freedce seems to want it all part of one tcp packet for some reason. We can do that, it doesn't seem to really matter*/
  while (!done)
    {
      /*send this fragment*/
#if 1
      if (left<=MAXSMBFRAGLENGTH)
#else
      if (0)
#endif
	{
	  if (!first)
	    printf("Doing last\n");
	  /*last fragment*/
	  last=1;
	  sendlength=left;
	}

      /*error returns 0*/
      if (!s_send_dce_fragment(p,sendlength,first,last,function_number,
			       callid,left,1,fid,uid,tid,object,
			       ifid,ifid_ver,activity_id,fragnum))
	{
	  printf("Error sending dce fragment\n");
	  return 0;
	}

      /*clumsy, but should work*/
      if (left>MAXSMBFRAGLENGTH)
      {
	left-=MAXSMBFRAGLENGTH;
	p+=MAXSMBFRAGLENGTH;
	first=0;
      }
      else
      {
	/*done with all the fragments*/
	done=1;
      }
    }

  setspike(old_spike);
  spike_free(dce_spike);

  return 1;

}


void
s_smb_readx(unsigned char *fid,unsigned char * uid, unsigned char* tid)
{
  /*netbios header*/
  /*type Session message*/
  s_binary("00");
  /*("Add this to size?")*/
  s_binary("00");
#if 1
  s_binary_block_size_halfword_bigendian("smbblock");
#else
  s_binary("00 01");
#endif
  s_block_start("smbblock");


  s_binary("ff 53 4d 42 2e 00 00 00 00 18 00 80 00 00 00 00 00 00 00 00 00 00 00 00 ");
  s_push(tid,2);
  s_binary("fe ca ");
  s_push(uid,2);
  s_binary("c0 07 0c ff 00 00 00 ");
  s_push(fid,2);
  s_binary("00 00 00 00 d0 03 d0 03 ff ff ff ff d0 03 00 00 00 00 00 00 02");
  s_block_end("smbblock");

}


/*sends a readx packet and then reads the netbios+smb+dce packet response
  returns 0 on success
 */
int
s_smbdce_doreadx(unsigned char *fid,unsigned char * uid, unsigned char* tid,
		 char *buffer,unsigned int buffersize)
{
  char * tmpbuf;
  int ret;

  printf("Reading ...\n");
  spike_clear();
  s_smb_readx(fid,uid,tid);
  spike_send();
  s_fd_wait();
  ret=s_read_bigendian_word_packet(&tmpbuf);
  if (ret==-1)
    {
      return -1;
    }
  if (ret>buffersize)
    {
      return -2;
    }

  ret=0;

  /*check for smb error*/
  if (memcmp(tmpbuf+5,&ret,4)!=0)
    {
      printf ("Read request returned  error %x\n",*((int*)(tmpbuf+5)));
      return -1;
    }

  printf("Got good response from server.\n");
  free(tmpbuf);

  /*0 is success*/
  return 0;
}

/*a random 16 bytes as a string!*/
char *
get_random_uuid()
{
  static unsigned char buffer[16];
  static unsigned char guid[2555];
  int i;
  for (i=0; i<16; i++) {
    buffer[i]=rand();
  }

  guid_rawbuffer_to_string(buffer,guid);
  return guid;
}
