#include<stdlib.h>
#include<stdio.h>
#include "lexer.h"

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("Error: Incorrect usage\nCorrect usage: ./mycompiler <filename>\n");
		return 1;//returns with error code
	}
	//call the lexer with the source file
	token* token_arr = lexer(argv[1]);

	return 0;
}