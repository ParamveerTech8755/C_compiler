#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "include/components/constant.h"
#include "include/customstring.h"
#include "include/errors.h"

int generate_code(Parser *parser, char *source_file, char *output_file) {
  if(output_file == NULL){
    unsigned int len = stringlen(source_file);
    unsigned int end, i = len-1;

    while(source_file[i] != '.')
        i--;
    end = i;

    //everything after this is the part of the extension
    // the extension of the assembly file will be asm
    char* name = (char*)calloc(end+5, sizeof(char));
    name[end+4] = '\0';
    name[end+1] = 'a';
    name[end+2] = 's';
    name[end+3] = 'm';

    for(i = 0; i <= end; i++)
        name[i] = source_file[i];

    output_file = name;
  }

  FILE *file = fopen(output_file, "w");
  if(file == NULL){
      perror(FILE_ERR);
      return EXIT_FAILURE;
  }

  int charsWritten = fprintf(file, "\t.globl main\n");
  if(charsWritten < 0){
      perror(FILE_W_ERR);
      return EXIT_FAILURE;
  }

  Program* program = parser->ast_root;
  //main is the last function
  for(int i = 0; i < program->index; i++){
      int stat = generate_function_code(program->FUNCTION_LIST[i], file);
      if(stat < 0){
          perror(FILE_W_ERR);
          break;
      }
  }

  fclose(file);
  return 0;
}



void print_ast(Parser *parser) {
  // go thru the program.. and find all the functions
  Function **functions = parser->ast_root->FUNCTION_LIST;

  for (int i = 0; i < parser->ast_root->index; i++) {
    printf("FN %s -> %s:\n", functions[i]->name, functions[i]->return_type);
    // looping thru the functions
    // now loop thru the statements of the functions
    Statement **statements = functions[i]->STATEMENT_LIST;
    for (int j = 0; j < functions[i]->index; j++) {
      printf("\t");
      if (statements[i]->type == RETURN) {

        //if it returns constant
        switch(statements[i]->expression->constant->type){
            case INT:
                printf("RETURN <%d>\n", statements[i]->expression->constant->value.number);
                break;
            case CHAR:
                printf("RETURN <%c>\n", statements[i]->expression->constant->value.ch);
                break;
            case STRING:
                printf("RETURN <%s>\n", statements[i]->expression->constant->value.text);
                break;
            default:
                perror("Invalid constant.");
                return;
        }
      }
      printf("\n");
    }
  }
}
