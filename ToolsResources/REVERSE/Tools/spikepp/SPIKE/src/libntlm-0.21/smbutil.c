#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "ntlm.h"
#include "smbencrypt.h"
#include "smbbyteorder.h"

char versionString[] ="libntlm version 0.21";

/* Utility routines that handle NTLM auth structures. */

/* The [IS]VAL macros are to take care of byte order for non-Intel
 * Machines -- I think this file is OK, but it hasn't been tested.
 * The other files (the ones stolen from Samba) should be OK.
 */


/* I am not crazy about these macros -- they seem to have gotten
 * a bit complex.  A new scheme for handling string/buffer fields
 * in the structures probably needs to be designed
 */

#define AddBytes(ptr, header, buf, count) \
{ \
if (buf && count) \
  { \
  SSVAL(&ptr->header.len,0,count); \
  SSVAL(&ptr->header.maxlen,0,count); \
  SIVAL(&ptr->header.offset,0,((ptr->buffer - ((uint8*)ptr)) + ptr->bufIndex)); \
  memcpy(ptr->buffer+ptr->bufIndex, buf, count); \
  ptr->bufIndex += count; \
  } \
else \
  { \
  ptr->header.len = \
  ptr->header.maxlen = 0; \
  SIVAL(&ptr->header.offset,0,ptr->bufIndex); \
  } \
}

#define AddString(ptr, header, string) \
{ \
char *p = string; \
int len = 0; \
if (p) len = strlen(p); \
AddBytes(ptr, header, ((unsigned char*)p), len); \
}

#define AddUnicodeString(ptr, header, string) \
{ \
char *p = string; \
unsigned char *b = NULL; \
int len = 0; \
if (p) \
  { \
  len = strlen(p); \
  b = strToUnicode(p); \
  } \
AddBytes(ptr, header, b, len*2); \
}


#define GetUnicodeString(structPtr, header) \
unicodeToString(((char*)structPtr) + IVAL(&structPtr->header.offset,0) , SVAL(&structPtr->header.len,0)/2)
#define GetString(structPtr, header) \
toString((((char *)structPtr) + IVAL(&structPtr->header.offset,0)), SVAL(&structPtr->header.len,0))
#define DumpBuffer(fp, structPtr, header) \
dumpRaw(fp,((unsigned char*)structPtr)+IVAL(&structPtr->header.offset,0),SVAL(&structPtr->header.len,0))


static void dumpRaw(FILE *fp, unsigned char *buf, size_t len)
  {
  int i;
  
  for (i=0; i<len; ++i)
    fprintf(fp,"%02x ",buf[i]);
    
    fprintf(fp,"\n");
  }

static char *unicodeToString(char *p, size_t len)
  {
  int i;
  static char buf[1024000];

  assert(len+1 < sizeof buf);
  
  for (i=0; i<len; ++i)
    {  
    buf[i] = *p & 0x7f;
    p += 2;
    }

  buf[i] = '\0';
  return buf;
  }

static unsigned char *strToUnicode(char *p)
  {
  static unsigned char buf[1024000];
  size_t l = strlen(p);
  int i = 0;
  
  assert(l*2 < sizeof buf);
  
  while (l--)
    {
    buf[i++] = *p++;
    buf[i++] = 0;
    }
  
  return buf;
  }

static unsigned char *toString(char *p, size_t len)
  {
  static unsigned char buf[1024000];
  
  assert(len+1 < sizeof buf);
  
  memcpy(buf,p,len);
  buf[len] = 0;
  return buf;
  }

void dumpSmbNtlmAuthRequest(FILE *fp, tSmbNtlmAuthRequest *request)
  {
  fprintf(fp,"NTLM Request:\n");
  fprintf(fp,"      Ident = %s\n",request->ident);
  fprintf(fp,"      mType = %d\n",IVAL(&request->msgType,0));
  fprintf(fp,"      Flags = %08x\n",IVAL(&request->flags,0));
  fprintf(fp,"       User = %s\n",GetString(request,user));
  fprintf(fp,"     Domain = %s\n",GetString(request,domain));
  }

void dumpSmbNtlmAuthChallenge(FILE *fp, tSmbNtlmAuthChallenge *challenge)
  {
  fprintf(fp,"NTLM Challenge:\n");
  fprintf(fp,"      Ident = %s\n",challenge->ident);
  fprintf(fp,"      mType = %d\n",IVAL(&challenge->msgType,0));
  fprintf(fp,"     Domain = %s\n",GetUnicodeString(challenge,uDomain));
  fprintf(fp,"      Flags = %08x\n",IVAL(&challenge->flags,0));
  fprintf(fp,"  Challenge = "); dumpRaw(fp, challenge->challengeData,8);
  }

void dumpSmbNtlmAuthResponse(FILE *fp, tSmbNtlmAuthResponse *response)
  {
  fprintf(fp,"NTLM Response:\n");
  fprintf(fp,"      Ident = %s\n",response->ident);
  fprintf(fp,"      mType = %d\n",IVAL(&response->msgType,0));
  fprintf(fp,"     LmResp = "); DumpBuffer(fp,response,lmResponse);
  fprintf(fp,"     NTResp = "); DumpBuffer(fp,response,ntResponse);
  fprintf(fp,"     Domain = %s\n",GetUnicodeString(response,uDomain));
  fprintf(fp,"       User = %s\n",GetUnicodeString(response,uUser));
  fprintf(fp,"        Wks = %s\n",GetUnicodeString(response,uWks));
  fprintf(fp,"       sKey = "); DumpBuffer(fp, response,sessionKey);
  fprintf(fp,"      Flags = %08x\n",IVAL(&response->flags,0));
  }

void buildSmbNtlmAuthRequest(tSmbNtlmAuthRequest *request, char *user, char *domain)
  {
    char *u = strdup(user);
    char *p = strchr(u,'@');
    
    if (p)
      {
        if (!domain) 
          domain = p+1;
        *p = '\0';
      }
    
    request->bufIndex = 0;
    memcpy(request->ident,"NTLMSSP\0\0\0",8);
    SIVAL(&request->msgType,0,1);
    SIVAL(&request->flags,0,0x0000b207);  /* have to figure out what these mean */
    AddString(request,user,u);
    AddString(request,domain,domain);
    free(u);
  }

void buildSmbNtlmAuthResponse(tSmbNtlmAuthChallenge *challenge, tSmbNtlmAuthResponse *response, char *user, char *password)
  {
    uint8 lmRespData[24];
    uint8 ntRespData[24];
    char *d = strdup(GetUnicodeString(challenge,uDomain));
    char *domain = d;
    char *u = strdup(user);
    char *p = strchr(u,'@');
    
    if (p)
      {
        domain = p+1;
        *p = '\0';
      }
    
    SMBencrypt(password,   challenge->challengeData, lmRespData);
    SMBNTencrypt(password, challenge->challengeData, ntRespData);
    
    response->bufIndex = 0;
    memcpy(response->ident,"NTLMSSP\0\0\0",8);
    SIVAL(&response->msgType,0,3);
    
    AddBytes(response,lmResponse,lmRespData,24);
    AddBytes(response,ntResponse,ntRespData,24);
    AddUnicodeString(response,uDomain,domain);
    AddUnicodeString(response,uUser,u);
    AddUnicodeString(response,uWks,u);
    AddString(response,sessionKey,NULL);
  
    response->flags = challenge->flags;
    
    free(d);
    free(u);
  }
    
