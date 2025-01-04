#include "errors.h"
#include<stdio.h>
// #include<stdlib.h>

void printTokenError(int row, int col, ERRORS code){
	perror("line: %d\ncolumn: %d\nmessage: %s\n", row, col, code);
}

void printError(ERRORS code){
	perror("%s\n", code);
}