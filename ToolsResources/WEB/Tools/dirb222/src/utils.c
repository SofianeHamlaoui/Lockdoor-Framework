/*
 * DIRB
 *
 * utils.c - Utilidades varias
 *
 */

#include "dirb.h"


/*
 * LIMPIA_URL: Corta la cadena si contiene los caracteres (\r), (\n) o ( )
 *
 */

void limpia_url(char *limpia) {
  char *ptr;

  ptr=(char *)strchr(limpia, '\r');
  if(ptr!=0) *ptr='\0';
  ptr=(char *)strchr(limpia, '\n');
  if(ptr!=0) *ptr='\0';
  ptr=(char *)strchr(limpia, ' ');
  if(ptr!=0) *ptr='\0';

}


/*
 * BARRA: Añade una barra (/) al final de una cadena si no la tiene
 *
 */

void barra(char *barr) {

  if(barr[strlen(barr)-1]=='/') {
    if(options.debuging>5) printf("[+++++] barra() LAST = 0x2F\n");
    } else {
    strncat(barr, "\x2f", 1);
    }

}


/*
 * GUARDADIR: Guarda un directorio en la lista de directorios
 *
 */

void guardadir(char *direccion) {

  if(!options.silent_mode) printf("                                                                               \r");
  IMPRIME("==> DIRECTORY: %s\n", direccion);
  options.recursion_level++;
  if(options.debuging>4) printf("[++++] guardadir() RECURSION_LEVEL: %d\n", options.recursion_level);
  strncpy(dirlist_final->word, direccion, STRING_SIZE-1);
  dirlist_final->siguiente=(struct words *)malloc(sizeof(struct words));
  memset(dirlist_final->siguiente, 0, sizeof(struct words));
  dirlist_final=dirlist_final->siguiente;
  existant=0;

}


/*
 * ELIMINA_DUPWORDS: Elimina elementos duplicados en una lista de palabras
 *
 */

void elimina_dupwords(struct words *puntero) {
  struct words *epun;
  struct words *next;
  struct words *prev;

  epun=puntero;

  prev=epun;

  next=epun->siguiente;

  while(epun->siguiente!=0) {

    if(
    (strncmp(epun->word, next->word, STRING_SIZE-1)==0 && !options.insensitive && next->siguiente!=0)
    ||
    (strncasecmp(epun->word, next->word, STRING_SIZE-1)==0 && options.insensitive && next->siguiente!=0)
    ) {

      if(options.debuging>4) printf("[++++] elimina_dupwords() DUP_WORD: %s - %s\n", epun->word, next->word);
    contador--;
      prev->siguiente=next->siguiente;
      free(next);
      next=prev->siguiente;

      // recursion_level--; (!) En caso de necesitar eliminar directorios duplicados

      } else {
      prev=next;
      next=next->siguiente;
      }

    if(next==0) {
      if(options.debuging>5) printf("[+++++] elimina_dupwords() FIN_loop\n");
      epun=epun->siguiente;
      prev=epun;
      next=epun->siguiente;
      }

    }

}



/*
 * ABRIR_FILE: Abre un fichero de output
 *
 */

FILE *abrir_file(char *file) {
  FILE *desc;

  /*
  if(access (file, F_OK)==0) {
  printf("\n(!) FATAL: File exists, can't overwrite\n");
  exit(-1);
    }
  */

  if((desc=fopen(file, "a"))==0) {
    printf("\n(!) FATAL: Error opening output file: %s\n", file);
    exit(-1);
    }

  return desc;

}


/*
 * LOCATION_CMP: Compara 2 cabeceras Location
 *
 */

int location_cmp(char *A, char *B) {
  int result=0;
  char *ptr=0;

  if(strncmp(A, "http://", 7)==0 || strncmp(A, "https://", 8)==0) {
    ptr=(char *)strchr(A, '/');
    if(ptr!=0) A=ptr+1;
    ptr=(char *)strchr(A, '/');
    if(ptr!=0) A=ptr+1;
    ptr=(char *)strchr(A, '/');
    if(ptr!=0) A=ptr+1;
    }

  if(options.debuging>4) printf("[++++] location_cmp() A[%d]: '%s'\n", strlen(A), uri_decode(A));

  if(strncmp(B, "http://", 7)==0 || strncmp(B, "https://", 8)==0) {
    ptr=(char *)strchr(B, '/');
    if(ptr!=0) B=ptr+1;
    ptr=(char *)strchr(B, '/');
    if(ptr!=0) B=ptr+1;
    ptr=(char *)strchr(B, '/');
    if(ptr!=0) B=ptr+1;
    }

  if(options.debuging>4) printf("[++++] location_cmp() B[%d]: '%s'\n", strlen(B), uri_decode(B));

  result=strncmp(uri_decode(A), uri_decode(B), strlen(A)>strlen(B) ? strlen(A) : strlen(B));

  if(options.debuging>4) printf("[++++] location_cmp() RESULT: %d (%d)\n", result, strlen(A)>strlen(B) ? strlen(A) : strlen(B));

  return result;

}


/*
 * LOCATION_CLEAN: Limpia la cacebera location
 *
 */

void location_clean(char *cleaned, char *toelim) {
  char *ptr=0, *ptr2=0;
  char *A=cleaned;
  int remain;
  char tmpstr[STRING_SIZE];

  if(options.debuging>3) printf("[+++] location_clean() TOCLEAN: %s | TOELIM: %s\n", cleaned, toelim);

  // Jump to uri-path
  if(strncmp(A, "http://", 7)==0 || strncmp(A, "https://", 8)==0) {
    ptr=(char *)strchr(A, '/');
    if(ptr!=0) A=ptr+1;
    ptr=(char *)strchr(A, '/');
    if(ptr!=0) A=ptr+1;
    ptr=(char *)strchr(A, '/');
    if(ptr!=0) A=ptr+1;
    }

  // Remove session
  ptr=(char *)strchr(A, ';');
  if(ptr!=0) {
	ptr2=(char *)strchr(ptr, '?');
	if(ptr2==0) ptr2=cleaned+strlen(cleaned);
    remain=cleaned+strlen(cleaned)-ptr2;
	strncpy(tmpstr, ptr2, remain);
	strncpy(ptr, tmpstr, remain);
    memset(ptr+remain, 0, 1);
    }

  // Remove string
  ptr=(char *)strstr(A, toelim);
  if(ptr!=0) {
    ptr2=ptr+strlen(toelim);
	remain=cleaned+strlen(cleaned)-ptr2;
    strncpy(tmpstr, ptr2, remain);
    strncpy(ptr, tmpstr, remain);
    memset(ptr+remain, 0, 1);
    }

  if(options.debuging>3) printf("[+++] location_clean() CLEANED: %s\n", cleaned);

}


/*
 * CHECK_URL: Comprueba que la URL inicial tiene el formato correcto
 *
 */

void check_url(char *url) {

  if(options.debuging>4) printf("[++++] check_url() URL: %s\n", url);

  if(strncmp(url, "http://", 7)!=0 && strncmp(url, "https://", 8)!=0) {
    printf("\n(!) FATAL: Invalid URL format: %s\n", url);
    printf("    (Use: \"http://host/\" or \"https://host/\" for SSL)\n");
    exit(-1);
    }

}


/*
 * ISLISTABLE: Comprueba si un directorio es listable o no
 *
 */

int islistable(char *direccion) {

  listable=-1;
  get_url(direccion);

  if(listable==-1) listable=0;

  return listable;

}


/*
 * KBHIT: Comprueba si alguna tecla ha sido pulsada (no bloqueante)
 *
 */

char kbhit(void){
  struct timeval tv;
  fd_set read_fd;
  char key = 0;
  static struct termios term;

  tcgetattr(0, &term);
  term.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &term);

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&read_fd);
  FD_SET(0, &read_fd);

  if(select(1, &read_fd, NULL, NULL, &tv) == -1) key = -1;

  if(FD_ISSET(0, &read_fd)) {
    read(0, &key, 1);
    } else {
  key = 0;
    }

  if(options.debuging>4) printf("[++++] kbhit() %d\n", key);

  return(key);
}


/*
 * CIERRE: Codigo de finalizacion
 *
 */

void cierre(void) {
  struct tm *ptr;
  time_t tm;

  tm = time(NULL);
  ptr = localtime(&tm);

  if(!options.silent_mode) printf("                                                                               \r");
  IMPRIME("\n-----------------\n");

  IMPRIME("END_TIME: %s", asctime(ptr));
  IMPRIME("DOWNLOADED: %d - FOUND: %d\n", descargadas, encontradas);

  if(options.saveoutput) fclose(outfile);

  //curl_easy_cleanup(curl); // Segfaults on last cygwin :?

}


/*
 * CODE2STRING: Convierte un codigo en su cadena equivalente
 *
 */

char *code2string(struct code *a, u_int v) {
  int i=0;

  while(a[i].codenum!=v && a[i].codenum!=0xff) {
  i++;
  }

  return a[i].desc;

}


/*
 * INIT_EXTS: Inicializa el array de extensiones
 *
 */

void init_exts(void) {

  // Si no hay extensiones, creamos una vacia

  if(exts_num==0) {

    exts_current=(struct words *)malloc(sizeof(struct words));
    memset(exts_current, 0, sizeof(struct words));
    exts_base=exts_current;

    strncpy(exts_current->word, "", 2);

    exts_current->siguiente=(struct words *)malloc(sizeof(struct words));
    memset(exts_current->siguiente, 0, sizeof(struct words));
    exts_current=exts_current->siguiente;

    exts_num=1;

  }

}


/*
 * URI_DECODE: Decodifica una cadena con caracteres uri-encoded
 *
 */

char *uri_decode(char *uri) {
  int i=0;
  int ptr=0;
  char *decoded;
  char hexa[3];
  char code;

  decoded=(char *)malloc(strlen(uri)+1);

  for(i=0;i<strlen(uri);i++) {

    switch(*(uri+i)) {
      case '%':
        memset(hexa, 0, 3);
        sprintf(hexa, "%c%c", *(uri+i+1), *(uri+i+2));
        i+=2;
        //printf("HEXA: %s\n", hexa);
        sscanf(hexa, "%x", (int *)&code);
        //printf("CODE: %c\n", code);
        *(decoded+ptr)=code;
        ptr++;
        break;
      default:
        *(decoded+ptr)=*(uri+i);
        ptr++;
        break;
      }

    }

  *(decoded+ptr)='\0';

  return decoded;

}


/*
 * COMPARE_STR: Devuelve la posicion del primer caracater diferente
 *
 */

int compare_str(char *A, char *B) {
  int i=0;

  for(i=0; i<strlen(A); i++) {
	if(A[i]!=B[i]) return i;
    }

  return 0;

}