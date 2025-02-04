#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"

enum STATEMENT_TYPE {
	RETURN,
	DECLARATION,
	ASSIGNMENT,
	EXPRESSION,
	IF,
	FOR
	//thats it for now
};

typedef struct Statement_Struct {
	enum STATEMENT_TYPE type;
	//will have different grammar based on the type of the expression.
	union{
	    Expression* expression;
		struct {
    		Expression* expression;
    		struct Statement_Struct *when_true;
    		struct Statement_Struct *when_false;
		} conditional;
	};
} Statement;

Statement* initialize_statement();

void generate_statement_asm(Statement*, unsigned int, FILE*);

void generate_return_statement_asm(Statement*, unsigned int, FILE*);

void generate_declaration_statement_asm(Statement*, FILE*);

void generate_assignment_statement_asm(Statement*, FILE*);

void generate_if_statement_asm(Statement*, unsigned int, FILE*);

void destroy_statement(Statement**);

#endif
