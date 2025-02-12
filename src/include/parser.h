#ifndef PARSER_H
#define PARSER_H

#include "components/expression.h"
#include "components/function.h"
#include "components/program.h"
#include "components/statement.h"
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

int parse_statement(Statement *, Function *, Parser *);

int parse_return_statement(Statement *, Parser *);

int parse_declaration_statement(Statement*, Function *, Parser*);

int parse_assignment_statement(Expression*, Statement*, Parser*);

int parse_expression_statement(Statement*, Parser*);

int parse_if_statement(Statement*, Function*, Parser*);

int parse_compound_statement(Statement*, Function*, Parser*);

int parse_for_statement(Statement*, Function*, Parser*);

int parse_while_statement(Statement*, Function*, Parser*);

int parse_do_statement(Statement*, Function*, Parser*);

Expression* parse_logical_or_expression(Parser*);

Expression* parse_expression(Parser*);

Expression* parse_conditional_expression(Parser*);

Expression* parse_logical_and_expression(Parser*);

Expression* parse_bitwise_or_expression(Parser*);

Expression* parse_bitwise_xor_expression(Parser*);

Expression* parse_bitwise_and_expression(Parser*);

Expression* parse_equality_expression(Parser*);

Expression* parse_comparative_expression(Parser*);

Expression* parse_additive_expression(Parser*);

Expression* parse_term(Parser*);

Expression* parse_power(Parser*);

Expression* parse_factor(Parser*);

void destroy_parser(Parser **);

#endif
