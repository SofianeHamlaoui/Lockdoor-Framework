/*
 * DIRB
 *
 * calculanec.c - Obtiene el codigo NEC
 *
 */

#include "dirb.h"
#include "http_codes.h"


/*
 * GET_NECS: Obtienes los NECs correspondientes a cada extension
 *
 */

int get_necs(char *direccion){
  char initial_url[STRING_SIZE];
  int exts_pos=0;
  int necs=0;


  if(!options.silent_mode) printf("*** Calculating NOT_FOUND code...\r");
  fflush(stdout);

  // Inicializamos

  memset(initial_url, 0, STRING_SIZE);

  exts_current=exts_base;

  strncpy(initial_url, direccion, STRING_SIZE-1);


  // Bucle

  for(exts_pos=0;exts_pos<exts_num;exts_pos++) {

    nec[exts_pos]=calcula_nec(initial_url);

    if(options.debuging>1) printf("[+] calcula_nec() NEC[%s]: %d -> %s\n", exts_current->word, nec[exts_pos]->codigo_http, code2string(code2message, nec[exts_pos]->codigo_http));

    if(options.debuging>1) printf("[+] calcula_nec() (Size: %d - Location: '%s')\n", nec[exts_pos]->body_size, nec[exts_pos]->location);

    if(options.debuging>1) printf("[+] calcula_nec() SERVER_BANNER[%s]: %s\n", exts_current->word, nec[exts_pos]->server);

    exts_current=exts_current->siguiente;

    necs++;

    }

  exts_current=exts_base;

  if(!options.silent_mode) printf("                                                                               \r");
  fflush(stdout);

  return necs;

}


/*
 * CALCULA_NEC: Obtiene el codigo de pagina no existente
 *
 */

struct result *calcula_nec(char *direccion) {
  struct result nec1, nec2;
  struct result *mynec;
  char url[STRING_SIZE];
  char rand_url1[STRING_SIZE]="randomfile1";
  char rand_url2[STRING_SIZE]="frand2";

  // Inicializamos

  mynec=malloc(sizeof(struct result));
  memset(mynec, 0, sizeof(struct result));

  memset(&nec1, 0, sizeof(struct result));
  memset(&nec2, 0, sizeof(struct result));


  // Calculo del primer NEC

  memset(url, 0, STRING_SIZE);
  strncpy(url, direccion, STRING_SIZE-1);

  strncat(rand_url1, exts_current->word, STRING_SIZE-1-strlen(rand_url1));
  strncat(url, rand_url1, STRING_SIZE-1-strlen(url));

  nec1=get_url(url);

  if(options.debuging>2) printf("[++] calcula_nec() NEC1: %d\n", nec1.codigo_http);


  switch(nec1.codigo_http) {

    case 200:
      if(options.finetunning==1) nec1.body_size=nec1.body_words;
      break;

    case 301:
    case 302:
      if(options.finetunning==1) {
      location_clean(nec1.location, rand_url1);
      }

      break;

    default:
      if(options.finetunning==1) nec1.body_size=nec1.body_words;
      break;

    }


  // Calculo del segundo NEC

  memset(url, 0, STRING_SIZE);
  strncpy(url, direccion, STRING_SIZE-1);

  strncat(rand_url2, exts_current->word, STRING_SIZE-1-strlen(rand_url2));
  strncat(url, rand_url2, STRING_SIZE-1-strlen(url));

  nec2=get_url(url);

  if(options.debuging>2) printf("[++] calcula_nec() NEC2: %d\n", nec2.codigo_http);


  // Comparamos

  if(nec1.codigo_http==nec2.codigo_http) {

    switch(nec2.codigo_http) {

      case 200:
        if(options.finetunning==1) nec2.body_size=nec2.body_words;

        if(nec1.body_size!=nec2.body_size) {
          IMPRIME("(!) WARNING: NOT_FOUND[%s] page not stable, unable to determine the correct URLs {200}.\n", exts_current->word);
          IMPRIME("    (Try using FineTunning: '-f')\n");
          if(options.exitonwarn) { next_dir=1; }
          } else {
          mynec->body_size=nec1.body_size;
          }
        break;

      case 301:
      case 302:
        if(options.finetunning==1) {
          location_clean(nec2.location, rand_url2);
          }

        if(strncmp(nec1.location, nec2.location, STRING_SIZE-1)!=0) {
          IMPRIME("(!) WARNING: NOT_FOUND[%s] not stable, unable to determine correct URLs {30X}.\n", exts_current->word);
          IMPRIME("    (Try using FineTunning: '-f')\n");
          if(options.exitonwarn) { next_dir=1; }
          } else {
          strncpy(mynec->location, nec1.location, STRING_SIZE-1);
          }
        break;

      default:
        mynec->body_size=nec1.body_size;
        break;

    }

  } else {

    IMPRIME("(!) WARNING: NOT_FOUND[%s] not stable, unable to determine the correct URLs {%d,%d}.\n", exts_current->word, nec1.codigo_http, nec2.codigo_http);
    IMPRIME("    (Server is returning random responses)\n");
    if(options.exitonwarn) { next_dir=1; }
    }

  mynec->codigo_http=nec1.codigo_http;
  strncpy(mynec->server, nec1.server, STRING_SIZE-1);

  if(options.debuging>2) printf("[++] calcula_nec() NEC: %d - %d\n", mynec->codigo_http, mynec->body_size);

  return mynec;

}


