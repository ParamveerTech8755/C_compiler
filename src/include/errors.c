#include "errors.h"
#include "customstring.h"
#include<stdio.h>
// #include<stdlib.h>

void printTokenError(char* text, char* expected, int row, int col){
	fprintf(stderr, "Unexpected token %s at line %d, column %d: expected:%s\n", text, row, col-stringlen(text), expected);
}

void printInvalidIdentifier(string* text, int row, int col){
    fprintf(stderr, "Invalid identifier %s at line %d, column %d\n", text->str, row, col-text->length);
}

void printUndefinedVariable(char *name, int row, int col){
    fprintf(stderr, "Undefined variable %s at row:%d, col:%d\n", name, row,col);
}
