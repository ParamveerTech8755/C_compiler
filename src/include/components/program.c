#include "program.h"
#include <stdlib.h>
#include <stdio.h>
#include "../errors.h"

Program* initialize_program(){
	Program* program = (Program*)malloc(sizeof(Program));
	program->index = 0;
	program->capacity = 5;
	//initial size of FUNCTION_LIST
	program->FUNCTION_LIST = (Function**)calloc(program->capacity, sizeof(Function*));
	if(program->FUNCTION_LIST == NULL){
		perror(NO_MEM);
		free(program);
		program = NULL;
	}

	return program;
}

void push_function(Program* program, Function* function){
	if(program->index >= program->capacity){
		program->capacity *= 2;
		//assuming the capacity doesnt overflow int data type
		Function** temp = (Function**)calloc(program->capacity, sizeof(Function*));
		for(int i = 0; i < program->index; i++)
			*(temp+i) = *(program->FUNCTION_LIST+i);

		free(program->FUNCTION_LIST);
		program->FUNCTION_LIST = temp;
	}

	*(program->FUNCTION_LIST + program->index++) = function;
}

void destroy_program(Program** program_ptr){
	Program* program = *program_ptr;

	for(int i = 0; i < program->index; i++)
		destroy_function(program->FUNCTION_LIST+i);

	free(program->FUNCTION_LIST);
	free(program);

	*program_ptr = NULL;
}
