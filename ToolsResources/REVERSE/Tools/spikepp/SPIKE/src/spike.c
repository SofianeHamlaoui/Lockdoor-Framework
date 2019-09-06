

#include <stdlib.h>
#include <string.h>

/*for select()*/
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <unistd.h>
#include <fcntl.h>

#include <ctype.h>

#include "hdebug.h"
#include "tcpstuff.h"
#include "udpstuff.h"

#include "spike.h"
#include "teststorun.h"

#define SPIKEVERSION 2.9
#define TIMEINSECONDS 0
#define TIMEINUSECONDS 50010
//define this to use long strings
#define LONGSTRINGS
/*undef this for less info*/
#undef DEBUG

/*bug in includes with linux so here's this precurser*/

struct spike * current_spike;       

/*overkill really*/
char **s_fuzzstring=NULL;
int *s_fuzzints=NULL;
static int s_maxfuzzints=0;
static int maxfuzzstring=0;


int
spike_clear()
{
  char * tmp;

  if (current_spike==NULL){
    return 0;
  }
  tmp=realloc(current_spike->databuf,4);
  if (tmp==NULL)
    {
      return 0;
    }

  current_spike->databuf=tmp; /*newly allocated buffer*/  
  current_spike->datasize=0;
  current_spike->currentvariable=0;


  /*current_spike->destsockaddr=NULL;*/
  initListenerArray(current_spike->block_listeners);
  initListenerArray(current_spike->size_listeners);
  return 1;
}

struct spike *
new_spike()
{
  struct spike *outspike;
  outspike=malloc(sizeof(struct spike));
  outspike->databuf=malloc(1);
  outspike->datasize=0;
  outspike->endbuf=outspike->databuf;
  outspike->destsockaddr=NULL;
  initListenerArray(outspike->block_listeners);
  initListenerArray(outspike->size_listeners);
  /*no need for a reserved port yet*/
  outspike->needreserved=0;  
  outspike->do_unicode_ms=0;
  outspike->terminate_unicode=0;

  return (outspike);
}

int
spike_free(struct spike * old_spike)
{
  //printf("Freeing SPIKE (%p:%p)\n",old_spike,old_spike->databuf);
  if (old_spike==current_spike){
    current_spike=NULL;
  }
  if (old_spike==NULL)
    return 0;
  //printf("Before frees\n");
  free(old_spike->databuf);
  free(old_spike);
  //printf("After frees\n");
  return 1;
}

int
setspike(struct spike * newspike)
{
  if (newspike==NULL)
    return 0; /*failure*/
  current_spike=newspike;
  return 1; /*success*/
}

struct spike * 
getcurrentspike()
{
  return current_spike;
}

int
get_spike_fd()
{
  if (current_spike==NULL)
    return -1;
  return current_spike->fd;
}


void
s_set_unicode(int ms)
{
  /*set us up to do it the ms way, if ms is 1*/
  current_spike->do_unicode_ms=1;
  current_spike->terminate_unicode=1;
}

/*update any listening sizes*/
void
s_update_listeners(unsigned long size)
{

  int i;

  for (i=0; i< MAXLISTENERS; i++)
    {
       if (current_spike->block_listeners[i].isused && 
	   current_spike->block_listeners[i].isopen)
	 {
	   current_spike->block_listeners[i].size+=size;
	 }
    }

}


/*
  this pushes an arbitrary byte string onto the spike stack
  and updates any listening blocksizes
*/
int
s_push(unsigned char * pushme, unsigned long size)
{
  unsigned char * tmp;

/*
  printf("Datasize=%lu+%lu\n",current_spike->datasize,size);
*/

  tmp=realloc(current_spike->databuf,current_spike->datasize+size);
  if (tmp==NULL)
    {
      return 0;
    }

  current_spike->databuf=tmp; /*newly allocated buffer*/
  memcpy(current_spike->databuf+current_spike->datasize,pushme,size);
  /*now we update any buffer listening for size information*/
  s_update_listeners(size);
  current_spike->datasize+=size;
  return 1;
}

/*
  inputs a string of hex ("ABCDABCD01010101")
  outputs throwing the string of bytes on the spike stack

  This is a huge function that does a little stupid thing. It better work.
*/
int
s_binary(char * instring)
{
  /*grab code from hydrogen for this*/
  /*TODO*/
  unsigned long size=0;

  unsigned long i=0;

  int done=0;


  int firstnibble=1;
  unsigned char currentnibble,lastnibble;

  unsigned char * outarray;


#ifdef DEBUG
  printf("s_binary entered\n");
#endif

  if (instring==NULL)
    {
      /*DEBUG*/
      /*
	printf("sbinary entered with null instring\n");
      */
      return 0;
    }

  outarray=malloc(4); /*just some basic space*/
  if (outarray==NULL)
    {
      printf("ERROR failed to malloc in s_binary()\n");
      return 0;
    }

  while (!done)
    {
#if 0
   printf("S_binary() at %d\n",i);
#endif
      /*check for the end of the string*/
      if (instring[i]==0x00)
	{
	  done=1;
	  /*DEBUG*/
	  /*
	  printf("done with instring at %d\n",i);
	  */
	  continue;
	}
      /*check for 0x or \x*/
      /*
	chance for one byte overflow here, if string is not
	null terminated. So don't do that.
      */
      if ( (instring[i]=='0' && instring[i+1]=='x') ||
	   (instring[i]=='\\' && instring[i+1]=='x') )
	{
	  i+=2;
	  continue;
	}

      /*clear whitespace*/
      if (isspace(instring[i]))
	{
	  i++;
	  continue;
	}

      /*only hex digits are valid now*/
      if (!isxdigit(instring[i]))
	{
	  printf("Parser failure in s_binary!\n");
	  free(outarray);
	  return 0;
	}

      /*handle a hex digit*/
      if (isdigit(instring[i]))
	currentnibble=instring[i]-'0';
      else
	currentnibble=toupper(instring[i])-'A'+10;

      if (firstnibble)
	{
	  /*this is the first nibble*/
	  firstnibble=0;
	}
      else
	{
	  /*finishing off a whole byte*/
	  size++;
	  outarray=realloc(outarray,size);
	  outarray[size-1]=(lastnibble<<4)+currentnibble;
	  firstnibble=1;
	}
      lastnibble=currentnibble;
      i++; /*next input char please*/
    }

  /*DEBUG*/
  /*
  printf("DEBUG: s_binary() pushing %d bytes\n",size);
  */
  s_push(outarray,size);
  free(outarray);
  return 1; /*success*/
}

/*puts a C style (null terminated) string on the spike*/
void
s_cstring(char *instring)
{
  s_string(instring);
  s_binary("00");
}


/*not a real unicode string, but a pretend unicode string anyways*/
/*e.g. we don't handle non ascii printable characters correctly, since
  that never seems to work out for me.*/
void
s_unistring(char *instring)
{
  int i;
  int len;

  /*BUGBUG: I shouldn't need this, but I do or else s_unistring_variable will
   sometimes crash me*/
  if (instring==NULL)
    return;

#if 0
  printf("s_unistring:\n");
  printf("s_unistring %s\n",instring);
#endif

  len=strlen(instring);

  if (current_spike->do_unicode_ms)
    {
      for (i=0; i<len; i++)
	{
	  s_push(instring+i,1);
	  s_binary("00");
	}
    }
  else
    {
      /*do it the normal way*/
      for (i=0; i<len; i++)
	{
	  s_binary("00");
	  s_push(instring+i,1);
	}
    }

  if (current_spike->terminate_unicode)
    {
      //now do trailing double nulls
      s_binary("0000");
    }

}

/*throws a string onto the stack.*/
int
s_string(char * instring)
{

  unsigned long size;


  if (instring==NULL)
    return 0;

 
  size=strlen(instring);
  if (size!=0) /*no null strings for us*/
    s_push(instring,size); /*won't include trailing zero*/
  
  return 1; /*success*/
}

/*pushes an XDR style string onto the spike stack.
  An XDR style string is an asciiz string with a prepended big
  endian length word and the string itself gets padded with zeros out to
  a word boundry. Many protocols use this sort of thing. Windows, of course,
  needs little endian size fields, but that's another spike_call anyways...
*/
int
s_xdr_string(unsigned char *astring)
{
  unsigned long length;
  int mod;
  int i;
  
  /*null check*/
  if (!astring)
    return 0;

  length=strlen(astring);
  if (length%4==0)
    mod=0;
  else
    mod=4-length%4;

  /*first push the length there*/
  s_bigword(length);
  s_string(astring);

  /*add padding*/
  for (i=0; i<mod; i++)
    {
      s_binary("00"); 
    }
  return 1;
}

int
add_size_listener(int size, char * instring, int type, long plussome, float mult)
{
  listener *myListener;
  char *number_placeholder;
  
#if 0
  printf("Adding new listener. Size=%d, instring=%s, type=%d plussome=%ld mult=%f\n",
	 size,instring,type,plussome,mult);
#endif
  myListener=add_listener(current_spike->size_listeners,instring);
  myListener->sizetype=type;

 
  myListener->bufstart=current_spike->datasize;
  myListener->length=size;
  myListener->size=0;
  myListener->addme=plussome;
  myListener->multme=mult;
  number_placeholder=malloc(size);

  memset(number_placeholder,0x00,size);
  s_push((unsigned char *)number_placeholder,size);
  free(number_placeholder);


  return 1;

}

/*support function for fuzzing integers*/
int
is_int_fuzz_variable(int type)
{
  /*is it our turn?*/
 if (current_spike->fuzzvariable==current_spike->currentvariable)
      {
	/*actually push an integer if it is*/
	s_push_int(s_fuzzints[current_spike->fuzzstring],type);
	current_spike->didlastvariable=1;
	if (current_spike->fuzzstring==s_maxfuzzints)
	  {
	    current_spike->didlastfuzzint=1;
	    current_spike->fuzzstring=0;
	  }
	return 1;
      }
 return 0;
}

/*input: 

  reserves some bytes to throw an ascii number in
  if the block ends on 1 then it becomes 00000000000000000000001 etc.
*/

int
s_blocksize_string(char * instring, int size)
{
#ifdef DEBUG
  printf("s_blocksize string entered\n");
#endif
  return add_size_listener(size,instring,ASCII,0,1);
}

int 
s_blocksize_signed_string_variable(char * instring, int size)
{
  int retval=1;
  if (!is_int_fuzz_variable(ASCII))
    {
      current_spike->didlastvariable=0;
      retval=add_size_listener(size,instring,ASCII,0,1);
    }
  current_spike->firstvariable=0;
  current_spike->currentvariable++;
  return retval;
}

int 
s_blocksize_unsigned_string_variable(char * instring, int size)
{
  int retval=1;
   if (!is_int_fuzz_variable(ASCIIUNSIGNED))
    {
      retval=add_size_listener(size,instring,ASCII,0,1);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;
}

int
s_blocksize_asciihex(char * blockname)
{
  int retval;
  retval=add_size_listener(8,blockname,ASCIIHEX,0,1);
  return retval;
}

int
s_blocksize_asciihex_variable(char * blockname)
{
  int retval=1;
  int size=8;

   if (!is_int_fuzz_variable(ASCIIHEX))
    {
      retval=add_size_listener(size,blockname,ASCIIHEX,0,1);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;
}


int
s_block_start(char *blockname)
{
  listener *myListener ;

#ifdef DEBUG
  printf("s_block_start entered\n");
#endif

  myListener=add_listener(current_spike->block_listeners,blockname);
  if (myListener==NULL)
    return 0; /*failure*/


  return 1; /*success*/
}

int
s_binary_block_size_word_bigendian(char *blockname)
{
#ifdef DEBUG
  printf("s_blocksize_word_bigendian entered\n");
#endif
  return add_size_listener(4,blockname,BINARYBIGENDIAN,0,1);
}

int
s_binary_block_size_word_bigendian_variable(char *blockname)
{
  int retval=1;
   if (!is_int_fuzz_variable(BINARYBIGENDIAN))
    {
      retval=s_binary_block_size_word_bigendian(blockname);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;

}

int
s_binary_block_size_halfword_bigendian(char * blockname)
{
  return add_size_listener(2,blockname,BINARYBIGENDIANHALFWORD,0,1);
}

int
s_binary_block_size_halfword_bigendian_variable(char *blockname)
{
  int retval=1;
   if (!is_int_fuzz_variable(BINARYBIGENDIANHALFWORD))
    {
      retval=s_binary_block_size_halfword_bigendian(blockname);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;

}



int
s_binary_block_size_byte(char * blockname)
{
  return add_size_listener(1,blockname,ONEBYTE,0,1);
}

int
s_binary_block_size_byte_variable(char * blockname)
{
  int retval=1;
   if (!is_int_fuzz_variable(ONEBYTE))
    {
      retval=s_binary_block_size_byte(blockname);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;

}


int
s_binary_block_size_byte_plus(char * blockname, long plus)
{
  return add_size_listener(1,blockname,ONEBYTE,plus,1);
}


int
s_binary_block_size_word_bigendian_plussome(char *blockname, long some)
{
#ifdef DEBUG
  printf("s_blocksize_word_bigendian_plussome entered\n");
#endif
  return add_size_listener(4,blockname,BINARYBIGENDIAN,some,1);
}




int
s_binary_block_size_intel_halfword(char *blockname)
{

  return add_size_listener(2,blockname,BINARYLITTLEENDIANHALFWORD,0,1);
}


int
s_binary_block_size_intel_halfword_variable(char *blockname)
{
  int retval=1;
   if (!is_int_fuzz_variable(BINARYLITTLEENDIANHALFWORD))
    {
      retval=s_binary_block_size_intel_halfword(blockname);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;

}

int
s_binary_block_size_intel_halfword_plus_variable(char *blockname,long plus)
{
  int retval=1;
   if (!is_int_fuzz_variable(BINARYLITTLEENDIANHALFWORD))
    {
      retval=s_binary_block_size_intel_halfword_plus(blockname,plus);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;

}


int
s_binary_block_size_intel_halfword_plus(char *blockname,long plus)
{
  return add_size_listener(2,blockname,BINARYLITTLEENDIANHALFWORD,plus,1);
}


int
s_binary_block_size_byte_mult(char * blockname, float mult)
{
  return add_size_listener(1,blockname,ONEBYTE,0,mult);
}

int
s_binary_block_size_halfword_bigendian_mult(char * blockname, float mult)
{
  return add_size_listener(2,blockname,BINARYBIGENDIANHALFWORD,0,mult);
}



int
s_binary_block_size_word_bigendian_mult(char *blockname, float mult)
{
#ifdef DEBUG
  printf("s_blocksize_word_bigendian_plussome entered\n");
#endif
  return add_size_listener(4,blockname,BINARYBIGENDIAN,0,mult);
}


int
s_binary_block_size_intel_word(char *blockname)
{
  long some=0;
  float mult=1;
  return add_size_listener(4,blockname,INTELENDIANWORD,some,mult);

}

int
s_binary_block_size_intel_word_variable(char *blockname)
{
  int retval=1;
   if (!is_int_fuzz_variable(INTELENDIANWORD))
    {
      retval=s_binary_block_size_intel_word(blockname);
      current_spike->didlastvariable=0;
    }
   current_spike->firstvariable=0;
   current_spike->currentvariable++;
   return retval;

}



int
s_binary_block_size_intel_word_plus(char *blockname,long some)
{
  float mult=1;
  return add_size_listener(4,blockname,INTELENDIANWORD,some,mult);
}

/*YOU CAN'T CALL THIS FROM A .spk!*/
int
s_binary_block_size_word_intel_mult_plus(char *blockname, long some, float mult)
{
#if 1
  printf("mult is %f\n",mult);
#endif

  return add_size_listener(4,blockname,INTELENDIANWORD,some,mult);
}

int
s_binary_block_size_intel_halfword_mult(char *blockname,float mult)
{
  return add_size_listener(2,blockname,BINARYLITTLEENDIANHALFWORD,0,mult);
}



/*pushes the word on the s_stack in big endian format*/
int
s_bigword(unsigned int aword)
{
  unsigned int tempword;
  tempword=htonl(aword);
#ifdef DEBUG
  if (sizeof(tempword)!=4)
    printf("error: sizeof(aword) != 4\n");
#endif
  s_push((unsigned char *)&tempword,sizeof(tempword)); /*better be 4*/
  return 1;
}

int
s_intelword(unsigned int aword)
{
  unsigned int tempword;
  tempword=ioal(aword);
  s_push((unsigned char *)&tempword,sizeof(tempword)); /*better be 4*/
  return 1;
}

/*pushes a short onto the spike in intel order*/
/*also returns the short in ioas format - who knows.*/
int
s_intelhalfword(unsigned short ashort)
{
  unsigned short tempshort;
  unsigned char tempc;
  tempshort=ioas(ashort);

  /*shorts are only two bytes*/
  tempc=(unsigned char)((tempshort >> 0 ) & 0xff);
  s_push(&tempc,1);
  tempc=(unsigned char)((tempshort >> 8 ) & 0xff);
  s_push(&tempc,1);
  return tempshort;
}

/*mult comes before add*/
void
close_a_size(listener *myListener,unsigned long size)
{
  long addme;
#ifdef DEBUG
  printf("close_a_size entered\n");
#endif

  /*mult support*/
  size=size*myListener->multme;
  
  addme=myListener->addme;
  /*wacky signed issues - cludgy and ugly but better stinkin' work*/
  if (addme<0)
    {
      addme=-addme; /*make it positive*/
      size=size-addme;
    }
  else
    size=size+addme;


  switch (myListener->sizetype)
    {
    case ASCIIHEX:
      {
	char tmpbuf[2000];
	char fmt[20];
	/*do we need a special format for long?*/
	sprintf(fmt,"%%8.8x");
	sprintf(tmpbuf,fmt,size);
	/*debug*/
#ifdef DEBUG
	printf("format: *%s*\n",fmt);
	printf("size: *%s*\n",tmpbuf);
#endif
	memcpy(current_spike->databuf+myListener->bufstart,tmpbuf,myListener->length);
      }
      break;
    case ASCII:
      {
	char tmpbuf[2000];
	char fmt[20];
	/*do we need a special format for long?*/
	sprintf(fmt,"%%%lulu",myListener->length);
	sprintf(tmpbuf,fmt,size);
	/*debug*/
#ifdef DEBUG
	printf("format: *%s*\n",fmt);
	printf("size: *%s*\n",tmpbuf);
#endif
	memcpy(current_spike->databuf+myListener->bufstart,tmpbuf,myListener->length);
      }
      break;
    case BINARYBIGENDIAN:
     
      {
	unsigned long tmpint;
	tmpint=htonl(size);
#ifdef DEBUG
	printf("BINARYBIGENDIAN size = %p\n",(void *)tmpint);
#endif
	memcpy(current_spike->databuf+myListener->bufstart,&tmpint,myListener->length);
      }
      break;
    case ONEBYTE:
      {
	unsigned char tmpc;
	tmpc=(unsigned char)size;
	memcpy(current_spike->databuf+myListener->bufstart,&tmpc,myListener->length);
      }
      break;
    case  INTELENDIANWORD:
      {
	unsigned char a[4];
	a[0]=size & 0xff;
	a[1]=(size >> 8) & 0xff;
	a[2]=(size >> 16) & 0xff;
	a[3]=(size >> 24) & 0xff;
	memcpy(current_spike->databuf+myListener->bufstart,a,4);
#if 0
	printf("INTEL WORD: %d %f %x%x%x%x\n",size,(unsigned int)myListener->multme,a[0],a[1],a[2],a[3]);
#endif
	

      }
      break;

    case BINARYLITTLEENDIANHALFWORD:
      {
	/*theoretically this actually works*/
	unsigned short temps;
	unsigned char a,b;
	temps=(unsigned short)size;
	a=temps & 0xff;
	b=(temps >> 8) & 0xff;
	memcpy(current_spike->databuf+myListener->bufstart,&a,1);
	memcpy(current_spike->databuf+myListener->bufstart+1,&b,1);
      }
      break;
    case BINARYBIGENDIANHALFWORD:
      {
	unsigned short temps;
	unsigned char a,b;
	temps=(unsigned short)size;
	b=temps & 0xff;
	a=(temps >> 8) & 0xff;
	memcpy(current_spike->databuf+myListener->bufstart,&a,1);
	memcpy(current_spike->databuf+myListener->bufstart+1,&b,1);
      }
	break;
    default:
      fprintf(stderr,"Something wrong - we don't have that type!\n");
      fprintf(stderr,"This occures when you've closed a block, spike"
		     "went to go fill in size information, and was "
		     "unable to because it did not recognize sizetype\n");
      break;
      
    }
}

void
close_sizes(char * blockname, unsigned long size)
{

  int i;

#ifdef DEBUG
  printf("close_sizes entered\n");
#endif

  for (i=0; i< MAXLISTENERS; i++)
    {

#if 0

      printf("looking at listener %d\n",i);
#endif

      if (current_spike->size_listeners[i].isused )
       {
#if 0
	 printf("looking at listener with mystring %s for %s\n",current_spike->size_listeners[i].mystring,blockname);
#endif
	 if (!strcmp(current_spike->size_listeners[i].mystring,blockname))
	   {
#if 0
	     printf("Closing_a_size i=%d\n",i);
#endif
	     close_a_size(&(current_spike->size_listeners[i]),size);
	   }

       }


       

    }
}

int
s_block_end(char * blockname)
{
  listener *myListener;

#ifdef DEBUG
  printf("s_block_end entered\n");
#endif

  myListener=find_listener(current_spike->block_listeners,blockname);



  if (myListener==NULL)
    {
      /*urg. This should never happen*/
      return 0;
    }

  /*ok, now I've found the block*/
  /*close it*/
  myListener->isopen=0;

  close_sizes(blockname,myListener->size);
  
  return 1;
}

void
s_print_buffer()
{
  unsigned long i;

#ifdef DEBUG
  printf("s_print_buffer entered\n");
#endif

  printf("Datasize=%lu\n",current_spike->datasize);
  printf("Start buffer:\n");
  for (i=0; i< current_spike->datasize; i++)
    {
      printf("%2.2x ",current_spike->databuf[i]);
      if ((i+1)%8==0)
	printf("\n");
    }
  printf("\nEnd buffer:\n");
}

void
s_printf_buffer()
{
  printf("***%s***\n",current_spike->databuf);

}
unsigned long 
s_get_size()
{
  return current_spike->datasize;
}

unsigned char * 
s_get_databuf()
{
  return current_spike->databuf;
}

void
spike_clear_sendto_addr()
{
 if (current_spike->destsockaddr!=NULL)
   {
	free(current_spike->destsockaddr);
 	current_spike->destsockaddr=NULL;
   }
}


/*if you need a reserved source port (<1024) set this to true*/
int
s_set_needreserved(int needreserved)
{
  current_spike->needreserved=needreserved;
  return 1;
}

int
spike_set_sendto_addr(char * hostname, int destport)
{
  struct sockaddr_in *mysockaddr;
  mysockaddr=malloc(sizeof(struct sockaddr_in));
  if (!getHostAddress(hostname,mysockaddr))
      {
        hdebug("spike_set_sendto_addr(): getHostAddr failed\n");
	free(mysockaddr);
        return 0;
      }
  mysockaddr->sin_family=AF_INET;  
  mysockaddr->sin_port = htons(destport);

 spike_clear_sendto_addr();
 
 current_spike->destsockaddr=mysockaddr;
 hdebug("spike_set_sendto_addr returning 1 - success!\n");
 return 1;
}

int
sighandler()
{
  printf("alarm called\n");
  return 0;
}


int
s_send_lines()
     //sends a line at a time, and reads responses
{
  int retval;
  unsigned char * c;
  int size,totalsize;
  int done=0;
  signal(SIGALRM,(void*)sighandler);
  switch (current_spike->proto) 
    {
    case 1: /*TCP*/
      {
      if (current_spike->fd==-1)
	{
	  printf("tried to send to a closed socket!\n");
	  exit(1);
	}
      c=s_get_databuf();
      totalsize=s_get_size();
      if (memchr(c,'\n',totalsize)==NULL)
	    {
	      size=totalsize;
	    }
      else
	{
	  size=(unsigned char *)memchr(c,'\n',totalsize)-c+1;
	}

      do
	{
	  if (totalsize==0)
	    {
	      done=1;
	    }
	  //printf("Size=%d totalsize=%d\n",size,totalsize);
	  alarm(5);
	  retval=tcpwrite(current_spike->fd,size,c);
	  c+=size;
	  totalsize-=size;
	  if (memchr(c,'\n',totalsize)==NULL)
	    {
	      size=totalsize;
	    }
	  else
	    {
	      size=(unsigned char *)memchr(c,'\n',totalsize)-c+1;
	    }

	  //printf("Newsize=%d\n",size);
	  alarm(0);

	  while(s_fd_wait())
	    {
	      //printf ("Reading a line!\n");
	      if (!s_readline())
		break;
	    }
	}   while (!done);
       break;
      }
    case 2: /*UDP*/
      retval=udp_write_data(current_spike->fd, current_spike->destsockaddr, s_get_size(), s_get_databuf());
      break;
 
    }

  /*we don't want the stupid TCP stack under Linux to send the next spike_send packet with this one*/
  fflush(0);

  /*sadly this doesn't work at ALL. We'll comment it out until we figure it out*/
  /*usleep(50);*/

  return retval;
}



/*returns 1 on success, 0 on failure*/
/*right now spike only does tcp and udp*/
int
spike_send()
{
  int retval;

  /*
  printf("spike_send(): current_spike->fd=%d size=%d\n",current_spike->fd,s_get_size());
  */
  switch (current_spike->proto) 
    {
    case 1: /*TCP*/
      if (current_spike->fd==-1)
	{
	  printf("tried to send to a closed socket!\n");
	  exit(1);
	}
      alarm(5);
      signal(SIGALRM,(void*)sighandler);
      retval=tcpwrite(current_spike->fd,s_get_size(),s_get_databuf());
      //printf("Returned from SIGALARM\n");
      signal(SIGALRM,SIG_IGN);
      alarm(0);
      break;
    case 2: /*UDP*/
      retval=udp_write_data(current_spike->fd, current_spike->destsockaddr, s_get_size(), s_get_databuf());
      break;
 
    }

  /*we don't want the stupid TCP stack under Linux to send the next spike_send packet with this one*/
  fflush(0);

  /*sadly this doesn't work at ALL. We'll comment it out until we figure it out*/
  /*usleep(50);*/

  return retval;
}

int
spike_send_udp(char * host, int port)
{
  
  if (!spike_connect_udp(host,port))
    return 0;

  return spike_send();

}

int
spike_send_tcp(char * host, int port)
{
  /*
  printf("spike_send_tcp %s %d\n",host,port);
  */

  if (!spike_connect_tcp(host,port))
    return 0;

  return spike_send();
  /*  return tcpwrite(current_spike->fd,s_get_size(),s_get_databuf());*/

}


//see notes in udpstuff.c
int
spike_connect_udp_ex(char * host, int port, unsigned short local_port)
{
  int fd;
  /*ahh, having udpstuff.c makes this stuff easy*/
  fd=udpconnect_ex(host,port,local_port);
  if (fd==-1)
    {
      fprintf(stderr,"Couldn't udp connect to target\n");
      return (0);
    }

  current_spike->fd=fd;
  current_spike->proto=2; /*UDP*/
  return 1;


}

int 
spike_connect_udp(char * host, int port)
{
  int fd;
  /*ahh, having udpstuff.c makes this stuff easy*/
  fd=udpconnect(host,port);
  if (fd==-1)
    {
      fprintf(stderr,"Couldn't udp connect to target\n");
      return (0);
    }
  current_spike->fd=fd;
  current_spike->proto=2; /*UDP*/
  return 1;
}

int
spike_listen_udp(int port)
{
 int fd;

 if (!make_udp_listener(port,&fd))
   {
	/*success*/
        current_spike->fd=fd;
	current_spike->proto=2;/*udp*/
	return 1;
   }
  return 0;


}

int 
spike_connect_tcp(char * host, int port)
{
  int fd;

  fd=tcpconnect(host,port,current_spike->needreserved);
  if (fd==-1)
    {
      fprintf(stderr,"Couldn't tcp connect to target\n");
      return (0);
    }
  current_spike->fd=fd;
  current_spike->proto=1; /*TCP*/
  return 1;
}

int
s_tcp_accept(int listenfd)
{
  int fd;
  fd=tcp_accept(listenfd);
  if (fd==-1)
    return fd;

  current_spike->fd=fd;
  current_spike->proto=1;
  return fd;
}

void
s_close_udp()
{
 if (current_spike->fd!=-1)
 {
  close(current_spike->fd);
  current_spike->fd=-1;
 }

}

void
spike_close_tcp()
{

  //usleep(50);
  s_fd_clear();
  if (current_spike->fd!=-1) 
    {
      close(current_spike->fd);
      current_spike->fd=-1;
    }
}

int
s_binary_repeat(char *instring, int times) 
{
  for (; times>0; times--)
    s_binary(instring);
  return 1;
}

int
s_string_repeat(char *instring, int times) 
{
  for (; times>0; times--)
    s_string(instring);
  return 1;
}


/** throws an LF (0x0a) terminated string onto the stack.
this is useful for LPD, at least, and likely lots of others.
**/
int
s_string_lf(char * instring) 
{
    unsigned long size;

#ifdef DEBUG
    printf("s_string entered\n");
#endif

    if (instring==NULL)
        return 0;

    size=strlen(instring);

    if (size!=0) { /*no null strings for us*/
        s_push(instring,size); /*won't include trailing
zero*/
        s_binary("0x0a"); /* lf termination */
    }

    return 1; /*success*/
}

/*intel order any long . . . generates an intel byte order long from
  our long format, whatever it is. So if we're on an intel box, all
  good, and if we're on a sparc, all good.*/
unsigned long 
ioal(unsigned long myint)
{
  unsigned long tint;
  tint=htonl(myint);
  intel_order( (char *) &tint, 4);
  return tint;
}

unsigned short
ioas(unsigned short myshort)
{

  unsigned short tshort;
  tshort=htons(myshort);
  intel_order( (unsigned char *)&tshort, 2);
  return tshort;

}



/*Whatever endian you are, you have an intel endian long, 
  and you want it in your format*/
unsigned long
unintel(unsigned long myint)
{
  unsigned long tint;
  /*first, we reverse it*/
  /*
    printf("intel: %d, nonintel %d\n",myint,ntohl(myint));
  */
  tint=ioal(myint);
  /*now, if we're sparc, we do nothing, and if we're intel, we re-reverse it*/
  return (tint);

}



void
intel_order(char * buffer, int length)
{
  char temp[5000];
  int i;

  if (length>sizeof(temp))
    {
      printf("error on intel_order\n");
      return;
    }
    

  memcpy(temp,buffer,length);
  for (i=0; i<length; i++)
    {
      buffer[i]=*(temp+length-i-1);
    }

} 


void
s_fd_clear()
{
  /*Clears the socket of all data*/
  /*theoretically, anyways.*/

  int fd;
  fd_set rfds;
  struct timeval tv;
  int retval;
  char buf[100];

  if (!(s_fd_wait()))
    {

      return;
    }

  while (1)
    {
      fd=current_spike->fd;

      /* Watch server_fd (fd 0) to see when it has input. */
      FD_ZERO(&rfds);
      FD_SET(fd, &rfds);
      /* Wait up to zero seconds  .  will this wait forever? not on linux.*/

      /* from man page: timeout is an upper bound on the amount of time
	 elapsed before select returns. It may be zero, causing select
	 to return immediately.  If timeout is NULL (no timeout), select
	 can block indefinitely. */

      /*wait 2 seconds only*/
      tv.tv_sec = 0;
      tv.tv_usec = 2;

      retval = select(fd+1, &rfds, NULL, NULL, &tv);
      /* Don't rely on the value of tv now! */

      switch (retval)
	{
	case 0:
	  /*Timeout - no packet or keypress*/
	  return;
	case -1:
	  /* ignore interrupted system calls */
	  if (errno != EINTR)
	    {
	      /*some kind of weird select error. Die. */
	      exit(-1);
	    }
	  /*otherwise we got interrupted, so just return false*/
	  return;
	default:
	  {
	    if (FD_ISSET(fd,&rfds))
	      if (read(fd,buf,1)!=-1)
		{
		  return;
		}
	  }
	}
    }
}

int
s_fd_wait()
{
  /*this function does a select to wait for
    input on the fd, and if there
    is, returns 1, else 0 */
  int fd;
  fd_set rfds;
  struct timeval tv;
  int retval;

  fd=current_spike->fd;

  /* Watch server_fd (fd 0) to see when it has input. */
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  /* Wait up to zero seconds  .  will this wait forever? not on linux.*/

  /* from man page: timeout is an upper bound on the amount of time
       elapsed before select returns. It may be zero, causing select
       to return immediately.  If timeout is NULL (no timeout), select
       can block indefinitely. */

  /*wait 2 seconds only*/
  tv.tv_sec = TIMEINSECONDS;
  tv.tv_usec = TIMEINUSECONDS;
  //printf("Before select %d:%d\n",TIMEINSECONDS,TIMEINUSECONDS);
  retval = select(fd+1, &rfds, NULL, NULL, &tv);
  /* Don't rely on the value of tv now! */
  //printf("After select retval=%d.\n",retval);
  switch (retval)
   {
     case 0:
       /*Timeout - no packet or keypress*/
        return(0);
        break;
     case -1:
      /* ignore interrupted system calls */
       if (errno != EINTR)
         {
           /*some kind of weird select error. Die. */
           exit(-1);
         }
      /*otherwise we got interrupted, so just return false*/
       return (0);
       break;
     default:
         {
           if (FD_ISSET(fd,&rfds))
             return (1);
           else
            return (0);
         }
   }
}


/*must be called before doing any string fuzzing*/
void
s_resetfuzzvariable()
{
  if (current_spike==NULL)
    {
      printf("Trying to reset SPIKE which does not exist...");
      return;
    }
  current_spike->fuzzvariable=0;
  current_spike->didlastvariable=0;
  /*probably don't need these next two*/
  current_spike->didlastbeforefuzzstring=0;
  current_spike->didlastafterfuzzstring=0;
  
}

/*true if we've done the last variable. False otherwise. Survives spike_clear()*/
int
s_didlastvariable()
{
  return current_spike->didlastvariable;
}

void
s_resetfuzzstring()
{
  current_spike->fuzzstring=0;
  current_spike->didlastfuzzint=0;
  current_spike->didlastfuzzstring=0;
  current_spike->didlastbeforefuzzstring=0;
  current_spike->didlastafterfuzzstring=0;
}

int
s_didlastfuzzstring()
{
  if (current_spike==NULL)
    {
      printf("No last fuzz string to put in ... ");
      return 0;
    }
  if (current_spike->didlastfuzzint)
    {
      return 1;
    }
  else
    return current_spike->didlastfuzzstring && current_spike->didlastbeforefuzzstring && current_spike->didlastafterfuzzstring;
}

void
s_push_int(int value, int type)
{
  unsigned long size=value;

  switch (type) {
  case ZEROXASCIIHEX:
    {
	char tmpbuf[2000];
	char fmt[20];
	/*do we need a special format for long?*/
	sprintf(fmt,"0x%%8.8x");
	sprintf(tmpbuf,fmt,size);
	/*debug*/
	s_string(tmpbuf);

    }
    break;

  case ASCIIHEX:
    {
      	char tmpbuf[2000];
	char fmt[20];
	/*do we need a special format for long?*/
	sprintf(fmt,"%%8.8x");
	sprintf(tmpbuf,fmt,size);
	/*debug*/
	s_string(tmpbuf);
    }
    break;

  case ASCIIUNSIGNED:
    {
      	char tmpbuf[2000];
	char fmt[20];
	/*do we need a special format for long?*/
	sprintf(fmt,"%%u");
	sprintf(tmpbuf,fmt,size);
	/*debug*/
	s_string(tmpbuf);
    }
    break;
  case ASCII:
    {
	char tmpbuf[2000];
	char fmt[20];
	/*do we need a special format for long?*/
	sprintf(fmt,"%%d");
	sprintf(tmpbuf,fmt,size);
	/*debug*/
	s_string(tmpbuf);
      }
      break;
    case BINARYBIGENDIAN:
        {
	  unsigned long tmpint;
	  tmpint=htonl(size);
	  s_push((unsigned char *)&tmpint,4);
	}
	break;
    case ONEBYTE:
      {
	unsigned char tmpc;
	tmpc=(unsigned char)size;
	s_push(&tmpc,1);
      }
      break;
    case  INTELENDIANWORD:
      {
	unsigned char a[4];
	a[0]=size & 0xff;
	a[1]=(size >> 8) & 0xff;
	a[2]=(size >> 16) & 0xff;
	a[3]=(size >> 24) & 0xff;
	s_push(a,4);
      }
      break;
    case BINARYLITTLEENDIANHALFWORD:
       {
	unsigned short temps;
	unsigned char a,b;
	temps=(unsigned short)size;
	a=temps & 0xff;
	b=(temps >> 8) & 0xff;
	s_push(&a,1);
	s_push(&b,1);
      }
      break;
    case BINARYBIGENDIANHALFWORD:
 {
	unsigned short temps;
	unsigned char a,b;
	temps=(unsigned short)size;
	b=temps & 0xff;
	a=(temps >> 8) & 0xff;
	s_push(&a,1);
	s_push(&b,1);
      }
      break;
  default:
    fprintf(stderr,"unknown type trying to s_push int: %d\n",type);
  }

}



/*use to iterate over an integer*/
void
s_int_variable(int defaultvalue, int type)
{
  if (!is_int_fuzz_variable(type))
    {
      s_push_int(defaultvalue,type);
      current_spike->didlastvariable=0;
    }
    
    current_spike->firstvariable=0;
    current_spike->currentvariable++;
}

/*DOES NOT NULL TERMINATE A STRING!*/
/*add s_binary("00 00"); if you want to do that*/
void
s_unistring_variable(unsigned char *variable)
{
  /*if current variable is the fuzzed variable, then send in a fuzz string, else just send the variable*/
  if (current_spike->fuzzvariable==current_spike->currentvariable && current_spike->fuzzstring!=0 )
    {
      if (current_spike->didlastfuzzstring==1)
	{
	  if (current_spike->didlastbeforefuzzstring==1)
	    {
	      if (current_spike->didlastafterfuzzstring==1)
		{
		  fprintf(stderr,"Did all the fuzz strings. How did we get here?\n");
		  /*sleep cause this is a serious problem of some kind*/
		  sleep(5);
		}
	      else
		{
		  /*printf("Doing after fuzz string\n");*/
		  /*do the after fuzz strings*/
		  s_unistring(variable);
		  s_unistring(s_fuzzstring[current_spike->fuzzstring]);
			
		  current_spike->didlastvariable=1;
		  if (s_fuzzstring[current_spike->fuzzstring+1]==NULL)
		    {
		      current_spike->didlastafterfuzzstring=1;
		      current_spike->fuzzstring=0;
		    }
		}

	    }
	  else
	    {
	      /*printf("Doing before fuzz string\n");*/
	      /*do before fuzz strings*/
	      s_unistring(s_fuzzstring[current_spike->fuzzstring]);
	      s_unistring(variable);
	      /*do the after fuzz strings*/
	      current_spike->didlastvariable=1;
	      if (s_fuzzstring[current_spike->fuzzstring+1]==NULL)
		{
		  current_spike->didlastbeforefuzzstring=1;
		  current_spike->didlastafterfuzzstring=0;
		  current_spike->fuzzstring=0;
		}
	    }

	}
      else
	{
#if 0
	  printf("Doing normal fuzz string\n");
#endif
#if 0
	  printf("String: %d, %s\n",current_spike->fuzzstring,s_fuzzstring[current_spike->fuzzstring]);
#endif
	  /*here is the actual S_STRING call!*/
	  s_unistring(s_fuzzstring[current_spike->fuzzstring]);

	  /*we set this, and it gets unset the next time we do a
	    variable. It works, I guarantee it. :> */
	  current_spike->didlastvariable=1;
	  
	  if (s_fuzzstring[current_spike->fuzzstring+1]==NULL)
	    {
	      current_spike->didlastfuzzstring=1;
	      current_spike->fuzzstring=0;
	    }
	}
    }
  else
    {
      /* DEBUG */
      /*
      printf("Found variable %s\n",variable);
      */
      s_unistring(variable);
      current_spike->didlastvariable=0;

    }

  current_spike->firstvariable=0;
  current_spike->currentvariable++;
}

/*DOES NOT NULL TERMINATE A STRING!*/
void
s_string_variable(unsigned char *variable)
{
int size,size2;
size=current_spike->datasize;
  /*if current variable is the fuzzed variable, then send in a fuzz string, else just send the variable*/
  if (current_spike->fuzzvariable==current_spike->currentvariable && current_spike->fuzzstring!=0)
    {
      if (current_spike->didlastfuzzstring==1)
	{
	  if (current_spike->didlastbeforefuzzstring==1)
	    {
	      if (current_spike->didlastafterfuzzstring==1)
		{
		  fprintf(stderr,"Did all the fuzz strings. How did we get here?\n");
		  /*sleep cause this is a serious problem of some kind*/
		  sleep(5);
		}
	      else
		{
		  /*printf("Doing after fuzz string\n");*/
		  /*sleep(1);*/
		  /*do the after fuzz strings*/
		  s_string(variable);
		  s_string(s_fuzzstring[current_spike->fuzzstring]);
			
		  current_spike->didlastvariable=1;
		  if (s_fuzzstring[current_spike->fuzzstring+1]==NULL)
		    {
		      current_spike->didlastafterfuzzstring=1;
		      current_spike->fuzzstring=0;
		    }
		}

	    }
	  else
	    {
		    /*
	      printf("Doing before fuzz string\n");
	      */
	      /*sleep(1);*/
	      /*do before fuzz strings*/
	      s_string(s_fuzzstring[current_spike->fuzzstring]);
	      s_string(variable);
	      /*do the after fuzz strings*/
	      current_spike->didlastvariable=1;
	      if (s_fuzzstring[current_spike->fuzzstring+1]==NULL)
		{
		  current_spike->didlastbeforefuzzstring=1;
		  current_spike->didlastafterfuzzstring=0;
		  current_spike->fuzzstring=0;
		}
	    }

	}
      else
	{
	  /*printf("Doing normal fuzz string\n");*/
	  /*here is the actual S_STRING call!*/
	  s_string(s_fuzzstring[current_spike->fuzzstring]);

size2=current_spike->datasize;
printf("Variablesize= %d\n",size2-size);
	  /*we set this, and it gets unset the next time we do a
	    variable. It works, I guarantee it. :> */
	  current_spike->didlastvariable=1;
	  
	  if (s_fuzzstring[current_spike->fuzzstring+1]==NULL)
	    {
	      current_spike->didlastfuzzstring=1;
	      current_spike->fuzzstring=0;
	    }
	}
    }
  else
    {
      /* DEBUG */
      /*
      printf("Found variable %s\n",variable);
      */
      s_string(variable);
      current_spike->didlastvariable=0;

    }
  current_spike->firstvariable=0;
  current_spike->currentvariable++;
}


void
s_string_variables(unsigned char splitchar, unsigned char *variables)
{
  char * tempstr;
  char * variable;
  char splitarray[2];
  char * origstr;
  char * beforeequal;
  char * afterequal;
  splitarray[0]=splitchar;
  splitarray[1]=0;

  tempstr=strdup(variables);
  origstr=tempstr;

  /*for each variable, do ... */
  while (tempstr!=NULL)
    {
      /*if not the first variable*/
      if (!current_spike->firstvariable)
	{
	  /*put an ampersand between us and previous because we're not the first*/
	  s_string(splitarray);
	}
      variable=strsep(&tempstr,splitarray);
      afterequal=strdup(variable);
      splitarray[0]='='; /*set to = to split on the variable name*/
      beforeequal=strsep(&afterequal,splitarray);
      s_string(beforeequal);

      /*print an equal sign if we have a variable*/
      if (strlen(beforeequal)>0)
	s_string("=");
      
      /*if we're fuzzing, then use fuzz string, else use what we got*/
      /*we do this even if we don't have anything in afterequal*/
      s_string_variable(afterequal);
      splitarray[0]=splitchar; /*reset to & */
      
      free(beforeequal);
    }
  free(origstr);
}

void
s_incrementfuzzvariable()
{
  current_spike->fuzzvariable++;
}

void
s_incrementfuzzstring()
{
  current_spike->fuzzstring++;
}

/*if envvar is set, pushes that, else pushes realvar*/
void
s_string_or_env(unsigned char *envvar, unsigned char *realvar)
{
  if (getenv(envvar))
    {
      s_string(getenv(envvar));
    }
  else
    {
      s_string(realvar);
    }

}

void
s_add_fuzzint(unsigned long fuzzint)
{
  int i;
  i=s_maxfuzzints++;
  
  s_fuzzints=realloc((void*)s_fuzzints,(s_maxfuzzints+2)*sizeof(unsigned long));
  s_fuzzints[i]=fuzzint;
  
  
}

void
s_add_fuzzstring(unsigned char * newfuzzstring)
{
  int i;


  //for (i=0; s_fuzzstring[i]!=NULL; i++);

  i=maxfuzzstring+1;
  maxfuzzstring++;
  s_fuzzstring=realloc((void*)s_fuzzstring,(maxfuzzstring+2)*sizeof(char *));
  s_fuzzstring[i]=strdup(newfuzzstring);
  s_fuzzstring[i+1]=NULL;

}

void
s_add_long_fuzzchars(unsigned char fuzzchar)
{
  unsigned char buffer[1000001];
  memset(buffer,fuzzchar,sizeof(buffer));
#ifdef LONGSTRINGS
  buffer[1000000]=0;
  //s_add_fuzzstring(buffer);
  buffer[300000]=0;
  //s_add_fuzzstring(buffer);
  buffer[99999]=0;
  //s_add_fuzzstring(buffer);
  buffer[65536]=0;
  //s_add_fuzzstring(buffer);
  buffer[65535]=0;
  //s_add_fuzzstring(buffer);
  buffer[65534]=0;
  s_add_fuzzstring(buffer);
  buffer[32768]=0;
  s_add_fuzzstring(buffer);
  buffer[32767]=0;
  s_add_fuzzstring(buffer);
  buffer[32766]=0;
  s_add_fuzzstring(buffer);
  buffer[32765]=0;
  s_add_fuzzstring(buffer);
  buffer[32764]=0;
  s_add_fuzzstring(buffer);
  buffer[32763]=0;
  s_add_fuzzstring(buffer);
  buffer[32762]=0;
  s_add_fuzzstring(buffer);
  buffer[20000]=0;
  s_add_fuzzstring(buffer);
  buffer[10000]=0;
  s_add_fuzzstring(buffer); 
  buffer[5000]=0;
  s_add_fuzzstring(buffer);
  buffer[4097]=0;
  s_add_fuzzstring(buffer);
  buffer[4096]=0;
  s_add_fuzzstring(buffer);
  buffer[4095]=0;
  s_add_fuzzstring(buffer);
  buffer[2048]=0;
  s_add_fuzzstring(buffer);
  buffer[1024]=0;
  s_add_fuzzstring(buffer);
  buffer[1023]=0;
  s_add_fuzzstring(buffer);
#endif
  buffer[512]=0;
  s_add_fuzzstring(buffer);
  buffer[420]=0;
  s_add_fuzzstring(buffer);


  buffer[257]=0;
  s_add_fuzzstring(buffer);
  buffer[256]=0;
  s_add_fuzzstring(buffer);
  buffer[240]=0;
  s_add_fuzzstring(buffer);
  buffer[128]=0;
  s_add_fuzzstring(buffer);
}


/*here are a bunch of interesting integers!*/
void
init_fuzz_ints()
{
  int i;
  s_fuzzints=malloc(40);
  s_add_fuzzint(0);
  for (i=0; i<255; i++)
    s_add_fuzzint(i);

  for (i=0; i<100; i++)
    s_add_fuzzint(-1*i);

  s_add_fuzzint(0x7F000000);
  s_add_fuzzint(0x7effffff);
  s_add_fuzzint(65535);
  s_add_fuzzint(65534);
  s_add_fuzzint(65536);
  s_add_fuzzint(536870912);


}

/*initializes a bunch of strings and throws them into s_fuzzstring,
  which is a global array of strings, ending with a null*/
void
s_init_fuzzing()
{
  char buffer[1000000];
  int i;
  
#if 0
  printf("S_init_fuzzing called\n");

#endif

  if (maxfuzzstring!=0)
    //done
    return;

  init_fuzz_ints();

  /*first we init it*/
  s_fuzzstring=malloc(40);
  maxfuzzstring=0;

#ifdef LONGSTRINGS 
  /*weird dce-rpc for the locator service, from MS pages ...*/
  strcpy(buffer,"/.:/");
  memset(buffer+strlen(buffer),0x41,5000);
  buffer[5005]=0;
  s_add_fuzzstring(buffer);
  /* */

  strcpy(buffer,"/.../");
  memset(buffer+strlen(buffer),0x41,5000);
  buffer[5005]=0;
  s_add_fuzzstring(buffer);

  /*172 if we stop here*/
#endif

  s_add_fuzzstring("/.../.../.../.../.../");

  s_add_fuzzstring("C:\\");
  s_add_fuzzstring("C:");
  s_add_fuzzstring("C:\\$Mft");
  s_add_fuzzstring("../../../../../../../../../../../../etc/hosts%00");
  s_add_fuzzstring("../../../../../../../../../../../../etc/hosts");
  s_add_fuzzstring("../../../../../../../../../../../../etc/passwd%00");
  s_add_fuzzstring("../../../../../../../../../../../../etc/passwd");
  s_add_fuzzstring("../../../../../../../../../../../../etc/shadow%00");
  s_add_fuzzstring("../../../../../../../../../../../../etc/shadow");
  s_add_fuzzstring("../../../../../../../../../../../../boot.ini%00");
  s_add_fuzzstring("../../../../../../../../../../../../boot.ini");
  s_add_fuzzstring("../../../../../../../../../../../../localstart.asp%00");
  s_add_fuzzstring("../../../../../../../../../../../../localstart.asp");
  s_add_fuzzstring("//../../../../../../etc/passwd");
  s_add_fuzzstring("..:..:..:..:..:..:..:..");
  s_add_fuzzstring("../../../../../../../winnt/system32/ipconfig.exe");
  s_add_fuzzstring("../../../../../../../winnt/system32/");
  s_add_fuzzstring("/localstart.asp%20"); /*old htw trick*/
  s_add_fuzzstring("immunitysec.com");
  s_add_fuzzstring(".immunitysec.com");
  s_add_fuzzstring("\\\\*");
  s_add_fuzzstring("\\\\?\\");

#ifdef LONGSTRINGS
  for (i=0;i<5000;i+=2){
    buffer[i]='/';
    buffer[i+1]='\\';
  }
  buffer[i]=0;
  s_add_fuzzstring(buffer);

  for (i=0;i<5000;i+=2){
    buffer[i]='/';
    buffer[i+1]='.';

  }
  buffer[i]=0;
  s_add_fuzzstring(buffer);
  for (i=0;i<5000;i+=2){
    buffer[i]='/';
    buffer[i+1]='.';

  }
  buffer[i]=0;
  s_add_fuzzstring(buffer);


  for (i=0;i<5000;i+=2){
    buffer[i]='/';
    buffer[i+1]=':';
  }
  buffer[i]=0;
  s_add_fuzzstring(buffer);

  /*add AAAA....AAA:\ to crash abyss server, maybe others*/
  for (i=0;i<0x800;i+=1){
     buffer[i]='A';
  }


  buffer[i]=':';
  buffer[i+1]='\\';
  buffer[i+2]=0;
  s_add_fuzzstring(buffer);
#endif






  strcpy(buffer,"%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%00");
  s_add_fuzzstring(buffer);

  strcpy(buffer,"%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%255cetc%255chosts");
  s_add_fuzzstring(buffer);

  strcpy(buffer,"%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%255cboot.ini");
  s_add_fuzzstring(buffer);

  strcpy(buffer,"/%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%00");
  s_add_fuzzstring(buffer);

 strcpy(buffer,"/%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..winnt/desktop.ini");
  s_add_fuzzstring(buffer);

/*some numbers*/
   s_add_fuzzstring("65536");
   s_add_fuzzstring("0xfffffff");
   s_add_fuzzstring("fffffff");
   s_add_fuzzstring("268435455");
   s_add_fuzzstring("1");
   s_add_fuzzstring("0");
   s_add_fuzzstring("-1");
   s_add_fuzzstring("-268435455");
   s_add_fuzzstring("4294967295");
   s_add_fuzzstring("-4294967295");
   s_add_fuzzstring("4294967294");
   s_add_fuzzstring("-20");
   s_add_fuzzstring("536870912");



#ifdef BUFFEROVERFLOWTESTS
#ifdef LONGSTRINGS
  buffer[0]=0;
  for (i=0; i<1200; i++)
   {
     strcat(buffer,"../");
   }
  s_add_fuzzstring(buffer);

  buffer[0]=0;
  for (i=0; i<1200; i++)
   {
     strcat(buffer,"./");
   }
  s_add_fuzzstring(buffer);

  buffer[0]=0;
  for (i=0; i<1200; i++)
   {
     strcat(buffer,"%25n");
   }
  s_add_fuzzstring(buffer);

  
  strcpy(buffer,"localhost");
  s_add_fuzzstring(buffer); 
#endif
  /*add a ton of long strings*/
  /*takes a long long time to fuzz with this on.*/
#ifdef TRYALLCHARS
  for (i=0; i< 255; i++)
    s_add_long_fuzzchars(i);
#else
/*the basics*/
  s_add_long_fuzzchars('A');
  s_add_long_fuzzchars('B'); //for heap overflows on win32
  s_add_long_fuzzchars('1');
  s_add_long_fuzzchars('<');
  s_add_long_fuzzchars('>');
  s_add_long_fuzzchars('"');
  s_add_long_fuzzchars('/');
  s_add_long_fuzzchars('\\');
  s_add_long_fuzzchars('?');
  s_add_long_fuzzchars('=');
  s_add_long_fuzzchars('&');
  s_add_long_fuzzchars('.');
  s_add_long_fuzzchars('(');
  s_add_long_fuzzchars(')');
  s_add_long_fuzzchars(']');
  s_add_long_fuzzchars('[');
  s_add_long_fuzzchars('%');
  s_add_long_fuzzchars('*');
  s_add_long_fuzzchars('-');
  s_add_long_fuzzchars('+');
  s_add_long_fuzzchars('{');
  s_add_long_fuzzchars('}');
  s_add_long_fuzzchars('\'');
  s_add_long_fuzzchars('\x14');

  /*these will get expanded to 4 characters under utf16*/
  s_add_long_fuzzchars('\xfe');
  s_add_long_fuzzchars('\xff');
#endif /*all longs*/

#endif /*buffer overflow tests*/

#ifdef FORMATSTRINGTESTS
  /*various format string tests*/
  strcpy(buffer,"%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n");
  s_add_fuzzstring(buffer);
  s_add_fuzzstring("%.50000x");
#ifdef LONGSTRINGS
  for(i=0; i<1500; i++)
    strcat(buffer,"%n");
  s_add_fuzzstring(buffer);




  strcpy(buffer,"%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n%25n");
  s_add_fuzzstring(buffer);
#endif
#endif /*FORMATSTRINGTESTS*/

  /*one X*/
  buffer[0]='X';
  buffer[1]=0;
  s_add_fuzzstring(buffer);

  buffer[0]='*'; /*a single star*/
  s_add_fuzzstring(buffer);

  buffer[0]='.';
  s_add_fuzzstring(buffer);
  buffer[0]='/';
  s_add_fuzzstring(buffer);
  buffer[0]='$';
  s_add_fuzzstring(buffer);
  buffer[0]='-';
  s_add_fuzzstring(buffer);
  /*nothing*/
  buffer[0]=0;
  s_add_fuzzstring(buffer);
  s_add_fuzzstring("/%00/"); /* old roxen bug */
  s_add_fuzzstring("%00/");
  s_add_fuzzstring("%00");
  s_add_fuzzstring("%u0000"); //just flailing on this one

  /*old SEARCH issue with IIS 5.0*/
  s_add_fuzzstring("Select \"DAV:displayname\" from scope()");

  /*Any IP will do, we look for long pauses to see if it works*/
  s_add_fuzzstring("\\\\24.3.19.135\\C$\\asdf"); /*dave's old ip. So sad.*/

#ifdef WEIRDCHARINJECTION
  /*Shell Character Injection Stuff*/
  strcpy(buffer,"1;SELECT%20*");
  s_add_fuzzstring(buffer);

  strcpy(buffer,"'sqlattempt1");
  s_add_fuzzstring(buffer);
  strcpy(buffer,"(sqlattempt2)"); /*people forget this one a lot*/
  s_add_fuzzstring(buffer);
  strcpy(buffer,"OR%201=1");
  s_add_fuzzstring(buffer);

/*some shell escapte character fun*/
  strcpy(buffer,";read;");
  s_add_fuzzstring(buffer);
  strcpy(buffer,";netstat -a;");
  s_add_fuzzstring(buffer);
  strcpy(buffer,"\nnetstat -a\n"); /*people miss this one a lot*/
  s_add_fuzzstring(buffer);
  strcpy(buffer,"\"hihihi");
  s_add_fuzzstring(buffer);
  strcpy(buffer,"|dir");
  s_add_fuzzstring(buffer);
  strcpy(buffer,"|ls");
  s_add_fuzzstring(buffer);
  //s_add_fuzzstring("+%00");

/*people forget these two ways of breaking out of shells*/
  strcpy(buffer,"%20$(sleep%2050)");
  s_add_fuzzstring(buffer);
 
  strcpy(buffer,"%20'sleep%2050'");
  s_add_fuzzstring(buffer);

  strcpy(buffer,"!@#$%%^#$%#$@#$%$$@#$%^^**(()");
  s_add_fuzzstring(buffer);
  strcpy(buffer,"%01%02%03%04%0a%0d%0aADSF");
  s_add_fuzzstring(buffer);
  //s_add_fuzzstring("Bob's%20Foot"); /*dave aitel's traditional sql string*/

#endif /*WEIRD CHAR INJECTION*/
  printf("Total Number of Strings is %d\n",maxfuzzstring);
}

/*controls when we put an ampersand*/
void
s_setfirstvariable()
{
  current_spike->firstvariable=1;
}

/*
      if ((response=s_scan_for_variable(buffer,"WWW-Authenticate: NTLM ","\r\n"))!=NULL)

As you can see, s_scan_for_variable(inputbuffer, start, end) tries to match a substring in a buffer separated by two strings. It returns null on failure to match.
*/

unsigned char *
s_scan_for_variable(unsigned char * buffer, unsigned char *startdelim, 
		    unsigned char * enddelim)
{

  unsigned char *found, *found2, *retstring;

  found=strstr(buffer,startdelim);
  if (found==NULL)
    {
      /*failure*/
      return NULL;
    }
  
  found+=strlen(startdelim); /*skip over this stuff and get to the meat*/

  found2=strstr(found,enddelim);
  if (found2==NULL)
    {
      /*failure*/
      return NULL;
    }

  /*now we need to make a new string that copies from found to found2*/
  retstring=malloc(found2-found+1);
  memset(retstring,0x00,found2-found+1); /*this takes care of trailing null*/
  memcpy(retstring, found,found2-found); /*copy data*/
  return retstring; /*better free this later - garbage collection would be nice*/
  
  
}




void
s_read_packet()
{
  unsigned char buffer[5000];
  int i;
  int size;
  s_fd_wait();
  printf("Reading packet\n");
  memset(buffer,0x00,sizeof(buffer));
  alarm(1);
  fcntl(current_spike->fd, F_SETFL, O_NONBLOCK);
  size=read(current_spike->fd,buffer,1500);
  fcntl(current_spike->fd, F_SETFL, 0);
  alarm(0);

  for (i=0; i<size; i++)
    {
      if (isprint(buffer[i]))
	printf("%c",buffer[i]);
      else
	printf("[%2.2x]",buffer[i]);
    }

  printf("\nDone with read\n");
}

unsigned int
s_get_max_fuzzstring()
{
  return maxfuzzstring;
}

unsigned char *
s_get_fuzzstring(unsigned int index)
{
  
  return s_fuzzstring[index];
}

unsigned char *
s_get_random_fuzzstring()
{
  unsigned int index;

  index=1+(unsigned short) (s_get_max_fuzzstring()*1.0 *rand()/(RAND_MAX+1.0));


  return s_fuzzstring[index];
}





/*Read a packet deliminated in a bigendian word size, then packet data*/
/*returns -1 on error, else, size of data read*/
/*puts data read into the buf, which it mallocs. Be sure to free this.*/
int
s_read_bigendian_word_packet(char **buf)
{
#define MAXSIZE 50000

  unsigned char buffer[4];
  unsigned int size,newsize;
  unsigned char *mybuf;
  size=read(current_spike->fd,buffer,4);
  if (size<4)
    {
      return -1;
    }

  /*now get size*/
  size=*((unsigned int*)buffer);
  size=ntohl(size);
  if (size>MAXSIZE)
    {
      return -1;
    }
  mybuf=malloc(size);
  newsize=read(current_spike->fd,mybuf,size);
  if (size!=newsize)
    {
      free(mybuf);
      return -1;
    }
  *buf=mybuf;
  return 0;
}

int
s_readline() 
{
  unsigned char buf[5000];
  int i=0;
  int size;
  //printf("reading a line\n");
  if (!s_fd_wait())
	  return;
  while (i<4999) {
	size=read(current_spike->fd,buf+i,1);
	if (size!=1) {
		return 0;
  	}
	if (buf[i]=='\n')
	{
		buf[i+1]=0;
		printf("line read=%s",buf);
		return strlen(buf);
	}
	i++;
  }
  
  return strlen(buf);
}
