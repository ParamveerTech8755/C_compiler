#ifndef TOKEN_H
#define TOKEN_H

//ifndef prevents inclusion from happening multiple times in the project exe file
typedef struct TOKEN_STRUCT{
	char *value;
	enum {
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
}

#endif