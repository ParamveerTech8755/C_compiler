#include "include/errors.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Error: Incorrect usage\nCorrect usage: mycompiler <filename> [<output_filename>]");
    return EXIT_FAILURE; // returns with error code
  }
  // call the lexer with the source file
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  if (lexer == NULL) {
    perror(NO_MEM);
    return EXIT_FAILURE;
  }

  initialize_lexer(lexer, argv[1]);
  int lexer_status = lex_source_code(lexer);

  if (lexer_status != 0) {
    destroy_lexer(&lexer);
    return lexer_status;
  }

  printf("###token list###\n");
  // print the result found by lexer
  for (int i = 0; i < lexer->index; i++)
    printf("%s\n", (*(lexer->TOKEN_LIST + i))->value);
  printf("###token list ends###\n\n");

  Parser *parser = (Parser *)malloc(sizeof(Parser));
  if (parser == NULL) {
    perror(NO_MEM);
    destroy_lexer(&lexer);
    return EXIT_FAILURE;
  }

  initialize_parser(parser, lexer->TOKEN_LIST, lexer->index);

  int exit_code = parse_into_ast(parser);

  if(exit_code != 0)
      fprintf(stderr, "Parsing failed\n");
  else {
    print_ast(parser);

    (argc < 2) ? generate_code(parser, argv[1], NULL) : generate_code(parser, argv[1], argv[2]);

  }

  destroy_lexer(&lexer);
  destroy_parser(&parser);


  return exit_code;
}
