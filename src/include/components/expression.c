#include "expression.h"
#include <stdlib.h>


Expression* initialize_expression(){
    Expression* expression = (Expression*)malloc(sizeof(Expression));
    expression->constant = NULL;
	return expression;
}


int evaluate_expression(Expression *expression, FILE *file){
    //fine
    if(expression == NULL)
        return 0;
    //returns nothing. fine
    return 0;
}

void destory_expression(Expression** expression_ptr){
	destroy_constant(&(*expression_ptr)->constant);
    free(*expression_ptr);
	*expression_ptr = NULL;
}
