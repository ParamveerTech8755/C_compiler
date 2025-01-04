#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#define initialize_empty_string(str) initialize_string(str, NULL)
#define MAX_STRING_LEN 1024

typedef struct custom_string {
	char* str;
	int length;
} string;


string* strconcat(string*, string*);
int strlen(char*);
void initialize_string(string*, char*);
void strcpy(char*, char*);
int strcmp(char*, char*);
char* convertCharToCString(char ch);

#endif