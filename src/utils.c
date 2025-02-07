#include "include/utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/components/expression.h"
#include "include/components/statement.h"
#include "include/customstring.h"
#include "include/errors.h"
#include "include/token.h"

unsigned int getUniqueInt(){
    static unsigned int x = 0;
    x++;
    return x;
}

int isAsgnOperator(enum TOKEN_TYPE type){
    return (
        type == TOKEN_OP_ASGN || type == TOKEN_OP_ADD_ASGN || type == TOKEN_OP_SUB_ASGN || type == TOKEN_OP_DIV_ASGN ||
        type == TOKEN_OP_MUL_ASGN || type == TOKEN_OP_MOD_ASGN || type == TOKEN_OP_BIT_AND_ASGN || type == TOKEN_OP_BIT_OR_ASGN ||
        type == TOKEN_OP_BIT_XOR_ASGN
    );
}

Context* initialize_context(){
    Context* context = (Context*)malloc(sizeof(Context));
    context->loop_iden = 0;
    context->func_iden = 0;

    return context;
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
    Statement **statements = functions[i]->comp_statement->compound.statements;
    for (int j = 0; j < functions[i]->comp_statement->compound.size; j++) {
      printf("\t");
      if (statements[j]->type == RETURN){
          printf("RETURN < ");
          print_expression(statements[j]->expression);
          printf(" >");
      }
      else if(statements[j]->type == DECLARATION){
            char* name = NULL;

            if(statements[j]->expression->type == NODE_ID){
                name = statements[j]->expression->identifier.tk->value;

                //will definitely find it
                printf("DECLARE < %d > %s", statements[j]->expression->identifier.sizeInBytes, name);
            }
            else if(statements[j]->expression->type == NODE_ASGN){
                name = statements[j]->expression->node.var->identifier.tk->value;
                printf("DECLARE < %d > %s <- ", statements[j]->expression->node.var->identifier.sizeInBytes, name);
                print_expression(statements[j]->expression->node.asign);
                //why tho.. abhi tak to theek tha. finef
            }
      }
      else if(statements[j]->type == ASSIGNMENT){
          printf("%s %s ", statements[j]->expression->node.var->identifier.tk->value, statements[j]->expression->node.tk->value);
          print_expression(statements[j]->expression->node.asign);
      }
      else if(statements[j]->type == EXPRESSION){
          print_expression(statements[i]->expression);
      }
      else if(statements[j]->type == IF){
          printf("if");
      }
      else if(statements[j]->type == FOR){
          printf("for statement\n");
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
        printf("(");

        print_expression(expression->node.left);
        printf("%s", (expression->node.tk)->value);
        print_expression(expression->node.right);

        printf(")");

    }
    else if(expression->type == NODE_CHAR)
        printf("'%s'", expression->ch);
    else if(expression->type == NODE_ID)
        printf("%s", expression->identifier.tk->value);
    else if(expression->type == NODE_ASGN){
        printf("(");

        print_expression(expression->node.var);
        printf("%s", expression->node.tk->value);
        print_expression(expression->node.asign);

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
