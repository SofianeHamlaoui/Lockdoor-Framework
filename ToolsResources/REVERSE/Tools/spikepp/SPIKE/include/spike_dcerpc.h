#ifndef SPIKE_DCERPC_H
#define SPIKE_DCERPC_H

#include "spike.h"

#define BINDPACKET "0x0b"
#define REQUESTPACKET "0x00"
#define AUTH3PACKET "0x10"
#define FLAGS "0x03"
#define FIRSTFRAG 0x01
#define LASTFRAG 0x02
#define DCE_BIND_ACK 0x0c
#define DCE_FAULT 0x03
#if 1
#define MAXDCEFRAGLENGTH 5840
#else
#define MAXDCEFRAGLENGTH 0xffe0
#endif
#if 1
#define MAXSMBFRAGLENGTH 800
#else
#define MAXSMBFRAGLENGTH 0xffff
#endif

/*5840 is real length as posed by MS*/

int
s_dce_bind(char * uuid,char * transfer_syntax,
	   unsigned int callid, unsigned int interfaceversionmajor,
	   unsigned int interfaceversionminor,
	   unsigned int syntaxversion);

int 
s_dce_string(unsigned char * mystring);

int
s_dce_call_header(int callid, int allochint, unsigned short opnum, int first, int last, unsigned char * object);

int
s_random_dce_string();

int 
s_dce_wordstring(unsigned char * mystring);

/*takes in a buffer and funnels all that data down to it*/
/*assumes bind has already been done*/


/*takes in a buffer and funnels all that data down to it*/
/*assumes bind has already been done*/
int
s_do_dce_call(unsigned short function_number,unsigned char * buffer, unsigned int length,unsigned char * object,
	      //following for UDP only
	      unsigned char * ifid, unsigned int ifid_ver, unsigned char * activity_id );

/*SMB stuff*/
int 
s_smb_negotiate();

int
s_smb_session_setup();

int
s_smb_ipc_connect(unsigned char *uid);

int
s_nt_createandx(unsigned char *tid,unsigned char *uid,char * pipe);

int
s_smbheader(unsigned char *fid,unsigned char *uid,unsigned char *tid);


int
s_smb_dce_call(unsigned short function_number,unsigned char * buffer, unsigned int length,
	       unsigned char *fid,unsigned char *uid,unsigned char *tid, unsigned char * object);

void
s_smb_writex_header(int first,unsigned char *fid,unsigned char *uid,unsigned char *tid);

int
s_dce_unistring(unsigned char *mystring);

int
s_smbdce_call_header(int callid, int allochint, unsigned short opnum,
		     int first, int last, unsigned char * object);

/*sends a readx packet and then reads the netbios+smb+dce packet response
  returns 0 on success
 */
int
s_smbdce_doreadx(unsigned char *fid,unsigned char * uid, unsigned char* tid,
		 char *buffer,unsigned int buffersize);

/*hmm. we need to do the intel-reversing on this thing*/
int
uuid_string_to_buffer(unsigned char * buffer, char *uuid);


/*a random 16 bytes as a string!*/
char *
get_random_uuid();

#endif
