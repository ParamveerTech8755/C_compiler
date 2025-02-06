#include "function.h"
#include "statement.h"
#include "../utils.h"

Function* initialize_function(char* return_type, char* name/*, PARAMETER** parameters*/){
	Function* function = (Function*)malloc(sizeof(Function));

	// function->capacity = COMPOUND_STATEMENT_CAPACITY;//initial capacity of statements.
	// function->STATEMENT_LIST = (Statement**)calloc(function->capacity, sizeof(Statement*));
	function->comp_statement = initialize_comp_statement(COMPOUND_STATEMENT_CAPACITY);

	function->name = name;
	function->return_type = return_type;
	function->variableCnt = 0;
	function->maxVariableSize = 1;
	function->stack_offset = 0;
	//stack is pointing to the rbp

	return function;
}

void push_statement(Statement* comp_statement, Statement* statement){
	if(comp_statement->compound.size >= comp_statement->compound.capacity){
		comp_statement->compound.capacity *= 2;
		//assuming the capacity doesnt overflow int data type
		Statement** temp = (Statement**)calloc(comp_statement->compound.capacity, sizeof(Statement*));
		for(int i = 0; i < comp_statement->compound.size; i++)
			*(temp+i) = *(comp_statement->compound.statements+i);

		free(comp_statement->compound.statements);
		comp_statement->compound.statements = temp;
	}

	*(comp_statement->compound.statements + comp_statement->compound.size++) = statement;
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
    Statement* comp = function->comp_statement;
    for(int i = 0; i < comp->compound.size; i++){
        if(isCompound(comp->compound.statements[i]->type))
            hasCompound = 1;
        else if(comp->compound.statements[i]->type == RETURN){
            generate_statement_asm(comp->compound.statements[i], 0, file);
            break;
        }
        generate_statement_asm(comp->compound.statements[i], x, file);

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

	destroy_statement(&function->comp_statement);
	free(function);

	*function_ptr = NULL;
}
