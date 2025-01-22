#include "statement.h"
#include "expression.h"
#include <stdlib.h>
#include <stdio.h>

Statement* initialize_statement(){
	Statement* statement = (Statement*)malloc(sizeof(Statement));
	statement->expression = NULL;

	return statement;
}

void generate_return_statement_asm(char* function_name, Statement *statement, FILE *file){

    if(stringcmp(function_name, "main")){
        char* str = "\tmov $60, %%rax\n\tmov $%d, %%rdi\n\tsyscall\n";
        //then return statement must hand over the control to the OS
        if(statement->expression == NULL)
            fprintf(file, str, 0);
        else if(statement->expression->type == NODE_CHAR)
            fprintf(file, str, statement->expression->ch);
        else if(statement->expression->type == NODE_NUMBER)
            fprintf(file, str, statement->expression->value);
        else{
            generate_expression_asm(statement->expression, file);
            fprintf(file, "\tmov %%rax, %%rdi\n\tmov $60, %%rax\n\tsyscall\n");
        }
        return;
    }

    if(statement->expression == NULL){
        fprintf(file, "\tret\n");
        return;
    }

    if(statement->expression->type == NODE_CHAR)//constant type expression
        fprintf(file, "\tmovl $\'%c\',\t%%eax\n", statement->expression->ch);
    else
        generate_expression_asm(statement->expression, file);

    fprintf(file, "\tret\n");
}

void destroy_statement(Statement** statement_ptr){
	//maybe will have to use destroy_expression later
	free((*statement_ptr)->expression);
	free(*statement_ptr);
	*statement_ptr = NULL;
}
