#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../token.h"
#include <stdio.h>

enum NODE_TYPE {
    NODE_NUMBER,
    NODE_ID,//in case of variable
    NODE_OPERATOR,
    NODE_CHAR
};


typedef struct Expression_Struct {
  enum NODE_TYPE type;
  union {
      int value;
      char ch;
      struct {
          char op;
          struct Expression_Struct* left;
          struct Expression_Struct* right;
      }node;
  };
} Expression;

Expression *initialize_expression();

void generate_expression_asm(Expression*, FILE*);

Expression* create_op_node(char, Expression*, Expression*);

Expression* create_number_node(int);

Expression* create_identifier_node(token*);

void destroy_expression(Expression **);

#endif
