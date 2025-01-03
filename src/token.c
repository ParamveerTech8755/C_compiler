#include "include/token.h"
#include<stdio.h>
#define MAXLEN 20

int strcmp(char* a, char* b){
	//compare if two strings are equal
	int i = 0, j = 0;
	while(*(a+i) != '\0' && *(b+j) != '\0' && i < MAXLEN && j < MAXLEN){
		if(*(a+i) == *(b+j)){
			i++;
			j++;
		}
		else
			return 0;
	}

	return (*(a+i) == '\0' && *(b+j) == '\0');
}

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


token* create_token(char* value){
	token *newToken = malloc(sizeof(token));

	if(isNum(value)){
		newToken->value = value;
		newToken->type = TOKEN_NUMBER_LIT;
		return newToken;
	}
	///if not a number literal
	newToken->value = value;

	if(strcmp(value, "main"))
		newToken->type = TOKEN_MAIN;
	else if(strcmp(value, "{"))
		newToken->type = TOKEN_RBRACE;
	else if(strcmp(value, "}"))
		newToken->type = TOKEN_LBRACE;
	else if(strcmp(value, "("))
		newToken->type = TOKEN_RPAREN;
	else if(strcmp(value, ")"))
		newToken->type = TOKEN_LPAREN;
	else if(strcmp(value, "int"))
		newToken->type = TOKEN_INT;
	else if(strcmp(value, "void"))
		newToken->type = TOKEN_VOID;
	else if(strcmp(value, "return"))
		newToken->type = TOKEN_RETURN;
	else if(strcmp(value, ";"))
		newToken->type = TOKEN_SEMI;
	else{//not identified
		printf("Unidentified token");
		exit(1);
	}
	return newToken;
}