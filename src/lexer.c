#include "include/lexer.h"
#include "include/customstring.h"
#include "include/errors.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_lexer(Lexer *lexer, char *filename) {
  lexer->row = 0;
  lexer->col = 0;
  lexer->src = filename;
  lexer->index = 0;
  lexer->capacity = 100;
  lexer->TOKEN_LIST = (token **)calloc(lexer->capacity, sizeof(token *));
  if (lexer->TOKEN_LIST == NULL) {
    perror(NO_MEM);
    exit(EXIT_FAILURE);
  }
  lexer->text = (string *)malloc(sizeof(string));
  initialize_empty_string(lexer->text);
}

void push_token(Lexer *lexer, token *tok) {
  if (lexer->index >= lexer->capacity) {
    lexer->capacity *= 2;
    // assuming that it will fit in the integer limit.
    token **newList = (token **)calloc(lexer->capacity, sizeof(token *));
    for (int i = 0; i < lexer->index; i++)
      *(newList + i) = *(lexer->TOKEN_LIST + i);

    free(lexer->TOKEN_LIST);
    lexer->TOKEN_LIST = newList;
  }
  *(lexer->TOKEN_LIST + lexer->index++) = tok;
}

int lex_source_code(Lexer *lexer) {

  FILE *file = fopen(lexer->src, "r");
  if (file == NULL) {
    perror(FILE_ERR);
    exit(EXIT_FAILURE);
  }
  int *thisChar = &(lexer->thisChar);
  string *text = lexer->text;
  while ((*thisChar = fgetc(file)) != EOF) { // Read character by character
    lexer->col++;

    if (*thisChar == 10 || *thisChar == 13 || *thisChar == ' ' ||
        *thisChar == '\t') {
      if (*thisChar == 10 || *thisChar == 13) {
        lexer->row++;
        lexer->col = 0;
      }
      if (text->length > 0) {
        token *t = create_token(text, lexer->row, lexer->col);
        if (!t->value) {
          printTokenError(lexer->row, lexer->col);
          return EXIT_FAILURE;
        }
        /// push the token in the tokens array
        push_token(lexer, t);
        free(text->str);
        initialize_empty_string(text);
      }
    } else if (*thisChar == '(' || *thisChar == ')' || *thisChar == '{' ||
               *thisChar == '}' || *thisChar == ';') {
      // two tokens will be separated by either { or , or ; or ( or ) or } or
      // whitespace
      if (text->length > 0) {
        token *t = create_token(text, lexer->row, lexer->col);
        if (!t->value) {
          printTokenError(lexer->row, lexer->col);
          return EXIT_FAILURE;
        }
        push_token(lexer, t);
      }
      char *str = convertCharToCString(*thisChar);

      free(text->str);
      initialize_string(text, str);
      token *another_t = create_token(text, lexer->row, lexer->col);
      if (!another_t->value) {
        printTokenError(lexer->row, lexer->col);
        return EXIT_FAILURE;
      }
      push_token(lexer, another_t);

      free(text->str);
      initialize_empty_string(text);
    } else {

      char *character = convertCharToCString(*thisChar);
      string *append = (string *)malloc(sizeof(string));
      append->length = 1;
      append->str = character;

      lexer->text = stringconcat(text, append);
      free(append);
      free(character);
      free(text);
      text = lexer->text;
    }
  }

  // file ended
  if (text->length > 0) {
    token *t = create_token(text, lexer->row, lexer->col);
    if (!t->value) {
      printTokenError(lexer->row, lexer->col);
      return EXIT_FAILURE;
    }
    push_token(lexer, t);
  }

  fclose(file);

  return 0;
  // successful
}

void destroy_lexer(Lexer **lexer_ptr) {
  Lexer *lexer = *lexer_ptr;
  // free(lexer->src);
  // this is trying to free the pointer in the readonly area of the memory
  destroy_string(&lexer->text);

  for (int i = 0; i < lexer->index; i++)
    destroy_token(lexer->TOKEN_LIST + i);

  free(lexer->TOKEN_LIST);
  free(lexer);

  *lexer_ptr = NULL;
}
