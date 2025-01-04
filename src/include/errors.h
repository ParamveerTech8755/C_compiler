#ifndef ERRORS_H
#define ERRORS_H

enum ERRORS {
	INVALID_TOKEN = "Invalid token",
	HEAP_ALLOCATION_FAILED = "Insufficient memory";
} errors;

void printTokenError(int, int, ERRORS);
void printError(ERRORS);

#endif