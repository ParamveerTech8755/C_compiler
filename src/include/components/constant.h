#ifndef CONSTANT_H
#define CONSTANT_H

#include "../token.h"
#include "../customstring.h"

typedef struct Constant_Struct {
	enum TOKEN_TYPE type;
	//for time being, can only be TOKEN_NUMBER_LIT
	union {
		int number;
		char ch;
		string* text;
	} value;
} Constant;

#endif
