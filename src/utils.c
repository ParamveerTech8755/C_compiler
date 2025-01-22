#include "include/utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/components/expression.h"
#include "include/customstring.h"
#include "include/errors.h"

unsigned int getUniqueInt(){
    static unsigned int x = 0;
    x++;
    return x;
}

void generate_code(Parser *parser, char *source_file, char *output_file) {
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
      return;
  }
  //for time begin.. the only function is main
  fprintf(file, ".section .text\n");
  Program* program = parser->ast_root;
  for(int i = 0; i < program->index; i++)
      fprintf(file, "\t.globl %s\n", program->FUNCTION_LIST[i]->name);

  //main is the last function
  for(int i = 0; i < program->index; i++){
      generate_function_code(program->FUNCTION_LIST[i], file);
  }

  //add error handling
  fprintf(file, "_overflow:\n\tmov $60, %%rax\n\tmov $%d, %%rdi\n\tsyscall\n", INT_MAX);

  fclose(file);
}

void print_ast(Parser *parser) {
  Function **functions = parser->ast_root->FUNCTION_LIST;

  for (int i = 0; i < parser->ast_root->index; i++) {
    printf("FN %s -> %s:\n", functions[i]->name, functions[i]->return_type);
    // looping thru the functions
    // now loop thru the statements of the functions
    Statement **statements = functions[i]->STATEMENT_LIST;
    for (int j = 0; j < functions[i]->index; j++) {
      printf("\t");
      if (statements[i]->type == RETURN){
          printf("RETURN < ");
          print_expression(statements[i]->expression);
          printf(" >\n");
      }

      printf("\n");
    }
  }
}


void print_expression(Expression* expression){
    if(expression == NULL){
        printf("VOID");
    }
    if(expression->type == NODE_NUMBER){
        printf("%d", expression->value);
    }
    else if(expression->type == NODE_UNARY_OPERATOR){
        printf("(");
        printf("%s", (expression->node.tk)->value);
        print_expression(expression->node.child);
        printf(")");
    }
    else if(expression->type == NODE_BINARY_OPERATOR){
        // string *left, *op, *right, *begin, *end;
        // begin = (string*)malloc(sizeof(string));
        // initialize_with_char(begin, '(');
        printf("(");

        print_expression(expression->node.left);
        printf("%s", (expression->node.tk)->value);
        print_expression(expression->node.right);

        printf(")");

    }

}


int isNonCommutative(enum TOKEN_TYPE type){
    return (
        type == TOKEN_OP_MOD || type == TOKEN_OP_DIV || type == TOKEN_OP_SUB ||
        type == TOKEN_OP_GRT || type == TOKEN_OP_GRT_EQL || type == TOKEN_OP_LSR ||
        type == TOKEN_OP_LSR_EQL
    );
}
