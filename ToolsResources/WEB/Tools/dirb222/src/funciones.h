/*
 * DIRB
 *
 * funciones.h - Global Functions
 *
 */

#include "global.h"


/* Funciones */
// dirb.c
void banner(void);
void ayuda(void);
// get_url.c
struct result get_url(char *resp_url);
size_t get_header(void *ptr, size_t size, size_t nmemb, void *stream);
size_t get_body(void *ptr, size_t size, size_t nmemb, void *stream);
// lanza_ataque.c
void lanza_ataque(char *url_base, struct words *wordlist);
// options.c
void get_options(void);
// calculanec.c
int get_necs(char *direccion);
struct result *calcula_nec(char *direccion);
// crea_wordlist.c
struct words *crea_wordlist(char *ficheros);
struct words *crea_wordlist_fich(char *fichero);
struct words *crea_extslist(char *lista);
int count_words(struct words *list);
// utils.c
void limpia_url(char *limpia);
void barra(char *barr);
void guardadir(char *direccion);
void elimina_dupwords(struct words *puntero);
FILE *abrir_file(char *file);
int location_cmp(char *A, char *B);
void location_clean(char *cleaned, char *toelim);
void check_url(char *url);
int islistable(char *direccion);
char kbhit(void);
void cierre(void);
char *code2string(struct code *a, u_int v);
void init_exts(void);
char *uri_decode(char *uri);
int compare_str(char *A, char *B);
// resume.c
int mkpath(const char *s, mode_t mode);
void dump(void);
void resume(void);


