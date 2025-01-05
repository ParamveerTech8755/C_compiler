#include "include/parser.h"
#include "include/token.h"
#include "include/components/program.h"


void initialize_parser(Parser* parser){
	parser->ast_root = initialize_program();

}

void parse_into_ast(Parser* parser, token** TOKEN_LIST){

}

void destroy_parser(Parser** parser_ptr){
	destroy_program(&((*parser_ptr)->ast_root));
	*parser_ptr = NULL;
}
