#include "function.h"
#include "../errors.h"

Function* initialize_function(char* return_type, char* name/*, PARAMETER** parameters*/){
	Function* function = (Function*)malloc(sizeof(Function));
	
	function->index = 0;
	function->capacity = 80;//initial capacity of statements.
	function->STATEMENT_LIST = (Statement**)calloc(function->capacity, sizeof(Statement*));
	function->name = name;
	function->return_type = return_type;

	return function;
}

void push_statement(Function* function, Statement* statement){
	if(function->index >= function->capacity){
		function->capacity *= 2;
		//assuming the capacity doesnt overflow int data type
		Function** temp = (Function**)calloc(function->capacity, sizeof(Function*));
		for(int i = 0; i < function->index; i++)
			*(temp+i) = *(function->FUNCTION_LIST+i);

		free(function->FUNCTION_LIST);
		function->FUNCTION_LIST = temp;
	}

	*(function->FUNCTION_LIST + function->index++) = statement;
}


int parse_function(Function* function, Parser* parser){
	//aliases
	token** token_list = parser->TOKEN_LIST;
	int index = parser->token_index;

	//check for opening parenthesis
	if(!parser_is_token_valid(parser, TOKEN_RPAREN)){
		printTokenError(token_list[index]->row, token_list[index]->col);
		return EXIT_FAILURE;
	}
	else
		index = parser_next(parser);


	/*check for parameter list*/

	//check for closing parenthesis
	if(index == -1){
		perror(TOKEN_LIST_END);
		return EXIT_FAILURE;
	}
	else if(!parser_is_token_valid(parser, TOKEN_LPAREN)){
		printTokenError(token_list[index]->row, token_list[index]->col);
		return EXIT_FAILURE;
	}
	else
		index = parser_next(parser);

	//check for opening brace
	if(index == -1){
		perror(TOKEN_LIST_END);
		return EXIT_FAILURE;
	}
	if(!parser_is_token_valid(parser, TOKEN_RBRACE)){
		printTokenError(token_list[index]->row, token_list[index]->col);
		return EXIT_FAILURE;
	}
	else
		index = parser_next(parser);

	if(index == -1){
		perror(TOKEN_LIST_END);//since we are expecting statements
		return EXIT_FAILURE;
	}

	while(!parser_is_token_valid(parser, TOKEN_LBRACE)){
		Statement* statement = initialize_statement();

		int statement_status = parse_statement(statement, parser);
		if(statement_status != 0)
			return statement_status;		

		push_statement(function, statement);

		if(parser->token_index >= parser->token_size){
			perror(TOKEN_LIST_END);
			return EXIT_FAILURE;
		}
	}

	//we have a LBrace here
	parser_next(parser);

	return 0;
}

void destroy_function(Function** function_ptr){
	//no need to free return_type and name
	//as they are already freed by destory_lexer => destroy_token

	//destroy parameters

	Function* function = *function_ptr;
	//freeing the statement list
	for(int i = 0; i < function->index; i++)
		destroy_statement(function->STATEMENT_LIST+i);

	free(function->STATEMENT_LIST);
	free(function);

	*function_ptr = NULL;
}