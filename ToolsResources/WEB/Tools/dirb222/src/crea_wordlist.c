/*
 * DIRB
 *
 * crea_wordlist.c - Crea la lista de palabras a probar
 *
 */

#include <stdio.h>
#include "dirb.h"


/*
 * CREA_WORDLIST: Crea la lista de palabras a probar a partir de un fichero
 *
 */

struct words *crea_wordlist(char *ficheros) {
  FILE *file;
  struct words *current;
  char cbuffer[STRING_SIZE];
  char current_file[STRING_SIZE];
  char *apunt;


  // Inicializamos

  current=(struct words *)malloc(sizeof(struct words));
  memset(current, 0, sizeof(struct words));
  wordlist_base=current;
  contador=0;

  if(!options.silent_mode) printf("*** Generating Wordlist...\r");
  fflush(stdout);


  // Bucle de generacion de wordlist

  while(strlen(ficheros)) {

    // Separamos la lista de ficheros

    strncpy(current_file, ficheros, STRING_SIZE-1);

    apunt=(char *)strchr(current_file, ',');

    if(apunt!=0) *apunt='\0';

    ficheros=ficheros+strlen(current_file)+1;

    // Abrimos el fichero

    if((file=fopen(current_file, "r"))==0) {
      IMPRIME("\n(!) FATAL: Error opening wordlist file: %s\n", current_file);
      exit(-1);
      }

    // Bucle por cada fichero

    while(!feof(file)) {

      // Inicializamos

      memset(cbuffer, 0, STRING_SIZE);


      // Leemos y limpiamos

      if(fgets(cbuffer, STRING_SIZE-1, file)==0) {
        if(options.debuging>4) printf("[++++] crea_wordlist() Ending the parse of file: %s\n", current_file);
        break;
        }

      //limpiamos
      limpia_url(cbuffer);

      //comentario
      if(cbuffer[0]=='#') cbuffer[0]='\0';

      if(strlen(cbuffer)) {
        strncpy(current->word, cbuffer, STRING_SIZE-1);
        contador++;
        current->siguiente=(struct words *)malloc(sizeof(struct words));
        memset(current->siguiente, 0, sizeof(struct words));
        current=current->siguiente;
        }

      }

    fclose(file);

    }


  // Operaciones finales

  wordlist_final=current;

  elimina_dupwords(wordlist_base);

  if(!options.silent_mode) printf("                                                                               \r");
  fflush(stdout);
  IMPRIME("GENERATED WORDS: %d\n", contador);

  if(contador>BIGWORDLIST) {
    IMPRIME("(!) WARNING: Wordlist is too large. This will take a long time to end.\n");
    IMPRIME("    (Use mode '-w' if you want to scan anyway)\n");
    if(options.exitonwarn) exit(-3);
    }

  return wordlist_base;

}


/*
 * crea_wordlist_fich: Crea una lista de palabras a partir de un fichero
 *
 */

struct words *crea_wordlist_fich(char *fichero) {
  FILE *file;
  char cbuffer[STRING_SIZE];
  struct words *ecurrent;
  struct words *ebase;


  // Inicializamos

  ecurrent=(struct words *)malloc(sizeof(struct words));
  memset(ecurrent, 0, sizeof(struct words));
  memset(cbuffer, 0, STRING_SIZE);
  ebase=ecurrent;


  // Abrimos el fichero y creamos su lista

  if((file=fopen(fichero, "r"))==0) {
  IMPRIME("\n(!) FATAL: Error opening words file: %s\n", fichero);
  exit(-1);
  }


  while(!feof(file)) {

  memset(cbuffer, 0, STRING_SIZE);

  // Leemos y limpiamos

  if(fgets(cbuffer, STRING_SIZE-1, file)==0) {
      if(options.debuging>4) printf("[++++] crea_wordlist_fich() Ending the parse of file: %s\n", fichero);
      break;
      }

    limpia_url(cbuffer);

    // Metemos en la lista

    strncpy(ecurrent->word, cbuffer, STRING_SIZE-1);

    if(options.debuging>5) printf("[+++++] crea_wordlist_fich() ADD_WORD: %s\n", ecurrent->word);

    ecurrent->siguiente=(struct words *)malloc(sizeof(struct words));
    ecurrent=ecurrent->siguiente;
    memset(ecurrent, 0, sizeof(struct words));

    }

  elimina_dupwords(ebase);

  ecurrent=ebase;

  while(ecurrent->siguiente!=0) {
    if(options.debuging>5) printf("[+++++] crea_wordlist_fich() WORD: %s\n", ecurrent->word);
    ecurrent=ecurrent->siguiente;
    }

  fclose(file);

  return ebase;

}



/*
 * CREA_EXTSLIST: Crea la lista de extensiones
 *
 */

struct words *crea_extslist(char *lista) {
  char cbuffer[STRING_SIZE];
  struct words *ecurrent;
  struct words *ebase;
  char *apunt;


  // Inicializamos

  ecurrent=(struct words *)malloc(sizeof(struct words));
  memset(ecurrent, 0, sizeof(struct words));
  memset(cbuffer, 0, STRING_SIZE);
  ebase=ecurrent;


  while(strlen(lista)) {

    // Separamos la lista de extensiones

    strncpy(cbuffer, lista, STRING_SIZE-1);

    apunt=(char *)strchr(cbuffer, ',');

    if(apunt!=0) *apunt='\0';

    lista=lista+strlen(cbuffer)+1;

    // Metemos en la lista

    strncpy(ecurrent->word, cbuffer, STRING_SIZE-1);

    if(options.debuging>5) printf("[+++++] crea_extslist() EXT: %s\n", ecurrent->word);

    ecurrent->siguiente=(struct words *)malloc(sizeof(struct words));
    ecurrent=ecurrent->siguiente;
    memset(ecurrent, 0, sizeof(struct words));

    }

  elimina_dupwords(ebase);

  ecurrent=ebase;

  while(ecurrent->siguiente!=0) {
    if(options.debuging>5) printf("[+++++] crea_extslist() EXT: %s\n", ecurrent->word);
    ecurrent=ecurrent->siguiente;
    }

  return ebase;

}

/*
 * COUNT_WORDS: Cuenta las palabras de una wordlist
 *
 */

int count_words(struct words *list) {
  int count=0;
  struct words *ptr;

  ptr=list;

  while(ptr->siguiente!=0) {
	count++;
	ptr=ptr->siguiente;
	}

  return count;

}

