#include "constant.h"
#include <stdlib.h>

Constant *initialize_constant(token *tk) {
  Constant *constant = (Constant *)malloc(sizeof(Constant));

  if (tk->type == TOKEN_NUMBER_LIT) {
    constant->type = INT;
    constant->value.number = toInteger(tk->value);
  }
  else if(tk->type == TOKEN_CHAR_LIT) {
      constant->type = CHAR;
      constant->value.ch = *(tk->value);
  }
  else if(tk->type == TOKEN_STRING_LIT) {
      constant->type = STRING;
      constant->value.text = tk->value;
  }
  //character and string tokens
  return constant;
}


void destroy_constant(Constant ** constant_ptr){
    //constant->value.text points to the string of the token.... //no need to deallocate
    free(*constant_ptr);
    *constant_ptr = NULL;
}
