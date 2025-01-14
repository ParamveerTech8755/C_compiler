#include "customstring.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

string *stringconcat(string *a, string *b) {
  int total_length = a->length + b->length;
  string *newString = (string *)malloc(sizeof(string));
  newString->length = total_length;
  char *ptr = newString->str = (char *)calloc(total_length + 1, sizeof(char));

  for (int i = 0; i < a->length; i++)
    *(ptr++) = *(a->str + i);
  for (int i = a->length; i < total_length; i++)
    *(ptr++) = *(b->str + i - a->length);

  // terminate the new string
  *ptr = '\0';
  return newString;
}

int stringlen(char *text) {
  // should be '\0' terminated
  if (text == NULL)
    return 0;

  int i = 0;
  while (*(text + i) != '\0' && i < MAX_STRING_LEN)
    i++;
  if (i == MAX_STRING_LEN && *(text + i) != '\0')
    return -1; // invalid string
  return i;
}

void initialize_string(string *newString, char *text) {
  int length = stringlen(text);

  newString->length = length;

  if (length == -1) {
    perror("String size exceeds maximum supported value");
    newString->length = MAX_STRING_LEN;
    // only the first MAX_STRING_LEN characters are considered//
  }

  if (text) {
    newString->str = (char *)malloc((newString->length + 1) * sizeof(char));
    stringcpy(newString->str, text);
  } else {
    newString->length = 0;
    newString->str = (char *)malloc(sizeof(char));
    *(newString->str) = '\0';
  }
}

void stringcpy(char *a, char *b) {
  // a <- b
  // a = destination, b = source
  int i = 0;
  while (*(b + i) != '\0') {
    *(a + i) = *(b + i);
    i++;
  }
  *(a + i) = '\0';
}

int stringcmp(char *a, char *b) {
  // compare if two strings are equal
  int i = 0, j = 0;
  while (*(a + i) != '\0' && *(b + j) != '\0' && i < MAX_STRING_LEN &&
         j < MAX_STRING_LEN) {
    if (*(a + i) == *(b + j)) {
      i++;
      j++;
    } else
      return 0;
  }

  return (*(a + i) == '\0' && *(b + j) == '\0');
}

char *convertCharToCString(char ch) {
  char *str = (char *)malloc(2 * sizeof(char));
  *str = ch;
  *(str + 1) = '\0';

  return str;
}

string *toString(int num) {

  string *result = (string *)malloc(sizeof(string));
  initialize_empty_string(result);
  if (num == 0) {
    result->length = 1;
    result->str = convertCharToCString('0');
  }
  while (num > 0) {
    int digit = num % 10;
    num /= 10;
    string *t = (string *)malloc(sizeof(string));
    t->str = convertCharToCString('0' + digit);
    t->length = 1;

    string *temp = stringconcat(t, result);
    free(result->str);
    free(result);
    free(t->str);
    free(t);
    result = temp;
  }

  return result;
}

// code to convert a string to a whole number
// returns INT_MIN is case of some error
int toInteger(char *text) {

  int length = 0;

  while (text[length] != '\0') {
    if (length > 10)
      return INT_MIN;

    if (text[length] < '0' || text[length] > '9')
      return INT_MIN;

    length++;
  }
  if (length == 10 && text[0] > '2')
    return INT_MIN;

  // overflow
  int ans = 0, i = 1;

  while (length > 0) {
    int temp = i * (text[length - 1] - '0');
    if (INT_MAX - temp < ans)
      return INT_MIN; // overflow
    ans += temp;

    if(i < 1e9)
        i *= 10;

    length--;
  }

  return ans;
}

void destroy_string(string **text_ptr) {
  string *text = *text_ptr;
  free(text->str);
  free(text);
  *text_ptr = NULL;
}
