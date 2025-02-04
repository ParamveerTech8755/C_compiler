#ifndef ERRORS_H
#define ERRORS_H

#define EOF_ERROR "Unexpected end of file"
#define TOKEN_LIST_END "Unexpected end of token list"
#define NO_MEM "Insufficient Memory"
#define FILE_ERR "Error opening file"
#define FILE_W_ERR "Error writing to file"


void printTokenError(char*, int, int);
void printUndefinedVariable(char*, int, int);
#endif
