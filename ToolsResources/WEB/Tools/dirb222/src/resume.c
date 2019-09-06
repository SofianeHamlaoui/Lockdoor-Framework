/*
 * RESUME
 *
 * resume.c - Dump y Resume de sesiones de escaneo
 *
 */

#define _GNU_SOURCE
#include "dirb.h"


/*
 * DUMP: Vuelca a fichero el estado de una sesion de escaneo
 *
 */

void dump(void) {
  FILE *dumpfile;
  struct words *current;
  struct stat buffer;
  struct tm *ptr;
  time_t tm;
  char *home=NULL;
  char *dumppath=NULL;
  char *optionspath=NULL;
  char *wordlistpath=NULL;
  char *dirlistpath=NULL;

  tm = time(NULL);
  ptr = localtime(&tm);

  if(options.debuging>2) printf("[++] dump() Dumping Session State AT %s", asctime(ptr)); fflush(stdout);


  // Comprobamos si existe el directorio y sino intentamos crealo

  home = getenv("HOME");
  asprintf(&dumppath,"%s/%s", home, DUMP_DIR);
  asprintf(&optionspath, "%s/%s", home, OPTIONS_DUMP);
  asprintf(&wordlistpath, "%s/%s", home, WORDLIST_DUMP);
  asprintf(&dirlistpath, "%s/%s", home, DIRLIST_DUMP);

  if(stat(dumppath, &buffer)<0) {
    if(mkpath(dumppath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)<0) {
      printf("\n(!) FATAL: Error creating resume directory: %s\n", DUMP_DIR);
      exit(-1);
      }
    }


  // Volcamos la estructura options

  unlink(optionspath);
  dumpfile=abrir_file(optionspath);
  fwrite(&options, sizeof(struct opciones), 1, dumpfile);
  fclose(dumpfile);


  // Volcamos la wordlist

  unlink(wordlistpath);
  dumpfile=abrir_file(wordlistpath);
  current=wordlist_base;

  while(current->siguiente!=0) {
    if(options.debuging>6) printf("[++++++] dump() WORD: %s\n", current->word);
    fwrite(current->word, strlen(current->word), 1, dumpfile);
    fwrite("\n", 1, 1, dumpfile);
    current=current->siguiente;
    }

  fclose(dumpfile);


  // Volvamos la dirlist

  unlink(dirlistpath);
  dumpfile=abrir_file(dirlistpath);
  current=dirlist_current;

  while(current->siguiente!=0) {
    if(options.debuging>6) printf("[++++++] dump() DIR: %s\n", current->word);
    fwrite(current->word, strlen(current->word), 1, dumpfile);
    fwrite("\n", 1, 1, dumpfile);
    current=current->siguiente;
    }

  fclose(dumpfile);


}


/*
 * RESUME: Restaura el estado de una sesion de escaneo
 *
 */

void resume(void) {
  FILE *desc;
  char *home=NULL;
  char *dumppath=NULL;
  char *optionspath=NULL;
  char *wordlistpath=NULL;
  char *dirlistpath=NULL;

  resuming=1;
  home = getenv("HOME");
  asprintf(&dumppath,"%s/%s",home,DUMP_DIR);
  asprintf(&optionspath, "%s/%s",home,OPTIONS_DUMP);
  asprintf(&wordlistpath, "%s/%s",home,WORDLIST_DUMP);
  asprintf(&dirlistpath, "%s/%s",home,DIRLIST_DUMP);

  // Recuperamos la estructura options

  if((desc=fopen(optionspath, "r"))==0) {
    printf("\n(!) FATAL: Error opening input file: %s\n", optionspath);
    exit(-1);
    }
  fread(&options, sizeof(struct opciones), 1, desc);
  fclose(desc);


  // Inicializamos

  check_url(options.current_dir);
  get_options();
  init_exts();
  IMPRIME("CURRENT_BASE: %s\n", options.current_dir);
  IMPRIME("CURRENT_WORD: %s\n", options.current_word);
  IMPRIME("\n-----------------\n\n");


  // Mutations

  /*
  if(options.mutations_file) {
    muts_base=crea_wordlist_fich(options.mutation_file);
    } else if(options.mutations_list) {
    muts_base=crea_extslist(options.mutation_list);
    }
  */

  // Recuperamos la dirlist

  dirlist_current=crea_wordlist_fich(dirlistpath);
  dirlist_base=dirlist_current;
  while(dirlist_current->siguiente!=0) { dirlist_current=dirlist_current->siguiente; }
  dirlist_final=dirlist_current;
  dirlist_current=dirlist_base;


  // Recuperamos la wordlist

  wordlist_current=crea_wordlist_fich(wordlistpath);
  wordlist_base=wordlist_current;
  while(wordlist_current->siguiente!=0) { wordlist_current=wordlist_current->siguiente; }
  wordlist_final=wordlist_current;
  wordlist_current=wordlist_base;
  while(wordlist_current->siguiente!=0 && strncmp(wordlist_current->word, options.current_word, STRING_SIZE-1)!=0) { wordlist_current=wordlist_current->siguiente; }
  
  // Avanzamos la wordlist en 1
  wordlist_current=wordlist_current->siguiente;


  // Relanzamos el scan

  lanza_ataque(options.current_dir, wordlist_current);


  // Finalizamos

  cierre();
  exit(0);

}


/*
 * MKPATH: Crea el el directorio de volcado recursivamente (pcatalina)
 *
 */

int mkpath(const char *s, mode_t mode){
  char *q, *r = NULL, *path = NULL, *up = NULL;
  int rv;

  rv = -1;

  if(strcmp(s, ".") == 0 || strcmp(s, "/") == 0) return(0);

  if((path = strdup(s)) == NULL) return(-1);

  if((q = strdup(s)) == NULL) return(-1);

  if((r = dirname(q)) == NULL) goto out;

  if((up = strdup(r)) == NULL) return(-1);

  if((mkpath(up, mode) == -1) && (errno != EEXIST)) goto out;

  if((mkdir(path, mode) == -1) && (errno != EEXIST)) rv = -1;
    else rv = 0;

out:
  if(up != NULL) free(up);
  free(q);
  free(path);
  return(rv);

}

