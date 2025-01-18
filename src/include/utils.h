#ifndef UTILS_H
#define UTILS_H

#include "parser.h"
#define INT_MAX 2147483647
#define INT_MIN -2147483648

void generate_code(Parser *, char*, char *);
void print_ast(Parser*);
void print_expression(Expression*);

#endif
