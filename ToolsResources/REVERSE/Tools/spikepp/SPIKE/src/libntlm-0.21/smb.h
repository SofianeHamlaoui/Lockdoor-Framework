typedef unsigned short uint16;
typedef unsigned       uint32;
typedef unsigned char  uint8;

typedef struct
{
uint16  len;
uint16  maxlen;
uint32  offset;
}tSmbStrHeader;

typedef struct
{
char          ident[8];
uint32        msgType;
uint32        flags;
tSmbStrHeader    user;
tSmbStrHeader    domain;
uint8         buffer[1024];
uint32        bufIndex;
}tSmbNtlmAuthRequest;

typedef struct
{
char          ident[8];
uint32        msgType;
tSmbStrHeader    uDomain;
uint32        flags;
uint8         challengeData[8];
uint8         reserved[8];
tSmbStrHeader    emptyString;
uint8         buffer[1024];
uint32        bufIndex;
}tSmbNtlmAuthChallenge;


typedef struct
{
char          ident[8];
uint32        msgType;
tSmbStrHeader    lmResponse;
tSmbStrHeader    ntResponse;
tSmbStrHeader    uDomain;
tSmbStrHeader    uUser;
tSmbStrHeader    uWks;
tSmbStrHeader    sessionKey;
uint32        flags;
uint8         buffer[1024];
uint32        bufIndex;
}tSmbNtlmAuthResponse;

#define SmbLength(ptr) (((ptr)->buffer - (uint8*)(ptr)) + (ptr)->bufIndex)
