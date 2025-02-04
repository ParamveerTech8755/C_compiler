#include "function.h"
#include "statement.h"
#include "../utils.h"

Function* initialize_function(char* return_type, char* name/*, PARAMETER** parameters*/){
	Function* function = (Function*)malloc(sizeof(Function));

	function->index = 0;
	function->capacity = 80;//initial capacity of statements.
	function->STATEMENT_LIST = (Statement**)calloc(function->capacity, sizeof(Statement*));
	function->name = name;
	function->return_type = return_type;
	function->variableCnt = 0;
	function->maxVariableSize = 1;
	function->stack_offset = 0;
	//stack is pointing to the rbp

	return function;
}

void push_statement(Function* function, Statement* statement){
	if(function->index >= function->capacity){
		function->capacity *= 2;
		//assuming the capacity doesnt overflow int data type
		Statement** temp = (Statement**)calloc(function->capacity, sizeof(Statement*));
		for(int i = 0; i < function->index; i++)
			*(temp+i) = *(function->STATEMENT_LIST+i);

		free(function->STATEMENT_LIST);
		function->STATEMENT_LIST = temp;
	}

	*(function->STATEMENT_LIST + function->index++) = statement;
}

void generate_function_code(Function *function, FILE *file){
    unsigned int x = getUniqueInt();
    char* prologue = "%s:\n\tpushq %%rbp\n\tmovq %%rsp, %%rbp\n";
    char* epilogue = "\tmovq %rbp, %rsp\n\tpopq %rbp\n\tret\n";

    fprintf(file, prologue, function->name);

    //assigning memory for local variables
    int stackSize = -1*(function->variableCnt)*(function->maxVariableSize);
    //this must be 8byte aligned for 64bit architecture
    if(stackSize%8 != 0)
        stackSize -= stackSize%8 + 8;

    if(stackSize < 0)
        fprintf(file, "\tsubq $%d, %%rsp\n", stackSize);

    int hasCompound = 0;
    for(int i = 0; i < function->index; i++){
        if(function->STATEMENT_LIST[i]->type == IF || function->STATEMENT_LIST[i]->type == FOR)
            hasCompound = 1;
        else if(function->STATEMENT_LIST[i]->type == RETURN){
            generate_statement_asm(function->STATEMENT_LIST[i], 0, file);
            break;
        }
        generate_statement_asm(function->STATEMENT_LIST[i], x, file);

    }
    if(hasCompound)
        fprintf(file, "return%u:\n", x);
    fprintf(file, "%s", epilogue);
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
