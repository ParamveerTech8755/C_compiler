#include "include/token.h"
#include "include/customstring.h"
#include<stdio.h>
#include<stdlib.h>


token* create_char_token(string* value, int row, int col){
    token* newToken = (token*)malloc(sizeof(token));

    newToken->row = row;
    newToken->col = col;
    newToken->type = TOKEN_CHAR_LIT;
    newToken->value = (char*)malloc((value->length+1)*sizeof(char));

    stringcpy(newToken->value, value->str);

    return newToken;
}

token* create_token(string* value, int row, int col){
	token *newToken = (token*)malloc(sizeof(token));

	//the position of the token in the source file
	newToken->row = row;
	newToken->col = col;

	newToken->value = (char*)malloc((value->length + 1)*sizeof(char));
	//for each token, memory will be alloted separately, even if their value is the same
	stringcpy(newToken->value, value->str);


	if(isNum(value->str)){
		newToken->type = TOKEN_NUMBER_LIT;
		return newToken;
	}
	///if not a number literal

	if(stringcmp(value->str, "{"))
		newToken->type = TOKEN_RBRACE;
	else if(stringcmp(value->str, "}"))
		newToken->type = TOKEN_LBRACE;
	else if(stringcmp(value->str, "("))
		newToken->type = TOKEN_RPAREN;
	else if(stringcmp(value->str, ")"))
		newToken->type = TOKEN_LPAREN;
	else if(stringcmp(value->str, "int") || stringcmp(value->str, "char"))/*all the data-types*/
		newToken->type = TOKEN_DATA_TYPE;
	else if(stringcmp(value->str, "void"))
		newToken->type = TOKEN_VOID;
	else if(stringcmp(value->str, "return"))
		newToken->type = TOKEN_RETURN;
	else if(stringcmp(value->str, ";"))
		newToken->type = TOKEN_SEMI;
	else if(stringcmp(value->str, "+"))
	   newToken->type = TOKEN_OP_ADD;
	else if(stringcmp(value->str, "-"))
	   newToken->type = TOKEN_OP_SUB;
	else if(stringcmp(value->str, "*"))
	   newToken->type = TOKEN_OP_MUL;
	else if(stringcmp(value->str, "/"))
	   newToken->type = TOKEN_OP_DIV;
	else if(stringcmp(value->str, "="))
	    newToken->type = TOKEN_OP_ASGN;
	else if(stringcmp(value->str, "+="))
	   newToken->type = TOKEN_OP_ADD_ASGN;
	else if(stringcmp(value->str, "-="))
	   newToken->type = TOKEN_OP_SUB_ASGN;
	else if(stringcmp(value->str, "*="))
	   newToken->type = TOKEN_OP_MUL_ASGN;
	else if(stringcmp(value->str, "/="))
	   newToken->type = TOKEN_OP_DIV_ASGN;
	else if(stringcmp(value->str, "=="))
	    newToken->type = TOKEN_OP_EQUALS;
	else if(stringcmp(value->str, "^"))
	   newToken->type = TOKEN_OP_BIT_XOR;
	else if(stringcmp(value->str, "<"))
	    newToken->type = TOKEN_OP_LSR;
	else if(stringcmp(value->str, "<="))
	   newToken->type = TOKEN_OP_LSR_EQL;
	else if(stringcmp(value->str, ">"))
	    newToken->type = TOKEN_OP_GRT;
	else if(stringcmp(value->str, ">="))
	    newToken->type = TOKEN_OP_GRT_EQL;
	else if(stringcmp(value->str, "++"))
	   newToken->type = TOKEN_OP_INCRE;
	else if(stringcmp(value->str, "--"))
	    newToken->type = TOKEN_OP_DECRE;
	else if(stringcmp(value->str, "&&"))
	   newToken->type = TOKEN_OP_AND;
	else if(stringcmp(value->str, "||"))
	   newToken->type = TOKEN_OP_OR;
	else if(stringcmp(value->str, "!="))
	    newToken->type = TOKEN_OP_NOT_EQL;
	else if(stringcmp(value->str, "%"))
	    newToken->type = TOKEN_OP_MOD;
	else if(stringcmp(value->str, "!"))
	    newToken->type = TOKEN_OP_NOT;
	else if(stringcmp(value->str, "~"))
	    newToken->type = TOKEN_OP_BIT_NOT;
	else if(stringcmp(value->str, "&"))
	    newToken->type = TOKEN_OP_AMP; //can work as bitwise_and(binary), address_of operator(unary)
	else if(stringcmp(value->str, "|"))
        newToken->type = TOKEN_OP_BIT_OR;
    else if(stringcmp(value->str, "^="))
        newToken->type = TOKEN_OP_BIT_XOR_ASGN;
	else if(stringcmp(value->str, "%="))
	    newToken->type = TOKEN_OP_MOD_ASGN;
	else if(stringcmp(value->str, "|="))
	    newToken->type = TOKEN_OP_BIT_OR_ASGN;
	else if(stringcmp(value->str, "&="))
	    newToken->type = TOKEN_OP_BIT_AND_ASGN;
    else if(stringcmp(value->str, "?"))
        newToken->type = TOKEN_QUE_MRK;
    else if(stringcmp(value->str, ":"))
        newToken->type = TOKEN_COLON;
    else if(stringcmp(value->str, "\""))
        newToken->type = TOKEN_DB_QUOTE;
    else if(stringcmp(value->str, "'"))
        newToken->type = TOKEN_SG_QUOTE;
    else if(stringcmp(value->str, "if"))
        newToken->type = TOKEN_IF;
    else if(stringcmp(value->str, "else"))
        newToken->type = TOKEN_ELSE;
	else if(is_valid_identifier(value->str))
		newToken->type = TOKEN_ID;
	else{
	    free(newToken->value);
		free(newToken);
		newToken = NULL;
	}

	return newToken;
}

int is_valid_identifier(char *value){
    char *first = value;
    if(*first != '_' && !isAlpha(*first))
        return 0;
    ++first;
    while(*first != '\0'){
        if(*first != '_' && !isAlphaNum(*first))
            return 0;
        first++;
    }
    return 1;
}

void destroy_token(token** t){
	free((*t)->value);
	free(*t);
	*t = NULL;
}
