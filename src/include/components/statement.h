#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"

enum STATEMENT_TYPE {
	RETURN,
	DECLARATION,
	ASSIGNMENT,
	EXPRESSION,
	//thats it for now
};

typedef struct Statment_Struct {
	enum STATEMENT_TYPE type;
	//will have different grammar based on the type of the expression.
	Expression* expression;
} Statement;

Statement* initialize_statement();

void generate_return_statement_asm(Statement*, FILE*);

void generate_declaration_statement_asm(Statement*, FILE*);

void generate_assignment_statement_asm(Statement*, FILE*);

void destroy_statement(Statement**);

#endif
