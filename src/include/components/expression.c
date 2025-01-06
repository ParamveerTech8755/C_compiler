#include "expression.h"

Expression* initialize_expression(){
	Expression* expression = (Expression*)malloc(sizeof(Expression));

	return expression;
}

void destory_expression(Expression** expression_ptr){
	free(*expression_ptr);
	*expression_ptr = NULL;
}