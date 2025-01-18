#include "statement.h"
#include <stdlib.h>
#include <stdio.h>

Statement* initialize_statement(){
	Statement* statement = (Statement*)malloc(sizeof(Statement));
	statement->expression = NULL;

	return statement;
}

void generate_return_statement_asm(Statement *statement, FILE *file){

    if(statement->expression == NULL){
        fprintf(file, "\tret");
        return;
    }

    if(statement->expression->type == NODE_CHAR){//constant type expression
        fprintf(file, "\tmovl $\'%c\',\t%%eax\n\tret", statement->expression->ch);
    }
    else{
        generate_expression_asm(statement->expression, file);
        //they might print something to the file.
        fprintf(file, "\tret");
    }
}

void destroy_statement(Statement** statement_ptr){
	//maybe will have to use destroy_expression later
	free((*statement_ptr)->expression);
	free(*statement_ptr);
	*statement_ptr = NULL;
}
