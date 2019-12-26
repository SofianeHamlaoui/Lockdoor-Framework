typedef struct
  {
  char optionChar;
  enum 
    {
    OptionInteger,
    OptionDouble,
    OptionLong,
    OptionString,
    OptionBoolean,
    OptionEnumerated
    } optionType;
  void *optionPtr;
  int (*funcPtr)(char *param);
  char *helpString;
  char **enumValues;
  }argSpec;

extern int getargs(int argc, char *argv[], argSpec *arg, int numberSpecs);
