#include "expression.h"
#include <stdlib.h>


Expression* initialize_expression(token* startToken){
	Expression* expression = (Expression*)malloc(sizeof(Expression));
  expression->startToken = startToken;
  expression->size = 0;
	return expression;
}

int evaluateExpression(Expression* expression){
  //

  // expression->value = value;

  return 0;
  //status
}

void destory_expression(Expression** expression_ptr){
	free(*expression_ptr);
	*expression_ptr = NULL;
}
