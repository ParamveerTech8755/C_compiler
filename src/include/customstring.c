#include "customstring.h"
#include<stdio.h>
#include<stdlib.h>

string* stringconcat(string* a, string* b){
	int total_length = a->length + b->length;
	string* newString = (string*)malloc(sizeof(struct custom_string));
	newString->length = total_length;
	char* ptr = newString->str = (char*)calloc(total_length+1, sizeof(char));

	for(int i = 0; i < a->length; i++)
		*(ptr++) = *(a->str+i);
	for(int i = a->length; i < total_length; i++)
		*(ptr++) = *(b->str + i - a->length);

	//terminate the new string
	*ptr = '\0';
	return newString;
}

int stringlen(char* text){
	//should be '\0' terminated
	int i = 0;
	while(*(text+i) != '\0' && i < MAX_STRING_LEN)
		i++;
	if(i == MAX_STRING_LEN && *(text+i) != '\0')
		return -1;//invalid string
	return i;
}

void initialize_string(string* newString, char* text){
	if(text){
		newString->length = stringlen(text);
		if(newString->length == -1){
			perror("String size exceeds maximum supported value");
			exit(1);
		}
		newString->str = (char*)malloc((newString->length + 1)*sizeof(char));
		stringcpy(newString->str, text);
	}
	else{
		newString->length = 0;
		newString->str = (char*)malloc(sizeof(char));
		if(newString->str == NULL){
			perror("Insufficient memory");
			exit(EXIT_FAILURE);
		}
		*(newString->str) = '\0';
	}
}

void stringcpy(char* a, char* b){
	//a <- b
	//a = destination, b = source
	int i = 0;
	while(*(b+i) != '\0'){
		*(a+i) = *(b+i);
		i++;
	}
	*(a+i) = '\0';
}

int stringcmp(char* a, char* b){
	//compare if two strings are equal
	int i = 0, j = 0;
	while(*(a+i) != '\0' && *(b+j) != '\0' && i < MAX_STRING_LEN && j < MAX_STRING_LEN){
		if(*(a+i) == *(b+j)){
			i++;
			j++;
		}
		else
			return 0;
	}

	return (*(a+i) == '\0' && *(b+j) == '\0');
}

char* convertCharToCString(char ch){
	char* str = (char*)malloc(2*sizeof(char));
	*str = ch;
	*(str+1) = '\0';
	
	return str;
}

string* toString(int num){
	//only valid for whole numbers

	string* result = (string*)malloc(sizeof(string));
	if(num == 0){
		result->length = 1;
		result->str = convertCharToCString('0');
	}
	while(num > 0){
		int digit = num%10;
		num /= 10;
		string* t = (string*)malloc(sizeof(string));
		t->str = convertCharToCString('0'+digit);
		t->length = 1;

		string* temp = stringconcat(result, t);
		free(result->str);
		free(result);
		free(t->str);
		free(t);
		result = temp;
	}

	return result;
}