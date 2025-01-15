#include "include/parser.h"
#include "include/components/constant.h"
#include "include/components/program.h"
#include "include/components/statement.h"
#include "include/errors.h"
#include "include/token.h"
#include <complex.h>
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

    if ((*(token_list + parser->token_index))->type ==
        TOKEN_DATA_TYPE) // this token is TOKEN_DATA_TYPE
      return_type = (*(token_list + parser->token_index))->value;
    else {
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
    } else if ((token_list[parser->token_index]->type != TOKEN_MAIN &&
                token_list[parser->token_index]->type != TOKEN_ID)) {
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
  return 0; // if everything goes well
}

int parse_return_statement(Statement *statement, Parser *parser) {
  // this will definitely be a valid token

  if(!parser_is_token_valid(parser, TOKEN_SEMI)){
      Expression *expression = initialize_expression();
      int exp_status = parse_expression(expression, parser);
      if(exp_status != 0)
        return exp_status;

      statement->expression = expression;
  }

  //expecting a semi colon... fine
  if(parser->token_index >= parser->token_size || !parser_is_token_valid(parser, TOKEN_SEMI)){
      perror(TOKEN_LIST_END);
      return EXIT_FAILURE;
  }

  // go to the next token for the next statement
  parser_next(parser);
  return 0;
}


int parse_expression(Expression *expression, Parser *parser){
    //fr time being // only constants.. fine
    Constant* constant = initialize_constant(parser->TOKEN_LIST[parser->token_index]);
    expression->constant = constant;
    parser_next(parser);
    return 0;
}
