#ifndef PARSER_H
#define PARSER_H

#include "components/program.h"
#include "token.h"


typedef struct Parser_Struct{

	Program* ast_root;
} Parser;

void initialize_parser(Parser*);

void parse_into_ast(Parser*, token**);

void destroy_parser(Parser**);

#endif