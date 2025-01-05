#include "statement.h"

Statement* initialize_statement(){
	Statement* statement = (Statement*)malloc(sizeof(Statement));

	return statement;
}

// void set_statement_type(Statement* statement, enum STATEMENT_TYPE type){
// 	statement->type = type;
// }


void destroy_statement(Statement** statement_ptr){
	free(*statement_ptr);
	*statement_ptr = NULL;
}