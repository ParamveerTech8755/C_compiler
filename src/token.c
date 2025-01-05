#include "include/token.h"
#include "include/customstring.h"
#include<stdio.h>
#include<stdlib.h>

#define MAXLEN 20

int isNum(char *str){
	//check if the string is a number.. if all the consituent characters are numbers
	char* ptr = str;
	int i = 0;
	while(*ptr != '\0' && i < MAXLEN){
		if(*ptr > '9' || *ptr < '0')
			return 0;
		ptr++;
		i++;
	}
	return 1;
}


token* create_token(string* value){
	token *newToken = malloc(sizeof(token));
	newToken->value = (char*)malloc((value->length + 1)*sizeof(char));
	//for each token, memory will be alloted separately, even if their value is the same
	stringcpy(newToken->value, value->str);


	if(isNum(value->str)){
		newToken->type = TOKEN_NUMBER_LIT;
		return newToken;
	}
	///if not a number literal

	if(stringcmp(value->str, "main"))
		newToken->type = TOKEN_MAIN;
	else if(stringcmp(value->str, "{"))
		newToken->type = TOKEN_RBRACE;
	else if(stringcmp(value->str, "}"))
		newToken->type = TOKEN_LBRACE;
	else if(stringcmp(value->str, "("))
		newToken->type = TOKEN_RPAREN;
	else if(stringcmp(value->str, ")"))
		newToken->type = TOKEN_LPAREN;
	else if(stringcmp(value->str, "int"))
		newToken->type = TOKEN_INT;
	else if(stringcmp(value->str, "void"))
		newToken->type = TOKEN_VOID;
	else if(stringcmp(value->str, "return"))
		newToken->type = TOKEN_RETURN;
	else if(stringcmp(value->str, ";"))
		newToken->type = TOKEN_SEMI;
	else{//not identified
		newToken->value = NULL;
		newToken->type = TOKEN_UNDEFINED;
	}
	return newToken;
}

void destroy_token(token** t){
	free((*t)->value);
	free(*t);
	*t = NULL;
}