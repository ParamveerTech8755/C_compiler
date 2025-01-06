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
	TOKEN_SEMI,
	TOKEN_ID,
	TOKEN_UNDEFINED
};

typedef struct TOKEN_STRUCT{
	int row;
	int col;
	char *value;
	enum TOKEN_TYPE type;
} token;

token* create_token(string*, int, int);

void destroy_token(token**);

#endif