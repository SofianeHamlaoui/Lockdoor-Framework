/*
 * DIRB
 *
 * gendict.c - Genera un diccionario de forma incremental <jfs@t0s.org>
 * Ultima modificacion: 14/01/2004
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void gen(char *old_prefix, char current, char *pattern, char *charset, char wildcard);
void usage (char *progname);

int main(int argc, char **argv) {
  char *charset=0;
  char *charset_num="0123456789";
  char *charset_char="abcdefghijklmnopqrstuvwxyz";
  char *charset_upperchar="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *charset_hexa="0123456789abcdef";
  char *charset_alfanum="0123456789abcdefghijklmnopqrstuvwxyz";
  char *charset_sensitivealfanum="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char wildcard = 'X';


  if(argc != 3) {
    usage(argv[0]);
    }


  if(strcmp(argv[1], "-n")==0) {
    charset=charset_num;
    } else if (strcmp(argv[1], "-c")==0) {
    charset=charset_char;
    } else if (strcmp(argv[1], "-h")==0) {
    charset=charset_hexa;
    } else if (strcmp(argv[1], "-a")==0) {
    charset=charset_alfanum;
    } else if (strcmp(argv[1], "-C")==0) {
    charset=charset_upperchar;
    } else if (strcmp(argv[1], "-s")==0) {
    charset=charset_sensitivealfanum;
    } else {
    usage(argv[0]);
    }

  gen("", 0, argv[2], charset, wildcard);

  exit(0);
}



void gen(char *old_prefix, char current, char *pattern, char *charset, char wildcard) {
  char *p;
  char *prefix = (char *) malloc(strlen(old_prefix) + 2);
  sprintf(prefix, "%s%c", old_prefix, current);

  if(! *pattern) {
    printf("%s\n", prefix);
    } else {

    if(*pattern == wildcard) {
      for(p=charset; *p; p++) {
        gen(prefix, *p, pattern+1, charset, wildcard);
      }
      } else {
      gen(prefix, *pattern,  pattern+1, charset, wildcard);
      }
    }
  free(prefix);
}



void usage (char *progname) {
  printf("Usage: %s -type pattern\n", progname);
  printf("  type: -n numeric [0-9]\n");
  printf("        -c character [a-z]\n");
  printf("        -C uppercase character [A-Z]\n");
  printf("        -h hexa [0-f]\n");
  printf("        -a alfanumeric [0-9a-z]\n");
  printf("        -s case sensitive alfanumeric [0-9a-zA-Z]\n");
  printf("  pattern: Must be an ascii string in which every 'X' character wildcard\n           will be replaced with the incremental value.\n");
  printf("\n");
  printf("Example: %s -n thisword_X\n  thisword_0\n  thisword_1\n  [...]\n  thisword_9\n", progname);
  exit(-1);
}

