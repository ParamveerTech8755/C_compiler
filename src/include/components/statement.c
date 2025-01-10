#include "statement.h"
#include <stdlib.h>
#include <stdio.h>

Statement* initialize_statement(){
	Statement* statement = (Statement*)malloc(sizeof(Statement));

	statement->expression = (Expression*)malloc(sizeof(Expression));

	return statement;
}

void destroy_statement(Statement** statement_ptr){
	//maybe will have to use destroy_expression later
	free((*statement_ptr)->expression);
	free(*statement_ptr);
	*statement_ptr = NULL;
}
