#include "function.h"

Function* initialize_function(char* name, char* return_type/*, PARAMETER** parameters*/){
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