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
          printInvalidIdentifier(text, lexer->row, lexer->col);
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
          printInvalidIdentifier(text, lexer->row, lexer->col);
          return EXIT_FAILURE;
        }
        push_token(lexer, t);
      }

      free(text->str);
      text->str = convertCharToCString(*thisChar);
      text->length = 1;
      token *t = create_token(text, lexer->row, lexer->col);
      if (t == NULL) {
          printInvalidIdentifier(text, lexer->row, lexer->col);
        return EXIT_FAILURE;
      }
      push_token(lexer, t);

      free(text->str);
      initialize_empty_string(text);
    }
    else if(isOperator(*thisChar) || *thisChar == '~'){
        if(text->length > 0){
            if(
                (*thisChar == '=' && isOperator(lastChar)) || (*thisChar == '+' && lastChar == '+') ||
                (*thisChar == '-' && lastChar == '-') || (*thisChar == '&' && lastChar == '&') ||
                (*thisChar == '|' && lastChar == '|')
            ){
                string* auxString = (string*)malloc(sizeof(string));
                initialize_with_char(auxString, *thisChar);

                lexer->text = stringconcat(text, auxString);
                token* tk = create_token(lexer->text, lexer->row, lexer->col);
                if(tk == NULL){
                    printInvalidIdentifier(lexer->text, lexer->row, lexer->col);
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
                    printInvalidIdentifier(text, lexer->row, lexer->col);
                    return EXIT_FAILURE;
                }
                push_token(lexer, tk);
                free(text->str);
                initialize_with_char(text, *thisChar);

            }
        }
        else{
            free(text->str);
            initialize_with_char(text, *thisChar);
        }

    }
    else if(*thisChar == '\''){
        //consume '
        if((*thisChar = fgetc(file)) == EOF){
            fprintf(stderr, EOF_ERROR);
            return EXIT_FAILURE;
        }
        lexer->col++;
        //push text if exists and clear
        if(text->length > 0){
            token* tk = create_token(text, lexer->row, lexer->col);
            push_token(lexer, tk);
            free(text->str);
        }

        initialize_with_char(text, *thisChar);

        if(*thisChar == '\\'){
            //escape sequence
            if((*thisChar = fgetc(file)) == EOF){
                fprintf(stderr, EOF_ERROR);
                return EXIT_FAILURE;
            }
            string* append = (string*)malloc(sizeof(string));
            initialize_with_char(append, *thisChar);
            text = lexer->text = stringconcat(text, append);
            lexer->col++;
        }

        if((*thisChar = fgetc(file)) == EOF){
            fprintf(stderr, EOF_ERROR);
            return EXIT_FAILURE;
        }
        lexer->col++;

        //consume '
        if(*thisChar != '\''){
            fprintf(stderr, "Character can only hold 1 letter.");
            return EXIT_FAILURE;
        }
        token *tk = create_char_token(text, lexer->row, lexer->col);
        push_token(lexer, tk);

        free(text->str);
        initialize_empty_string(text);
    }
    else {
        if(text->length == 1 && isOperator(lastChar)){
            token* tk = create_token(text, lexer->row, lexer->col);
            if(tk == NULL){
                printInvalidIdentifier(text, lexer->row, lexer->col);
                return EXIT_FAILURE;
            }
            push_token(lexer, tk);
            free(text->str);
            initialize_empty_string(text);
        }
        string* append = (string*)malloc(sizeof(string));
        initialize_with_char(append, *thisChar);
        lexer->text = stringconcat(text, append);
        text = lexer->text;
    }

    lastChar = *thisChar;
    lexer->col++;
  }

  // file ended
  if (text->length > 0) {
    token *t = create_token(text, lexer->row, lexer->col);
    if (t == NULL) {
        printInvalidIdentifier(text, lexer->row, lexer->col);
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
