#ifndef TOKEN_H
#define TOKEN_H
#include "customstring.h"

//ifndef prevents this file to be included multiple times in the final exe file
enum TOKEN_TYPE {
	TOKEN_DATA_TYPE,
	TOKEN_MAIN,
	TOKEN_RPAREN,
	TOKEN_LPAREN,
	TOKEN_VOID,
	TOKEN_RBRACE,
	TOKEN_LBRACE,
	TOKEN_RETURN,
	TOKEN_NUMBER_LIT,
	TOKEN_CHAR_LIT,
	TOKEN_STRING_LIT,
	TOKEN_SEMI,
	TOKEN_ID,
	//operators
	TOKEN_OP_ADD, // +
	TOKEN_OP_SUB, // -
	TOKEN_OP_MUL, // *
	TOKEN_OP_DIV, // /
	TOKEN_OP_EXP, // ^
	TOKEN_OP_ASGN, // =
	TOKEN_OP_EQUALS, // ==,
	TOKEN_OP_DIV_ASGN, // /=
	TOKEN_OP_ADD_ASGN, // +=
	TOKEN_OP_SUB_ASGN, // -=
	TOKEN_OP_MUL_ASGN, // *=
	TOKEN_OP_INCRE, //++
	TOKEN_OP_DECRE, //--
	TOKEN_OP_GRT, // >
	TOKEN_OP_GRT_EQL, // >=
	TOKEN_OP_LSR, // <
	TOKEN_OP_LSR_EQL, // <=
};
//all these operators must be supported in lexing, however parsing them may not be supported for time being

typedef struct TOKEN_STRUCT{
	int row;
	int col;
	char *value;
	enum TOKEN_TYPE type;
} token;

token* create_token(string*, int, int);

int is_valid_identifier(char*);

void destroy_token(token**);

#endif
