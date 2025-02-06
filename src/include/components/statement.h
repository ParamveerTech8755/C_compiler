#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"

enum STATEMENT_TYPE {
	RETURN,
	DECLARATION,
	ASSIGNMENT,
	EXPRESSION,
	IF,
	COMPOUND,
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
            //the following are compound statements
            struct Statement_Struct* comp_true;
            struct Statement_Struct* comp_false;
		} conditional;
		struct {
		    struct Statement_Struct* init;
			Expression* cond;
			Expression* exp;
		    struct Statement_Struct* comp; // this is a compound statement
		} for_loop;
		struct {
		    struct Statement_Struct **statements;
			int size;
			int capacity;
		} compound;
	};
} Statement;

Statement* initialize_statement();

Statement* initialize_comp_statement(int);

int isCompound(enum STATEMENT_TYPE);

void generate_statement_asm(Statement*, unsigned int, FILE*);

void generate_return_statement_asm(Statement*, unsigned int, FILE*);

void generate_declaration_statement_asm(Statement*, FILE*);

void generate_assignment_statement_asm(Statement*, FILE*);

void generate_if_statement_asm(Statement*, unsigned int, FILE*);

void generate_compound_statement_asm(Statement*, unsigned int, FILE*);

void generate_for_statement_asm(Statement*, unsigned int, FILE*);

void destroy_statement(Statement**);

#endif
