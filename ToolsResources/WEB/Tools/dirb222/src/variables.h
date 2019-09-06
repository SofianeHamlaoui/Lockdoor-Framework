/*
 * DIRB
 *
 * variables.h - Global Variables
 *
 */


#include "global.h"


/* Global variables */

FILE *outfile;                    // Fichero de output
CURL *curl;                       // Descriptor CURL
struct opciones options;          // Array con las opciones recibidas del usuario

struct result *nec[100];          // Array que almacena los NECs
int contador;                     // Contador de palabras generadas
int descargadas;                  // Numero de URLs descargadas
int encontradas;                  // Numero de URLs encontradas
int errores;                      // Contador de errores
int existant;                     // Flag para determinar si la URL actual existe
int listable;                     // Flag para determinar si un directorio es listable

struct words *wordlist_base;      // Base de la lista de palabras
struct words *wordlist_current;   // Nodo actual de la lista de palabras
struct words *wordlist_final;     // Final de la lista de palabras

struct words *exts_base;          // Base de la lista de extensiones
struct words *exts_current;       // Nodo actual de la lista de externsiones
int exts_num;                     // Numero de extensiones

struct words *muts_base;          // Base de la lista de mutaciones

struct words *dirlist_base;       // Base de la lista de los directorios encontrados
struct words *dirlist_final;      // Final de la lista los directorios encontrados
struct words *dirlist_current;    // Nodo actual de la lista los directorios encontrados

int resuming;                     // Flag para saber si estamos en una sesion resumida
int next_dir;					  // Flag para saber si tenemos que pasar al siguiente directorio








