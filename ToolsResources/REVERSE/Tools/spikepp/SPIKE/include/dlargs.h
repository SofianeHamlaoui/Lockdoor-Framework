/*dlargs.h, where we define the dlargs structure (a linked list)
  and the functions used to play with it*/

#ifndef DLARGS_H
#define DLARGS_H

/*the first "data" is the function name, and the first datatype is
  the return dataType*/

struct dlargs {
  struct dlargs * next;
  unsigned char * data;
  int dataType;
} ;


void
dlargs_destroy(struct dlargs *mydlargs);

int
dlargs_add(int variabletype,struct dlargs *mydlargs, 
	   char * newbuffer);

/*constructor for our linked list*/
struct dlargs * 
dlargs_new();

/*length of the linked list*/
int
dlargs_getlength(struct dlargs *mydlargs);


#define VOIDRETURNTYPE 1
#define INTRETURNTYPE 2


#endif
