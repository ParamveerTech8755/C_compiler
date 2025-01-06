#include "statement.h"
#include "../errors.h"

Statement* initialize_statement(){
	Statement* statement = (Statement*)malloc(sizeof(Statement));

	statement->expression = (Expression*)malloc(sizeof(Expression));

	return statement;
}

// void set_statement_type(Statement* statement, enum STATEMENT_TYPE type){
// 	statement->type = type;
// }

int parse_statement(Statement* statement, Parser* parser){
	int index = parser->token_index;
	
	if(index >= parser->token_size){
		perror(TOKEN_LIST_END);
		return EXIT_FAILURE;
	}
	else if(parser_is_valid_token(parser, TOKEN_RETURN)){
		statement->type = RETURN;
		index = parser_next();
		// 
	}
	return 0;//if everything goes well
	
}


void destroy_statement(Statement** statement_ptr){
	//maybe will have to use destroy_expression later
	free((*statement_ptr)->expression);
	free(*statement_ptr);
	*statement_ptr = NULL;
}