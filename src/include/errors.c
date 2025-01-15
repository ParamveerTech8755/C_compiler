#include "errors.h"
#include<stdio.h>
// #include<stdlib.h>

void printTokenError(char* text, int row, int col){
	fprintf(stderr, "Unexpected token %s at line %d, column %d\n", text, row, col);
}
