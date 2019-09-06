/*parses a spike script file and runs it*/
/*
  Copyright Dave Aitel 2002.
  Released under GPL v 2.0
*/

/*
  TODO:
  Test: multi-line quoted strings
  FIX: \n into printf's and things?
*/



#include <ctype.h> /*for isdigit()*/
#include <string.h>

/*for exit()*/
#include <unistd.h>
#include <stdlib.h>

/*dlopen()*/
#include <dlfcn.h>

/*perreor()*/
#include <stdio.h>
#include <errno.h>


#include "dlrpc.h"
#include "spike.h"
#include "dlargs.h"

//DEFINE THIS TO DEBUG DLRPC PARSER
//#define DEBUG



/*my job is to take data and transform it into an argument!*/
int
dldemarshall(void **arg,int dataType, unsigned char *data)
{
  //printf("dldemarshall - dataType= %d\n",dataType);
  switch (dataType)
    {
    case INTTYPE:
      {
	//printf("inttype of %s\n",data);
	*arg=(void*)strtoul(data,NULL,0);
	//printf("inttype is %d\n",strtoul(data,NULL,0));
	return SUCCESS;
	break;
      }
    case PUCHARTYPE:
      {
	*arg=(void*)data;
	return SUCCESS;
	break;
      }
    case UCHARTYPE:
      {
	*arg=(void*)((unsigned long)(unsigned char) (data[0]));
	return SUCCESS;
	break;
      }
    default:
      return FAILURE;
    }

}


/*also, this is the ugliest function there ever was, most likely because 
  I don't want to revert to asm for the guts*/
/*does the actual calling*/
void
dlrpc_call(struct dlargs *mydlargs)
{
  int length;
  void *args[7];
  void *vreturn;
  void *p,*handle;
  char * error;
  int i;
  struct dlargs *tmpdlargs ;
  //printf("doing a dlrpc_call!\n");

  if (mydlargs==NULL)
    {
      printf("Can't call null!\n");
      return;
    }
#ifdef RTLD_NOW
  handle=dlopen(NULL, RTLD_NOW);
#else
  handle=dlopen(NULL, RTLD_LAZY);
#endif

  if (!handle)
    {
      fputs(dlerror(), stderr);
      exit(1);
    }

  //printf("calling %s\n",mydlargs->data);
  p=dlsym(handle,mydlargs->data); /*the first one is the function name*/

  if ((error = dlerror()) != NULL)  {
    fprintf (stderr, "%s\n", error);
    exit(1);
  }

  
  
  tmpdlargs=mydlargs->next;
  length=dlargs_getlength(tmpdlargs);
  for (i=0; i<length; i++)
    {
      if (dldemarshall(&args[i],tmpdlargs->dataType,tmpdlargs->data)==FAILURE)
	{
	  printf("Could not demarshall dataType %d on arg %s\n",
		 tmpdlargs->dataType,tmpdlargs->data);
	  exit(-1);
	}
      tmpdlargs=tmpdlargs->next;
    }
  /*ok, arguments are all set up!*/

  /*hmm*/
  /*a switch is probably the most portable way to do variable arguemtents*/
  /*SO AWFUL IT HURTS THE EYES!*/
  switch (length) {
  case 0:
    vreturn=((void*(*)())(p)) ();
    break;
  case 1:
    vreturn=((void*(*)())(p)) (args[0]);
    break;
  case 2: 
    vreturn=((void*(*)())(p)) (args[0],args[1],args[2]);
    break;
  case 3:
    vreturn=((void*(*)())(p)) (args[0],args[1],args[2],args[3]);
    break;
  case 4:
    vreturn=((void*(*)())(p)) (args[0],args[1],args[2],args[3],args[4]);
    break;
  case 5:
    vreturn=((void*(*)())(p)) (args[0],args[1],args[2],args[3],args[4],args[5]);
    break;
  case 6:
    vreturn=((void*(*)())(p)) (args[0],args[1],args[2],args[3],args[4],args[5]);
    break;
  default:
    printf("We don't support that many arguments: %d. Sorry\n",length);
  }

  dlclose(handle);
}




/***********BELOW HERE DOES PARSING ************************/
/*file->dlrpc_call() calls*/

/*returns a pointer to the next character that is NOT a whitespace.*/
/*MAY return a pointer to NULL*/
char *
chop_whitespace(char * in)
{
  char * out=in;

  /*wacky error*/
  if (out==NULL)
    {
      printf("Null passed to chop_whitespace, bad coder! \n");
      return NULL;
    }

  while (*out!=0 && ( *out==' ' || *out=='\n' || *out == '\r' ||
			 *out=='\t'))
    {
      out++;
    } 

  return out;

}

/*bad dave, stack overflow*/
#define MAXSTRING 50000
/*another bug-ugly parsing routine*/
/*hahaha, this actually works!*/
int
parse_quoted_string(char * in,char **newarg,char **next)
{
  char *c,*d;

  int quotelevel;
  int backslash;
  char *tempspace;

  tempspace=malloc(MAXSTRING);
  tempspace[0]=0;
  c=tempspace;
  d=in;
  //printf("Parsing a quoted string : %s\n",in);
  quotelevel=1;
  backslash=0;
  while (quotelevel>0 && *d!=0 && *d!='\r')
    {

      if (backslash)
	{
	  switch (*d) 
	    {
	    case 't':
	      *c='\t';
	      c++; d++;
	      break;
	    case 'n':
	      *c='\n';
	      c++; d++;
	      break;
	    case 'r':
	      *c='\r';
	      c++; d++;
	      break;
	    case '\\':
	      *c='\\';
	      c++; d++;
	      break;
	    case '"':
	      *c='"';
	      c++; d++;
	      break;
	    default:
	      printf("We don't support that backslashed character yet %c.\n",*d);
	    }
	}
      else
	{
	  switch (*d)
	    {
	    case '"':
	    /*we found the second quote mark!*/
#ifdef DEBUG
	      printf("Found the second quote mark. c=%s, d=%s\n",c,d);
#endif
	    quotelevel--; /*done!*/
	    continue;
	    case '\\':
	      backslash=1;
	      d++;
	      continue;
	    default:
	      *c=*d;
	      c++;
	      d++;
	    }
	}
      
      backslash=0;
    }
  


  if (*d==0)
    {
#ifdef DEBUG
      printf("Returning end of line\n");
#endif
      return ENDOFLINE;
    }

  /*NUL TERMINATE THIS PUPPY*/
  *c=0;
  /*d now points at the second quote mark*/
  *newarg=tempspace;

  /*jump over quote*/
  d++;
  /*clear out spaces*/
  d=chop_whitespace(d);
#ifdef DEBUG
  printf("d=%s\n",d);
#endif
  /*d now points to , or ) */
  if (*d!=',' && *d!=')')
    {
      printf("Failing to parse a string!\n");
      return FAILURE;
    }

  /*skip ,*/  
  d++;
  
  *next=d;
#ifdef DEBUG
  printf("quoted string success: %s\n",tempspace);
#endif
  return SUCCESS;
}

int
parse_int(unsigned char * in,unsigned char **newarg,unsigned char **next)
{
  char *c,*d;
  char *newposition;
  int length;
#ifdef DEBUG
  printf("Inside parse_int\n");
#endif
  c=in;
  d=c;
  if (*d=='-')
    d++;
  
  /*to cover the 0x case as well*/
  if (isdigit(*d) && *(d+1)=='x')
  {
    d+=2;
    while (isdigit(*d)||isxdigit(*d))
    {
	    d++;
    }
  }
  else
  {
    /*we actually account for floats here too*/
  while (isdigit(*d) || *d=='.')
    d++;
  }

  /*d now points at the first non-digit*/
  length=d-c;
  newposition=malloc (length+1);
  memcpy(newposition,c,length);
  newposition[length]=0; /*null terminate string*/
  *newarg=newposition;
  /*skip the comma*/
  d++;
  *next=d;
  //printf("Inside parse_int\n");
  
  return SUCCESS;
}

/*a phrase is simply the function name or argument
 * phrases end on unescaped , or )*/
/*RETURNS:
 * SUCCESSSTRING 1 on success : *result is set to the new string
 * SUCESSINT 2
 * PARSEERROR on failure to really parse it out for some unknown reason
 * ENDOFFUNCTION 3 on success and end of function 
 * ENDOFLINE 4 on end of line
 * ENDOFFILE 5 on error
   insidequotes to whether we ended inside quotes or not. Should only
   matter on return value of end of line.
   next is a pointer to the character after the character that ended the
   phrase. (should be after the , or ) 
   COMMENTLINE

 * */
/*Arguments
start = start of buffer to scan
result = pointer to buffer we will return on success
insidequotes = pointer to whether we start inside quotes or not, set
on end of line without finishing or something
*/
/*Cases:
s_string_blah("abcd
abcd2",123);
*/
int
get_phrase(unsigned char * start, unsigned char ** newstring, int state, unsigned char **next)
{
  char *c;
  int result;
  unsigned char * argumentstring; /*gets malloced by other functions*/
  unsigned char * newposition;
  unsigned char * d;
  int length;
  /*TODO: 
   * Add check for // comments and if so, skip this line
   * */
  /* we start blah (HERE   "string",number); */
  c=start;
  /*we're starting a new function on this line*/
  /*first we get the function name*/
  c=chop_whitespace(c);
  if (c==NULL)
    {
      return PARSEERROR;
    }

  if (*c==0)
    {
      /*null*/
      return ENDOFLINE;
    }

  switch (state)
    {
    case INFUNCTION:

      if (*c==';' || *c==')' )
	{
	  return ENDOFFUNCTION;
	}

      /*character string here*/
      if (*c=='\'')
	{
	  c++;
	  newposition=malloc(2);
	  newposition[0]=*c;
	  newposition[1]=0;
	  //printf("SETTING newposition[0]=%c\n",*c);
	  c++; /*around character*/
	  //printf("SETTING tickmark[0]=%c\n",*c);
	  c++; /*around tickmark*/
	  //printf("SETTING comma[0]=%c\n",*c);

	  c++; /*around comma*/
	  //printf("SETTING aftercomma=%c\n",*c);

	  *next=c;
	  *newstring=newposition;
	  return SUCCESSCHAR;
	}
      
      if (*c=='"')
	{
	  //printf("PARSING A QUOTED STRING\n");
	  /*get around the quote mark*/
	  c++;
	  
	  result=parse_quoted_string(c,&argumentstring,&newposition);
	  switch (result)
	    {
	    case SUCCESS:
	      {
		*next=newposition;
		*newstring=argumentstring;
		/*we have read an entire quoted string */
		//printf("returning success string!\n");
		return SUCCESSSTRING;
		break;
	      }
	    case ENDOFLINE:
	      {
		*next=newposition;
		*newstring=argumentstring;
		printf("returning end of line!\n");
		return ENDOFLINE;
	      }
	    default:
	      printf("Failed to parse a quoted string!\n");
	      sleep(1);
	      return FAILURE;
	    }
	}
      
      if (isdigit(*c) || *c=='-')
	{
	  result=parse_int(c,&argumentstring,&newposition);
	  switch (result) {
	  case SUCCESS:
	    *next=newposition;
	    *newstring=argumentstring;
	    //printf("SUCCESSINT\n");
	    return SUCCESSINT;
	    break;
	  case NOTINT:
	    return PARSEERROR;
	  default:
	    printf("What kind of result is that? %d\n",result);
	    break;
	  }
	}
      break;
    case NEWFUNCTION:
      d=c;
      length=0;
      d=chop_whitespace(c);

      /*check for COMMENT*/
      if (*d=='/')
	return ENDOFLINE;

      while (*d!='(' && *d!=0)
	{
	  d++;
	  length++;
	}
      if (*d==0)
	{
	  return PARSEERROR;
	}
      /*d now points to the ( */
      newposition=malloc (length+1);
      memcpy(newposition,c,length);
      newposition[length]=0; /*null terminate string*/
      *newstring=newposition;
      d++;
      *next=d;
      return SUCCESS;
      break;
    case INSIDEQUOTES:
      break;
    default:
      return PARSEERROR;

    }

  return FAILURE;
}

int 
s_parse_line(char * line, char * currentvariable, int linenumber, struct dlargs * mydlargs, int state, char * c)
{
	unsigned char *newbuffer;

	int linedone=0;
	int result;
	unsigned char * nextchar;

	linedone=0;
	  while (!linedone)
	    {
#ifdef DEBUG
	      printf("Calling get_phrase on %s\n",c);
#endif
	      result=get_phrase(c,&newbuffer,state,
				&nextchar);
	      /*advance c to the next phrase start*/
	      c=nextchar;
	      if (result==ENDOFFILE)
		{
		  printf("End of file before expected on line %d\n",linenumber);
		  exit(1);
		  
		}
	      if (result==COMMENTLINE)
		{
		  linedone=1;
		  break;
		}
	      
	      if (result==ENDOFFUNCTION)
		/*we saw the ); */
		{
			linedone=1;
			state=NEWFUNCTION;
			/*here we actually do whatever it was*/
			dlrpc_call(mydlargs);
			dlargs_destroy(mydlargs);
			break;
		}

	      if (result==ENDOFLINE)
		{
		  linedone=1;
		  break;
		}
	      
	      switch (state)
		{
		case NEWFUNCTION:
		  {
		    /*add an argument, the first argument, that says
		      which function we want to call, and what the return
		      type is. In this case, we ignore the return type*/
		    dlargs_add(VOIDRETURNTYPE,mydlargs,newbuffer);
		    state=INFUNCTION;
		    break;
		  }
		case INFUNCTION:
		  {
		    
		    if (result==ENDOFLINE)
		      {
			linedone=1;
			strcat(currentvariable,newbuffer);
		      }
		    else
		      if (result==ENDOFFUNCTION)
			{
			  state=NEWFUNCTION;
			}
		      else /*SUCCESS*/
			{
			  strcat(currentvariable,newbuffer);
			  switch (result) 
			    {
			    case SUCCESSINT:
			      //printf("INTTYPE ADDED\n");
			      dlargs_add(INTTYPE,mydlargs,newbuffer);
			      currentvariable[0]=0;
			      break;
			    case SUCCESSSTRING:
			      dlargs_add(PUCHARTYPE,mydlargs,newbuffer);
			      currentvariable[0]=0;
			      break;
			    case SUCCESSCHAR:
			      //printf("Someone returned SUCCESSCHAR!\n");
			      dlargs_add(UCHARTYPE,mydlargs,newbuffer);
			      currentvariable[0]=0;
			      break;
			    case PARSEERROR:
			      printf("Someone returned PARSEERROR!\n");
			      break;
			    default:
			      printf("return value not accepted on line %s!\n",c);
			      break;
			    }


			}
		    
		  }
		  
		  break;
		case OLDFUNCTION:
		default:
		  printf("Error parsing file on line %d!\n",linenumber);
		  exit(-1);
		}
	  
	    }
}


int 
s_parse_buffer(unsigned char * buffer)
{
  	int linenumber=0;
	int state;
	int done;
	unsigned char linebuffer[MAXLINESIZE];
	unsigned char *c,*c2;
	unsigned char currentvariable[MAXVARIABLESIZE];

	currentvariable[0]=0;
	done=0;
	c=buffer;
	c2=strchr(c,'\n');
	if (c2==NULL)
	  c2=c+strlen(c);
	memcpy(buffer,c,c2-c);

	while (c!=c2)
	  {
	    struct dlargs *mydlargs=dlargs_new();

	    s_parse_line(linebuffer,currentvariable,linenumber,mydlargs,state,c);
	    //advance the pointers to the next line
	    c=c2;
	    c2=strchr(c,'\n');
	    if (c2==NULL)
	      c2=c+strlen(c);
	    //copy line
	    memcpy(linebuffer,c,c2-c);
	    linebuffer[c2-c]=0;
	    linenumber++;
	  }

}

int
s_parse(char *filename)
{
	FILE *fp;
	int linenumber=0;
	int state;
	int done;
	unsigned char buffer[MAXLINESIZE];
	unsigned char *c;
	unsigned char currentvariable[MAXVARIABLESIZE];

	currentvariable[0]=0;
	done=0;
	fp=fopen(filename,"r");
	if (fp==NULL)
	  {
	    printf("Couldn't open file %s to parse with s_parse()\n",filename);
	    perror("Error:");
	    return 0;
	  }

	state=NEWFUNCTION;
	while (!done)
	{
	  struct dlargs *mydlargs=dlargs_new();


	  memset(buffer,0x00,sizeof(buffer));
	  linenumber++;
	  /*read one line*/
	  /*fgets does store the \n
	   * */
	  c=fgets(buffer,sizeof(buffer),fp);
	  if (c==NULL)
	    {
	      done=1;
	      break;
	    }
	  
	  s_parse_line(buffer,currentvariable,linenumber,mydlargs,state,c);
	  //we don't need to free the args, because when a call happens, they get destroyed in parse_line()
	}
	if (fclose(fp)!=0)
	  printf("Somehow didn't close fp!\n");
	return 1;

}
