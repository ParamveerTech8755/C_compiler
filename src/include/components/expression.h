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
    NODE_TERNARY,
    NODE_ASGN
};

typedef struct Expression_Struct {
  enum NODE_TYPE type;
  union {
      int value;
      char* ch;
      struct {
        token* tk;
        int sizeInBytes;
        int stack_offset;
      }identifier;
      struct {
        struct Expression_Struct* exp;
        struct Expression_Struct* when_true;
        struct Expression_Struct* when_false;
      }ternary;
      struct {
          token* tk;
          union {
              struct {
                  struct Expression_Struct* left;
                  struct Expression_Struct* right;
              };
              struct Expression_Struct* child;
              struct {
                  struct Expression_Struct* var;
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

Expression* create_asign_node(token*, Expression*, Expression*);

Expression* create_ternary_node(Expression*, Expression*, Expression*);

Expression* create_number_node(int);

Expression* create_char_node(char*);

Expression* create_identifier_node(token*, int, int);

void destroy_expression(Expression **);

#endif
