#include "errors.h"
#include<stdio.h>
// #include<stdlib.h>

void printTokenError(int row, int col){
	printf("Invalid token at line %d, column %d\n", row, col);
	perror("Invalid token");
}