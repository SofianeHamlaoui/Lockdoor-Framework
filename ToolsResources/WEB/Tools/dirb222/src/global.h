/*
 * DIRB
 *
 * global.h - Global Includes
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <libgen.h>
#include "../config.h"


/* Constantes */

#define MAX_FAILS   	3
#define TIMEOUT     	50
#define BIGWORDLIST   	100000
#define AGENT_STRING  	"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)"
#define MAX_ALERT   	100
#define STRING_SIZE   	256
#define DUMP_TIMEOUT  	100

#define DUMP_DIR    	".cache/dirb/resume"
#define OPTIONS_DUMP  	".cache/dirb/resume/options.dump"
#define WORDLIST_DUMP 	".cache/dirb/resume/wordlist.dump"
#define DIRLIST_DUMP  	".cache/dirb/resume/dirlist.dump"

