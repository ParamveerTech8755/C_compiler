#ifndef FUNCTION_H
#define FUNCTION_H

#include "statement.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct Function_Struct{
	char* return_type;//points to the same location as token->value
	/*return_type could be of enum token if the language does not support structs or user-defined data types*/
	char* name; // = token->value
	int index;
	int capacity;
	// Parameter** PARAMETER_LIST; array of parameters/arguments along with their data types.. fine
	Statement** STATEMENT_LIST;
} Function;

Function* initialize_function(char*, char*);

void push_statement(Function*, Statement*);

int generate_function_code(Function*, FILE*);

void destroy_function(Function**);


#endif
