#ifndef CONSTANT_H
#define CONSTANT_H

#include "../customstring.h"
#include "../token.h"

typedef struct Constant_Struct {
  enum { INT, CHAR, STRING } type;
  // for time being, can only be INT
  union {
    int number;
    char ch;
    char *text;
  } value;
} Constant;

Constant *initialize_constant(token *);

void destroy_constant(Constant **);

#endif
