#ifndef TOKEN_H
#define TOKEN_H

//ifndef prevents this file to be included multiple times in the final exe file
enum TOKEN_TYPE {
	TOKEN_INT,
	TOKEN_MAIN,
	TOKEN_RPAREN,
	TOKEN_LPAREN,
	TOKEN_VOID,
	TOKEN_RBRACE,
	TOKEN_LBRACE,
	TOKEN_RETURN,
	TOKEN_NUMBER_LIT,
	TOKEN_SEMI
};

typedef struct TOKEN_STRUCT{
	char *value;
	TOKEN_TYPE type;
} token;

token create_token(char *value);

#endif