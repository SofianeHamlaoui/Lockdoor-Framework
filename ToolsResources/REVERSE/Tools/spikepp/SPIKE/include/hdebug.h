/*hdebug - the hydrogen debugging routine. We undefine
  the hydrogen debugging routine or define it to a null
  routine, when we go production so that all those
  strings aren't hanging around in my executable */

#define hdebug(S)   /* */

//#define hdebug(S) hdebug_printf(S) /* */


void
hdebug_printf(char * info);
