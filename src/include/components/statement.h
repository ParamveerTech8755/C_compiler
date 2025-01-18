#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"

enum STATEMENT_TYPE {
	RETURN,
	//thats it for now
};

typedef struct Statment_Struct {
	enum STATEMENT_TYPE type;
	//will have different grammar based on the type of the expression.
	Expression* expression;
} Statement;

Statement* initialize_statement();

void generate_return_statement_asm(Statement*, FILE*);

void destroy_statement(Statement**);


// void set_statement_type(Statement*, enum STATEMENT_TYPE);

// void set_statement_exp(Statement*, Expression);

#endif
