#include "include/parser.h"
#include "include/components/expression.h"
#include "include/components/program.h"
#include "include/components/statement.h"
#include "include/customstring.h"
#include "include/errors.h"
#include "include/token.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_parser(Parser *parser, token **TOKEN_LIST, int size) {
  parser->token_index = 0;
  parser->ast_root = initialize_program();
  parser->token_size = size;
  parser->TOKEN_LIST = TOKEN_LIST;
}

int parser_next(Parser *parser) {
  parser->token_index++;

  if (parser->token_index >= parser->token_size)
    return -1; // next not valid

  return parser->token_index;
}

int parser_is_token_valid(Parser *parser, enum TOKEN_TYPE type) {
  // if(parser->token_index >= parser->token_size)
  // 	return -1;
  return (parser->TOKEN_LIST[parser->token_index])->type == type;
}

int parse_into_ast(Parser *parser) {
  token **token_list = parser->TOKEN_LIST;

  while (parser->token_index < parser->token_size) {
    char *return_type, *name;
    return_type = name = NULL;

    if ((*(token_list + parser->token_index))->type == TOKEN_DATA_TYPE || (*(token_list + parser->token_index))->type == TOKEN_VOID)
      return_type = (*(token_list + parser->token_index))->value;
    else {
        printf("BT hai\n");
        printTokenError(
            token_list[parser->token_index]->value,
            token_list[parser->token_index]->row,
            token_list[parser->token_index]->col
        );
      return EXIT_FAILURE;
    }

    parser->token_index++;
    if (parser->token_index >= parser->token_size) {
      perror(TOKEN_LIST_END);
      return EXIT_FAILURE;
    } else if (token_list[parser->token_index]->type != TOKEN_ID) {
        printf("yaha BT hai\n");
      printTokenError(
        token_list[parser->token_index]->value,
        token_list[parser->token_index]->row,
        token_list[parser->token_index]->col
      );
      return EXIT_FAILURE;
    } else
      name = token_list[parser->token_index]->value;

    int next_exists = parser_next(parser);
    if (next_exists == -1) {
      perror(TOKEN_LIST_END);
      return EXIT_FAILURE;
    }

    Function *function = initialize_function(return_type, name);

    int function_status = parse_function(function, parser);
    if (function_status != 0)
      return function_status;
    push_function(parser->ast_root, function);
  }

  return 0;
}

void destroy_parser(Parser **parser_ptr) {
  // no need to deallocate the TOKEN_LIST since it is deallocated by the Lexer
  destroy_program(&((*parser_ptr)->ast_root));
  free(*parser_ptr);
  *parser_ptr = NULL;
}

// function parser
int parse_function(Function *function, Parser *parser) {
  // aliases
  token **token_list = parser->TOKEN_LIST;
  int index = parser->token_index;

  // check for opening parenthesis
  if (!parser_is_token_valid(parser, TOKEN_RPAREN)) {
      printf("nahi idhar hai\n");
    printTokenError(token_list[parser->token_index]->value, token_list[index]->row, token_list[index]->col);
    return EXIT_FAILURE;
  } else
    index = parser_next(parser);

  /*check for parameter list*/

  // check for closing parenthesis
  if (index == -1) {
    perror(TOKEN_LIST_END);
    return EXIT_FAILURE;
  } else if (!parser_is_token_valid(parser, TOKEN_LPAREN)) {
      printf("idhar hai\n");
    printTokenError(token_list[parser->token_index]->value, token_list[index]->row, token_list[index]->col);
    return EXIT_FAILURE;
  } else
    index = parser_next(parser);

  // check for opening brace
  if (index == -1) {
    perror(TOKEN_LIST_END);
    return EXIT_FAILURE;
  }
  if (!parser_is_token_valid(parser, TOKEN_RBRACE)) {
      printf("here 1\n");
    printTokenError(token_list[parser->token_index]->value, token_list[index]->row, token_list[index]->col);
    return EXIT_FAILURE;
  } else
    index = parser_next(parser);

  if (index == -1) {
    perror(TOKEN_LIST_END); // since we are expecting statements
    return EXIT_FAILURE;
  }

  while (!parser_is_token_valid(parser, TOKEN_LBRACE)) {
    Statement *statement = initialize_statement();

    int statement_status = parse_statement(statement, parser);
    if (statement_status != 0)
      return statement_status;

    push_statement(function, statement);

    if (parser->token_index >= parser->token_size) {
      perror(TOKEN_LIST_END);
      return EXIT_FAILURE;
    }
  }

  // we have a LBrace here
  parser_next(parser);

  return 0;
}

int parse_statement(Statement *statement, Parser *parser) {
  int index = parser->token_index;

  if (index >= parser->token_size) {
    perror(TOKEN_LIST_END);
    return EXIT_FAILURE;
  } else if (parser_is_token_valid(parser, TOKEN_RETURN)) {
    statement->type = RETURN;
    index = parser_next(parser);
    if (index == -1) {
      perror(TOKEN_LIST_END);
      return EXIT_FAILURE;
    }
    // parse the remaining return statement
    int parse_status = parse_return_statement(statement, parser);
    if (parse_status != 0)
      return parse_status;
  }
  else if(parser_is_token_valid(parser, TOKEN_DATA_TYPE)){
    statement->type = DECLARATION;//for local variable not for functions(for now)
    int parse_status = parse_declaration_statement(statement, parser);

    if(parse_status != 0)
        return parse_status;
  }
  else if(parser_is_token_valid(parser, TOKEN_ID)){
      token* cur = parser->TOKEN_LIST[index];
      //the variable

      //if not
      index = parser_next(parser);
      if(index == -1){
          perror(TOKEN_LIST_END);
          return EXIT_FAILURE;
      }
      //if it is assignment related?
      enum TOKEN_TYPE token_type = parser->TOKEN_LIST[index]->type;
      if(isAsgnOperator(parser->TOKEN_LIST[index]->type)){
        /* identifier exists in the current scope */
        statement->type = ASSIGNMENT;
        int status = parse_assignment_statement(cur, statement, parser);
        if(status !=  0)
            return status;
      }
      else{
        statement->type = EXPRESSION;
        parser->token_index--;
        int status = parse_expression_statement(statement, parser);
        if(status !=  0)
            return status;
      }
  }
  else{
    int status = parse_expression_statement(statement, parser);
    if(status !=  0)
        return status;
  }

  return 0; // if everything goes well
}

int parse_declaration_statement(Statement* statement, Parser* parser){
    int index = parser->token_index;
    token* data_type = parser->TOKEN_LIST[index];
    index = parser_next(parser);

    if(index == -1){
        perror(TOKEN_LIST_END);
        return EXIT_FAILURE;
    }

    if(!parser_is_token_valid(parser, TOKEN_ID)){
        token* cur = parser->TOKEN_LIST[index];
        printTokenError(cur->value, cur->row, cur->col);
        return EXIT_FAILURE;
    }

    //save it somewhere
    index = parser_next(parser);
    if(index == -1){
        perror(TOKEN_LIST_END);
        return EXIT_FAILURE;
    }

    if(parser_is_token_valid(parser, TOKEN_SEMI))
        statement->expression = NULL;
    else if(parser_is_token_valid(parser, TOKEN_OP_ASGN)){
        if(parser_next(parser) == -1){
            perror(TOKEN_LIST_END);
            return EXIT_FAILURE;
        }

        Expression* exp = parse_expression(parser);
        if(exp == NULL)
            return EXIT_FAILURE;

        if(parser->TOKEN_LIST[parser->token_index]->type != TOKEN_SEMI){
            token* cur = parser->TOKEN_LIST[index];
            printTokenError(cur->value, cur->row, cur->col);
            return EXIT_FAILURE;
        }
        statement->expression = exp;
    }
    else{
        token* cur = parser->TOKEN_LIST[index];
        printTokenError(cur->value, cur->row, cur->col);
        return EXIT_FAILURE;
    }

    parser_next(parser);
    return 0;
}

int parse_assigment_statement(token* LHS_token, Statement* statement, Parser* parser){
    int index = parser->token_index;
    token* asgn_operator = parser->TOKEN_LIST[index];

    index = parser_next(parser);
    if(index == -1){
        perror(TOKEN_LIST_END);
        return EXIT_FAILURE;
    }

    Expression* exp = parse_expression(parser);
    if(exp == NULL)
        return EXIT_FAILURE;

    if(!parser_is_token_valid(parser, TOKEN_SEMI))
        return EXIT_FAILURE;


    statement->expression = create_asign_node(asgn_operator, LHS_token, exp);
    return 0;
}

int parse_expression_statement(Statement* statement, Parser* parser){
    Expression* exp = parse_expression(parser);
    if(exp == NULL)
        return EXIT_FAILURE;

    if(parser->TOKEN_LIST[parser->token_index]->type != TOKEN_SEMI){
        token* cur = parser->TOKEN_LIST[parser->token_index];
        printTokenError(cur->value, cur->row, cur->col);
        return EXIT_FAILURE;
    }
    statement->expression = exp;

    parser_next(parser);
    return 0;
}

int parse_return_statement(Statement *statement, Parser *parser) {
  // this will definitely be a valid token
  if(!parser_is_token_valid(parser, TOKEN_SEMI)){
      Expression* expression = parse_expression(parser);
      if(expression == NULL){
          return EXIT_FAILURE;
      }
      statement->expression = expression;
  }
  //if TOKEN_LIST ends with the expression, it returns NULL

  if(!parser_is_token_valid(parser, TOKEN_SEMI)){
      token* tk = parser->TOKEN_LIST[parser->token_index];
      printf("here 2\n");
      printTokenError(tk->value, tk->row, tk->col);
      return EXIT_FAILURE;
  }

  // go to the next token for the next statement
  parser_next(parser);
  return 0;
}

Expression* parse_expression(Parser* parser){
    Expression* acc = parse_logical_or_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    if(isAsgnOperator(cur->type)){
        if(acc->type != NODE_ID){
            fprintf(stderr, "Can only assign values to variables.");
            return NULL;
        }

        if(parser_next(parser) == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }

        Expression* exp = parse_expression(parser);
        acc = create_asign_node(cur, acc->node.var, exp);
    }

    return acc;
}

Expression* parse_logical_or_expression(Parser* parser){
    Expression* acc = parse_logical_and_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_OR){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_logical_and_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_logical_and_expression(Parser* parser){
    Expression* acc = parse_bitwise_or_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_AND){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_bitwise_or_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_bitwise_or_expression(Parser* parser){
    Expression* acc = parse_bitwise_xor_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_BIT_OR){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_bitwise_xor_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_bitwise_xor_expression(Parser* parser){
    Expression* acc = parse_bitwise_and_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_BIT_XOR){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_bitwise_and_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_bitwise_and_expression(Parser* parser){
    Expression* acc = parse_equality_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_AMP){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_equality_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_equality_expression(Parser* parser){
    Expression* acc = parse_comparative_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_EQUALS || cur->type == TOKEN_OP_NOT_EQL){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_comparative_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}
Expression* parse_comparative_expression(Parser* parser){
    Expression* acc = parse_additive_expression(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_GRT || cur->type == TOKEN_OP_GRT_EQL || cur->type == TOKEN_OP_LSR || cur->type == TOKEN_OP_LSR_EQL){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_additive_expression(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }

    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_additive_expression(Parser *parser){
    Expression* acc = parse_term(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_ADD || cur->type == TOKEN_OP_SUB){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_term(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }
    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}


Expression* parse_term(Parser* parser){
    Expression* acc = parse_factor(parser);
    if(acc == NULL)
        return NULL;

    token* cur = parser->TOKEN_LIST[parser->token_index];
    while(cur->type == TOKEN_OP_MUL || cur->type == TOKEN_OP_DIV || cur->type == TOKEN_OP_MOD){
        //mod has the same precedence as * and /
        int index = parser_next(parser);

        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        Expression* right = parse_factor(parser);
        if(right == NULL)
            return NULL;
        acc = create_bop_node(cur, acc, right);
        cur = parser->TOKEN_LIST[parser->token_index];
    }
    if(parser->token_index >= parser->token_size){
        perror(TOKEN_LIST_END);
        return NULL;
    }
    return acc;
}

Expression* parse_factor(Parser* parser){
    token* cur = parser->TOKEN_LIST[parser->token_index];
    Expression* result = NULL;
    if(cur->type == TOKEN_RPAREN){
        if(parser_next(parser) == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        result = parse_expression(parser);
        if(result == NULL)
            return NULL;//there was some error

        cur = parser->TOKEN_LIST[parser->token_index];
        if(cur->type != TOKEN_LPAREN){
            printTokenError(cur->value, cur->row, cur->col);
            return NULL;
        }
        if(parser_next(parser) == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
    }
    else if(cur->type == TOKEN_OP_SUB){//unary - => can be treated as binary operation, 0 - num
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }

        result = parse_factor(parser);
        if(result == NULL)
            return NULL;
        // printf("this should be ; =>%s\n", parser->TOKEN_LIST[parser->token_index]->value);
        result = create_uop_node(cur, result);
    }
    else if(cur->type == TOKEN_OP_ADD){//uanry + => this can be ignored
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }

        result = parse_factor(parser);
    }
    else if(cur->type == TOKEN_OP_NOT){// ! has the same precedence as unary - and +
        //now parse the factor right after it
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        result = parse_factor(parser);
        if(result == NULL)
            return result;

        result = create_uop_node(cur, result);
    }
    else if(cur->type == TOKEN_OP_BIT_NOT){
        int index = parser_next(parser);
        if(index == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
        result = parse_factor(parser);
        if(result == NULL)
            return NULL;

        result = create_uop_node(cur, result);
    }
    else if(cur->type == TOKEN_NUMBER_LIT){
        result = create_number_node(toInteger(cur->value));
        if(parser_next(parser) == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
    }
    else if(cur->type == TOKEN_ID){
        //fine
        printf("token_id: %s\n", cur->value);
        result = create_identifier_node(cur);
        if(parser_next(parser) == -1){
            perror(TOKEN_LIST_END);
            return NULL;
        }
    }
    else
        printTokenError(cur->value, cur->row, cur->col);

    return result;
}
