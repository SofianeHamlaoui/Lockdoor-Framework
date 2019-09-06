/*listener.h*/

#ifndef LISTENER_H
#define LISTENER_H

#include <stdio.h>
#include <netinet/in.h> /*sockaddr_in*/

/*here are our states of being*/
#define UNUSED 0
#define USED  1

/*defining our other states here*/
#define BINARYBIGENDIAN 1
#define ASCII           2
#define ONEBYTE         3
#define BINARYLITTLEENDIANHALFWORD 4
#define BINARYBIGENDIANHALFWORD 5
#define ZEROXASCIIHEX 6
#define ASCIIHEX      7
#define ASCIIUNSIGNED 8
#define INTELENDIANWORD 9


/*an arbitrary limit is for fools, but such is I.*/
#define MAXLISTENERS 50

typedef struct listener {
  int isused;
  int isopen;
  unsigned long size;
  char mystring[5000]; /*just for the names of blocks*/
  int sizetype;
  unsigned long bufstart;
  unsigned long length;
  long addme; /*for close_a_size and plussome*/
  float multme; /*ditto*/
} listener;

/*used to initialize a listenerArray*/
void
initListenerArray(listener *listenerArray);

/*used internally sometimes*/
listener *
find_unused_listener(listener * myList);


/*returns a pointer to the listener we added, or NULL on failure*/
listener *
add_listener(listener * myListeners,char * astring);

/*clears a lsitener after you're done with it*/
void
clear_listener(listener *myListener);

listener *
find_listener(listener * listenerArray,char * astring);
#endif
