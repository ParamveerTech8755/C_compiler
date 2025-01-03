#include<stdlib.h>

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("Error: Incorrect usage\nCorrect usage: ./mycompiler <filename>\n");
		return 1;//returns with error code.. fine
	}
	return 0;
}