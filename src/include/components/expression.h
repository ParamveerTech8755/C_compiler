#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../token.h"
#include <stdio.h>

enum NODE_TYPE {
    NODE_NUMBER,
    NODE_ID,//in case of variable
    NODE_BINARY_OPERATOR,
    NODE_UNARY_OPERATOR,
    NODE_CHAR,
    NODE_ASGN
};

typedef struct Expression_Struct {
  enum NODE_TYPE type;
  union {
      int value;
      char ch;
      token* identifier;
      struct {
          token* tk;
          union {
              struct {
                  struct Expression_Struct* left;
                  struct Expression_Struct* right;
              };
              struct Expression_Struct* child;
              struct {
                  token* var;
                  struct Expression_Struct* asign;
              };
          };
      }node;
  };
} Expression;

Expression *initialize_expression();

void generate_expression_asm(Expression*, FILE*);

Expression* create_bop_node(token*, Expression*, Expression*);
Expression* create_uop_node(token* , Expression*);
Expression* create_asign_node(token*, token*, Expression*);

Expression* create_number_node(int);

Expression* create_identifier_node(token*);

void destroy_expression(Expression **);

#endif
