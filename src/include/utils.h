#ifndef UTILS_H
#define UTILS_H

#include "parser.h"
#define COMPOUND_STATEMENT_CAPACITY 10

typedef struct Context_Struct{
    unsigned int func_iden;
    unsigned int loop_iden;
} Context;

Context* initialize_context();
void generate_code(Parser *, char*, char *);
void print_ast(Parser*);
void print_expression(Expression*);
int isNonCommutative(enum TOKEN_TYPE);
int isAsgnOperator(enum  TOKEN_TYPE);
unsigned int getUniqueInt();
#endif
