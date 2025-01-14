#ifndef PARSER_H
#define PARSER_H

#include "components/program.h"
#include "token.h"

typedef struct Parser_Struct {
  int token_index;
  int token_size;
  Program *ast_root;
  token **TOKEN_LIST;
} Parser;

void initialize_parser(Parser *, token **, int);

int parse_into_ast(Parser *);

int parser_next(Parser *);

int parser_is_token_valid(Parser *, enum TOKEN_TYPE);

// all the parsers go here
int parse_function(Function *, Parser *);

int parse_statement(Statement *, Parser *);

int parse_return_statement(Statement *, Parser *);

int parse_expression(Expression*, Parser*);

void destroy_parser(Parser **);

#endif
