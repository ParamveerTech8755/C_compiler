#include "errors.h"
#include<stdio.h>
// #include<stdlib.h>

void printTokenError(char* text, int row, int col){
	fprintf(stderr, "Unexpected token %s at line %d, column %d\n", text, row, col);
}

void printUndefinedVariable(char *name, int row, int col){
    fprintf(stderr, "Undefined variable %s at row:%d, col:%d", name, row,col);
}
