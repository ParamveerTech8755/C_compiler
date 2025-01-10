#ifndef PROGRAM_H
#define PROGRAM_H

#include "function.h"


typedef struct Program_Struct{
	int capacity;
	int index;
	Function** FUNCTION_LIST;
} Program;

Program* initialize_program();

void destroy_program(Program**);

void push_function(Program*, Function*);

#endif
