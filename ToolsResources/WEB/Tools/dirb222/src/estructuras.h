/*
 * DIRB
 *
 * estructuas.h - Global Structures
 *
 */

#include "global.h"


/* Estructuras */

// Resultado de una peticion

struct result {
  char url[STRING_SIZE];
  CURLcode estado;
  int codigo_http;
  int head_size;
  int head_lines;
  int body_size;
  int body_words;
  int body_lines;
  char location[STRING_SIZE];
  char server[STRING_SIZE];
};


// Codigo HTTP

struct code {
  int codenum;
  char desc[STRING_SIZE];
};


// Lista de palabras

struct words {
  void *anterior;
  char word[STRING_SIZE];
  void *siguiente;
};


/* Opciones de linea de comandos */

struct opciones {
  int use_vhost;
  int use_proxy;
  int use_proxypass;
  int use_pass;
  int use_cookie;
  int verify_ssl;
  int use_agent;
  int ignore_nec;
  int default_nec;
  int debuging;
  int exitonwarn;
  int save_found;
  int mutations_file;
  int mutations_list;
  int extensions_file;
  int extensions_list;
  int saveoutput;
  int print_location;
  int finetunning;
  int interactive;
  int nothide;
  int insensitive;
  int lasting_bar;
  int dont_recurse;
  int add_header;
  int silent_mode;
  int speed;

  /* Parametros de linea de comandos */

  char mutation_file[STRING_SIZE];
  char mutation_list[STRING_SIZE];
  char exts_file[STRING_SIZE];
  char exts_list[STRING_SIZE];
  char proxy[STRING_SIZE];
  char pass_string[STRING_SIZE];
  char proxypass_string[STRING_SIZE];
  char cookie[STRING_SIZE];
  char url_inicial[STRING_SIZE];
  char mfile[STRING_SIZE];
  char agente[STRING_SIZE];
  char vhost[STRING_SIZE];
  char savefile[STRING_SIZE];
  char header_string[STRING_SIZE];

  /* Parametros para resume */

  int recursion_level;
  char current_word[STRING_SIZE];
  char current_dir[STRING_SIZE];
};







