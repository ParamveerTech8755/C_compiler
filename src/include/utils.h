#ifndef UTILS_H
#define UTILS_H

#include "parser.h"

void generate_code(Parser *, char*, char *);
void print_ast(Parser*);
void print_expression(Expression*);
int isNonCommutative(enum TOKEN_TYPE);
unsigned int getUniqueInt();
#endif
