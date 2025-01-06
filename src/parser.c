#include "include/parser.h"
#include "include/token.h"
#include "include/errors.h"
#include "include/components/program.h"
#include <stdlib.h>

void initialize_parser(Parser* parser, token** TOKEN_LIST, int size){
	parser->token_index = 0;
	parser->ast_root = initialize_program();
	parser->token_size = size;
	parser->TOKEN_LIST = TOKEN_LIST;
}

int parser_next(Parser* parser){
	parser->token_index++;

	if(parser->token_index >= parser->token_size)
		return -1;//next not valid
	
	return parser->token_index;
}

int parser_is_token_valid(Parser* parser, enum TOKEN_TYPE type){
	// if(parser->token_index >= parser->token_size)
	// 	return -1;
	return (parser->TOKEN_LIST[parser->token_index])->type == type;
}

int parse_into_ast(Parser* parser){
	token** token_list = parser->TOKEN_LIST;


	while(parser->token_index < parser->token_size){
		char *return_type, *name;
		return_type = name = NULL;

		if(*(token_list+parser->token_index)->type == TOKEN_DATA_TYPE)//this token is TOKEN_DATA_TYPE
			return_type = *(token_list+parser->token_index)->value;
		else{
			printTokenError(token_list[parser->token_index]->row, token_list[parser->token_index]->col);
			return EXIT_FAILURE;
		}

		parser->token_index++;
		if(
			parser->token_index < parser->token_size
				&&
			(
				token_list[parser->token_index]->type == TOKEN_MAIN || 
				token_list[parser->token_index]->type == TOKEN_ID
			)
		)
			name = token_list[parser->token_index]->value;
		else{
			printTokenError(token_list[parser->token_index]->row, token_list[parser->token_index]->col);
			return EXIT_FAILURE;
		}

		
		int next_exists = parser_next(parser);
		if(next_exists == -1){
			perror(TOKEN_LIST_END);
			return EXIT_FAILURE;
		}

		Function* function = initialize_function(return_type, name);
		
		int function_status = parse_function(function, parser);
		if(function_status != 0)
			return function_status;

		push_function(parser->ast_root, function);
	}

	return 0;
}

void destroy_parser(Parser** parser_ptr){
	//no need to deallocate the TOKEN_LIST since it is deallocated by the Lexer
	destroy_program(&((*parser_ptr)->ast_root));
	free(*parser_ptr);
	*parser_ptr = NULL;
}
