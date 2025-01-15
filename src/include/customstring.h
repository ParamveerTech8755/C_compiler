#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#define initialize_empty_string(str) initialize_string(str, NULL)
#define MAX_STRING_LEN 1024

typedef struct custom_string {
  char *str;
  int length;
} string;

string *stringconcat(string *, string *);
unsigned int stringlen(char *);
void initialize_string(string *, char *);
void stringcpy(char *, char *);
int stringcmp(char *, char *);
char *convertCharToCString(char);
string *toString(int);
int toInteger(char *);
int isNum(char *);
int isAlpha(char);
int isAlphaNum(char);
int isOperator(char);

void destroy_string(string **);

#endif
