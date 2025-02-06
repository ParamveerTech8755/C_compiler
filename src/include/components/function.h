#ifndef FUNCTION_H
#define FUNCTION_H

#include "statement.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct Function_Struct{
	char* return_type;//points to the same location as token->value
	/*return_type could be of enum token if the language does not support structs or user-defined data types*/
	char* name; // = token->value
	int variableCnt;
	int maxVariableSize;
	int stack_offset;

	// Parameter** PARAMETER_LIST; array of parameters/arguments along with their data types.. fine
	Statement* comp_statement;

} Function;

Function* initialize_function(char*, char*);

void push_statement(Statement*, Statement*);

void generate_function_code(Function*, FILE*);

void destroy_function(Function**);


#endif
