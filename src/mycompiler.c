#include<stdlib.h>
#include<stdio.h>
#include "include/lexer.h"
#include "include/parser.h"

#define NO_MEM "Insufficient memory"

int main(int argc, char* argv[]){
	if(argc < 2){
		perror("Error: Incorrect usage\nCorrect usage: mycompiler <filename>");
		return EXIT_FAILURE;//returns with error code
	}
	//call the lexer with the source file
	Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
	if(lexer == NULL){
		perror(NO_MEM);
		destroy_lexer(&lexer);
		return EXIT_FAILURE;
	}

	initialize_lexer(lexer, argv[1]);
	lex_source_code(lexer);

	printf("###token list###\n");
	//print the result found by lexer
	for(int i = 0; i < lexer->index; i++)
		printf("%s\n", (*(lexer->TOKEN_LIST+i))->value);
	printf("###token list ends###\n\n");

	Parser* parser = (Parser*)malloc(sizeof(Parser));
	if(parser == NULL){
		perror(NO_MEM);
		destroy_lexer(&lexer);
		destroy_parser(&parser);
		return EXIT_FAILURE;
	}

	initialize_parser(parser);
	parse_into_ast(parser, lexer->TOKEN_LIST);


	destroy_lexer(&lexer);
	destroy_parser(&parser);

	return 0;
}