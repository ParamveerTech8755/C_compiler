#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include "customstring.h"

typedef struct Lexer_Struct {
	int row;
	int col;
	char* src;
	int thisChar;
	int index;
	int capacity;
	string* text;
	token** TOKEN_LIST;

} Lexer;

void initialize_lexer(Lexer*, char*);

void lex_source_code(Lexer*);

void push_token(Lexer*, token*);

void destroy_lexer(Lexer**);

#endif