#ifndef DLRPC_H
#define DLRPC_H

#include "dlargs.h"

#define MAXLINESIZE 20000
#define MAXVARIABLESIZE 15000


#define PARSEERROR 4
#define ENDOFLINE 5
#define ENDOFFUNCTION 6
#define SUCCESS 7
#define SUCCESSSTRING 8
#define SUCCESSINT 9
#define NOTINT 10
#define ENDOFFILE 11
#define COMMENTLINE 12
#define NEWFUNCTION 13
#define INFUNCTION 14
#define OLDFUNCTION 15
#define INTTYPE 16
#define UCHARTYPE 17
#define INSIDEQUOTES 18
#define FAILURE 19
#define PUCHARTYPE 20
#define SUCCESSCHAR 21

int
s_parse(char *filename);

/*I guess for now we just throw away the return value*/
/*for Helium, we'll have to fix this - should be void *, 
actually*/
void
dlrpc_call(struct dlargs *mydlargs);

int 
s_parse_buffer(unsigned char * buffer);

#endif


