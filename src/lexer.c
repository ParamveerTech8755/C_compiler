#include<stdio.h>
#include<stdlib.h>
#include "include/lexer.h"
#include "include/token.h"

token** lexer(char* filename){
	FILE *file = fopen(filename, "r");
	if(file == NULL){
		perror("Failed to open file %s", filename);
		exit(1);
	}
	int thisChar;

    while ((thisChar = fgetc(file)) != EOF) { // Read character by character
        printf("%c", thisChar); // Print the line
    }

    fclose(file);
}