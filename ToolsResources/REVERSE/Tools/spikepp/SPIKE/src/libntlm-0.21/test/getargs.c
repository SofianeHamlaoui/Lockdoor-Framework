#include "getargs.h"
#include <stdio.h>
#include <string.h>

double dummy(double x)
{
return x * 56.7;
}

static int findString(char *name, char **namelist)
  {
  int i;

  for (i=0; *namelist; ++namelist, ++i)
    if (!strcmp(name, *namelist))
      return i;
  return -1;
  }

static char *progName;

typedef int (*fptrNoParam)(void);
typedef int (*fptrParam)(char*);

static void errorMsg(char c, const char *expected, const char *got)
  {
  fprintf(stderr,"%s: option -%c expected %s parameter, got '%s'\n",progName,c,expected,got);
  }

static void prOptions(argSpec arg[], int specCount)
  {
  int i;
  fprintf(stderr,"%s: options: \n",progName);

  for (i=0; i<specCount; ++i)
    {
    fprintf(stderr,"        -%c ",arg[i].optionChar);
    switch (arg[i].optionType)
      {
      case OptionInteger:    fprintf(stderr,"<integer>  "); break;
      case OptionLong:       fprintf(stderr,"<long>     "); break;
      case OptionDouble:     fprintf(stderr,"<float>    "); break;
      case OptionString:     fprintf(stderr,"<string>   "); break;
      case OptionBoolean:    fprintf(stderr,"           "); break;
      case OptionEnumerated: fprintf(stderr,"<string>   "); break;
      }
    fprintf(stderr,arg[i].helpString);
    if (arg[i].optionType == OptionEnumerated)
      {
      char **s;
      int d = *(unsigned*)arg[i].optionPtr;
      int n;
      fprintf(stderr,"\n");
      fprintf(stderr,"                      where <string> is one of:\n");
      for (n=0,s=arg[i].enumValues; *s; ++s,++n)
	fprintf(stderr,"                          %s%s\n",*s,n==d ? " (default)" : "");
      }
    else
      {
      switch (arg[i].optionType)
	{
	case OptionInteger:    fprintf(stderr," (default %d)\n",*(int*)arg[i].optionPtr); break;
	case OptionLong:       fprintf(stderr," (default %ld)\n",*(long*)arg[i].optionPtr); break;
	case OptionDouble:     fprintf(stderr," (default %f)\n",*(double*)arg[i].optionPtr); break;
	case OptionString:     fprintf(stderr," (default '%s')\n",*(char**)arg[i].optionPtr); break;
	case OptionBoolean:    fprintf(stderr,"\n"); break;
	case OptionEnumerated: break;
	}
      }
    }
  }

union
  {
  int i;
  long l;
  double d;
  char *p;
  }trash;

int getargs(int argc, char *argv[], argSpec arg[], int specCount)
  {
  int argsUsed;
  char *p;
  int a;
  int argDone;

  progName = argv[0];
  argsUsed = 1;
  ++argv;
  
  while (argsUsed < argc)
    {
    /* at this point, argv[0] is the next item to be processed */
    
    p = argv[0];

    if (*p != '-')
      return argsUsed;

    ++argsUsed;
    ++argv;

    ++p;

    if (*p =='?')
      {
      prOptions(arg,specCount);
      return -1;
      }

    argDone = 0;

    for (a=0; a<specCount;)
      {
      char *optionArgPtr = "";
      void *optionPtr = &trash;

      if (*p == arg[a].optionChar)
	{
	if (arg[a].optionType != OptionBoolean)
	  {
	  if (p[1] != '\0')
	    optionArgPtr = p+1;
	  else
	    {
	    optionArgPtr = argv[0];
	    ++argsUsed;
	    ++argv;
	    }

	  if (argsUsed > argc)
	    {
	    errorMsg(*p,"","");
	    return -1;
	    }
	  }

	if (arg[a].optionPtr)
	  optionPtr = arg[a].optionPtr;

	switch (arg[a].optionType)
	  {
	  case OptionInteger:
	    {
	    char *format = "%d";
	    if (optionArgPtr[0]=='x')
	      {
	      format = "%x";
	      ++optionArgPtr;
	      }
	    else if (optionArgPtr[0]=='0' && optionArgPtr[1]=='x')
	      {
	      format = "%x";
	      optionArgPtr+=2;
	      }

	    if (sscanf(optionArgPtr,format,optionPtr) != 1)
	      {
	      errorMsg(*p,"integer",optionArgPtr);
	      return -1;
	      }
	    break;
	    }

	  case OptionLong:
	    if (sscanf(optionArgPtr,"%ld",(long*)optionPtr) != 1)
	      {
	      errorMsg(*p,"long",optionArgPtr);
	      return -1;
	      }
	    break;

	  case OptionString:
	    *((char**)optionPtr) = strdup(optionArgPtr);
	    break;

	  case OptionDouble:
	    if (sscanf(optionArgPtr,"%lf",(double*)optionPtr) != 1)
	      {
	      errorMsg(*p,"floating point",optionArgPtr);
	      return -1;
	      }
	    break;

	  case OptionBoolean:
	    *((int*)optionPtr) = 1;
	    break;

	  case OptionEnumerated:
	    if ((*((int*)optionPtr) = findString(optionArgPtr,arg[a].enumValues)) < 0)
		{
		char **n = arg[a].enumValues;
		fprintf(stderr,"%s: option -%c expects parameter to be one of:\n",progName,*p);
		while (*n)
		  {
		  fprintf(stderr,"                    %s\n",*n);
		  ++n;
		  }
		return -1;
		}
	    break;

	  default:
	    break;
	  }

	if (arg[a].funcPtr)
	  {
	  int s = (*arg[a].funcPtr)(optionArgPtr);
	  if (s<0)
	    return s;
	  }

	if (arg[a].optionType == OptionBoolean)
	  {
	  ++p;
	  if (*p != '\0')
	    {
	    a = 0;
	    continue;
	    }
	  }

	argDone = 1;
	break;
	}
      ++a;
      }

    if (!argDone)
      {
      fprintf(stderr,"%s: unrecognized option '%c'\n",progName,*p);
      prOptions(arg,specCount);
      return -1;
      }
    }
  return argsUsed;
  }

