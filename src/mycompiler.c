#include<stdlib.h>
#include<stdio.h>
#include "lexer.h"

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("Error: Incorrect usage\nCorrect usage: ./mycompiler <filename>\n");
		return 1;//returns with error code
	}
	//call the lexer with the source file
	Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
	initialize_lexer(lexer, argv[1]);

	lex_source_code(lexer);

	return 0;
}