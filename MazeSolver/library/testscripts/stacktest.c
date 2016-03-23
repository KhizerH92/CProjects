/*
 * Author: Douglas Tallmadge
 *
 * Created: Sunday February 27th 2016
 *
 * Script Name: stacktest.c
 *
 * Program Description: A script to test the genaric stack data type in stack.c
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>
#include <stdio.h>

// ---------------- Local includes
#include "stack.h"

// ---------------- Constant definitions
#define MAX_ADD 10000
#define STRING_LEN 10
// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
void randString(int length, char* str);

/* ========================================================================== */

int main()
{
	Stack* stack = initializeStack();
	Stack* stack2 = initializeStack();

	// Add MAX_ADD objects of varying types to stack, then pop them.
	for(int i = 0; i < MAX_ADD; i++){
		int* rnum = malloc(sizeof(int));

		*rnum = rand() % MAX_ADD;

		push(stack, rnum);
	}

	// Add some other objects..
	for(int i = 0; i < MAX_ADD; i++){
		char* str = malloc(sizeof(char) * STRING_LEN + 1);
		randString(STRING_LEN, str);
		push(stack2, str);
	}
	// Now Pop Everything .....
	void* var;

	while((var = pop(stack)) != NULL){
		printf("%d\n", *((int*) var));
		free(var);
	}
	free(stack);


	while((var = pop(stack2)) != NULL){
		printf("%s\n", (char*) var);
		free(var);
	}
	free(stack2);

	return 0;
}

void randString(int length, char* str){

	char* charSet = "abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < length; i++)
	{

		int r = rand() % 26;

		str[i] = charSet[r];
	}
}
