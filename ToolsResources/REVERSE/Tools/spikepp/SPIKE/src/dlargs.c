/*dlargs.c
Implementation of the dlargs linked list stuff
*/

#include "dlargs.h"

#include <stdio.h>
#include <stdlib.h>


/*go down the list and free everything*/
void
dlargs_destroy(struct dlargs *mydlargs)
{
  struct dlargs *p,*p2;

  if (mydlargs==NULL)
    {
      return;
    }
  for (p=mydlargs,p2=mydlargs->next; p2!=NULL; p=p2, p2=p2->next)
    {
      if (p->data!=NULL)
	free(p->data);
      free(p);
    }

  free(p);

}

/*constructor for our linked list*/
struct dlargs * 
dlargs_new()
{
  struct dlargs *p;
  p=(struct dlargs *)malloc(sizeof(struct dlargs));
  /*uh oh, didn't check for NULL! Oh well. On windows maybe someday
    we'll worry about that*/
  p->data=NULL;
  p->next=NULL;
  return p;
}


/*adds a variable and datatype to the list*/
int
dlargs_add(int variabletype,struct dlargs *mydlargs, 
	   char * newbuffer)
{
  struct dlargs *tempdlargs=mydlargs;

  //printf("dlargs_add type %d as %s \n",variabletype,newbuffer);

  if (mydlargs==NULL)
    {
      printf("Cannot dlargs_add to NULL\n");
      return 0;
    }

  /*find the last one*/
  while (tempdlargs->next!=NULL) 
    {
      tempdlargs=tempdlargs->next;
    }
  
  tempdlargs->dataType=variabletype;
  tempdlargs->data=newbuffer;
  tempdlargs->next=dlargs_new();
  return 1;
}


int
dlargs_getlength(struct dlargs *mydlargs)
{
  struct dlargs *tempdlargs=mydlargs;
  int i=0;

  tempdlargs=mydlargs;
  /*find the last one*/
  if (tempdlargs==NULL)
    return 0;

  while (tempdlargs->next!=NULL) 
    {
      //printf("counting: %s\n",tempdlargs->data);
      tempdlargs=tempdlargs->next;

      i++;
    }
  
  //printf("returning length of %d\n",i);
  return i;
}
