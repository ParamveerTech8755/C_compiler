#include<stdlib.h>
#include<stdio.h>
#include "include/lexer.h"

int main(int argc, char* argv[]){
	if(argc < 2){
		perror("Error: Incorrect usage\nCorrect usage: mycompiler <filename>");
		return EXIT_FAILURE;//returns with error code
	}
	//call the lexer with the source file
	Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
	initialize_lexer(lexer, argv[1]);
	lex_source_code(lexer);

	//print the result found by lexer
	for(int i = 0; i < lexer->index; i++)
		printf("%s\n", (*(lexer->TOKEN_LIST+i))->value);

	return 0;
}