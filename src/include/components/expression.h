#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "constant.h"
/*
Expression can be of multiple types:
1. Constant
2. unary operator, constant
3. unary operator, expression
4. constant, binary operator, constant
5. expression, binary op, constant
6. expression, binary op, expression
7. constant, binary op, expression

for time being, support only for constant expressions

*/

typedef struct Expression_Struct {
	Constant constant;
} Expression;

Expression* initialize_expression();

void destroy_expression(Expression**);

#endif