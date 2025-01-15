#include "include/lexer.h"
#include "include/customstring.h"
#include "include/errors.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_lexer(Lexer *lexer, char *filename) {
  lexer->row = 1;
  lexer->col = 1;
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
    return EXIT_FAILURE;
  }
  int *thisChar = &(lexer->thisChar);
  int lastChar = 0;

  string *text = lexer->text;
  while ((*thisChar = fgetc(file)) != EOF) { // Read character by character

    if (*thisChar == 10 || *thisChar == 13 || *thisChar == ' ' || *thisChar == '\t') {

      if (text->length > 0) {
        token *t = create_token(text, lexer->row, lexer->col);
        if (t == NULL) {
          printTokenError(text->str, lexer->row, lexer->col);
          return EXIT_FAILURE;
        }
        /// push the token in the tokens array
        push_token(lexer, t);
        free(text->str);
        initialize_empty_string(text);
      }

      if (*thisChar == 10 || *thisChar == 13) {
        lexer->row++;
        lexer->col = 0;
      }
    } else if (*thisChar == '(' || *thisChar == ')' || *thisChar == '{' || *thisChar == '}' || *thisChar == ';') {
      // two tokens will be separated by either { or , or ; or ( or ) or } or whitespace
      if (text->length > 0) {
        token *t = create_token(text, lexer->row, lexer->col);
        if (t == NULL) {
          printTokenError(text->str, lexer->row, lexer->col);
          return EXIT_FAILURE;
        }
        push_token(lexer, t);
      }
      char *str = convertCharToCString(*thisChar);

      free(text->str);
      initialize_string(text, str);
      token *t = create_token(text, lexer->row, lexer->col);
      if (t == NULL) {
        printTokenError(text->str, lexer->row, lexer->col);
        return EXIT_FAILURE;
      }
      push_token(lexer, t);

      free(text->str);
      initialize_empty_string(text);
    }
    else if(isOperator(*thisChar)){
        if(text->length > 0){
            if((*thisChar == '=' && isOperator(lastChar)) || (*thisChar == '+' && lastChar == '+') || (*thisChar == '-' && lastChar == '-')){
                char* character = convertCharToCString(*thisChar);
                string* auxString = (string*)malloc(sizeof(string));
                auxString->length = 1;
                auxString->str = character;
                lexer->text = stringconcat(text, auxString);
                free(auxString);
                free(character);
                free(text->str);
                free(text);
                token* tk = create_token(lexer->text, lexer->row, lexer->col);
                if(tk == NULL){
                    printTokenError(lexer->text->str, lexer->row, lexer->col);
                    return EXIT_FAILURE;
                }
                push_token(lexer, tk);
                free(lexer->text->str);
                initialize_empty_string(lexer->text);
                text = lexer->text;

            }
            else{
                token* tk = create_token(text, lexer->row, lexer->col);
                if(tk == NULL){
                    printTokenError(text->str, lexer->row, lexer->col);
                    return EXIT_FAILURE;
                }
                push_token(lexer, tk);
                free(text->str);
                text->str = convertCharToCString(*thisChar);
                text->length = 1;
            }
            // token *t = create_token(text, lexer->row, lexer->col);
            // if(t == NULL){
            //     printTokenError(lexer->row, lexer->col);
            //     return EXIT_FAILURE;
            // }
            // push_token(lexer, t);
            // free(text->str);
        }
        else{
            free(text->str);
            text->str = convertCharToCString(*thisChar);
            text->length = 1;
        }
        // char* ch = convertCharToCString(*thisChar);
        // text->str = ch;
        // text->length = 1;


    }
    else {
        if(text->length == 1 && isOperator(lastChar)){
            token* tk = create_token(text, lexer->row, lexer->col);
            if(tk == NULL){
                printTokenError(text->str, lexer->row, lexer->col);
                return EXIT_FAILURE;
            }
            push_token(lexer, tk);
            free(text->str);
            initialize_empty_string(text);
        }
        char *character = convertCharToCString(*thisChar);
        string *append = (string *)malloc(sizeof(string));
        append->length = 1;
        append->str = character;

        lexer->text = stringconcat(text, append);
        free(append);
        free(character);
        free(text->str);
        free(text);
        text = lexer->text;
    }

    lastChar = *thisChar;
    lexer->col++;
  }

  // file ended
  if (text->length > 0) {
    token *t = create_token(text, lexer->row, lexer->col);
    if (t == NULL) {
      printTokenError(text->str, lexer->row, lexer->col);
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
