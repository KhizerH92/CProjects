/* ========================================================================== */
/* File: Unit Test for Process Query FUnction testFunc.c - Tiny Search Engine Component: Query
 *
 * Author: Khizar Hussain
 * Date: 24/02/2016
 *
 * Input: Word combinations to be looked up.
 *
 *
 * Output: Prints out tokens one by one to make sure all are processed or gives errors based on criteria
 */
/* ========================================================================== */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>

#define MAX_QUERY 20000

char *argBuffer;
char *testedInput;

void sighandler(int);

void NormalizeWord(char *word)
{
    for(char *ptr = word; *ptr; ++ptr)
        *ptr = tolower(*ptr);
}

void checkAgainstNonAlpha(char *words){
	for(int o=0; o<strlen(words);++o){
			if(isdigit(words[o]) || !isalpha(words[o])){
				printf("Only letters are expected among arguments\n");
				continue;	
			} 
		}
}

char *processQueryString(char *query){
	//if it's just an empty line prompt for arguments;
	if(strcmp(query,"\n")==0){
		printf("Please provide valid arguments\n");
		return NULL;
	}

	if(query[0]==' '){
		printf("Spaces don't count as arguments. Please provide valid arguments\n");
		return NULL;
	}

	//* Assumption: User will not enter a line break character in the query */

	//remove end of the line:
	char *actQuery;
	actQuery = strtok(query,"\n");

	if(!actQuery){
		printf("Please provide a valid query\n");
		return NULL;
	}

	//Now break the query provided down and check for Operators Validity on the query:

	char **checkQ=calloc(strlen(actQuery)+1,sizeof(char*)); //to add all the tokens to an array for testing
	//separate tokens based on space:

	char *token;

	token = strtok(actQuery," ");
	int l = 0;

	//Add all the tokens to an array:
	while(token){
		checkQ[l] = calloc(strlen(token)+1,sizeof(char));
		strcpy(checkQ[l],token);
		//printf("%s\n",checkQ[l]);
		token = strtok(NULL," ");
		l += 1;
	}

	//Run tests on this array, making sure AND and OR are used as binary operators:
	//printf("%s\n", checkQ[0]);

	if ((strcmp(checkQ[0], "OR") == 0 || (strcmp(checkQ[l-1], "OR") == 0 || strcmp(checkQ[0], "AND") == 0)  || strcmp(checkQ[l-1], "AND") == 0)) {
			printf("Query can't start or end with Logical operators \n");
			return NULL;
	}

	// if(strcmp(checkQ[0],"''")==0 || checkQ[0]==NULL){
	// 	printf("Can't start with a space only. Please provide some valid arguments\n");
	// 	return NULL;
	// }

	//check for successive operators:
	for(int k=0;k<l-1;k++){
			if((strcmp(checkQ[k],"OR")==0 || strcmp(checkQ[k],"AND")==0) && (strcmp(checkQ[k+1],"OR")==0 || strcmp(checkQ[k+1],"AND")==0)){
			printf("Two Logical operators cannot follow each other immediately! Please input a correct Query\n");
			return NULL;
		}
	}

	//If everything checks out, return a character buffer for the program to work with:
	char *returnBuffer = calloc(MAX_QUERY,sizeof(char));
	checkAgainstNonAlpha(checkQ[0]);
	NormalizeWord(checkQ[0]);
	strcpy(returnBuffer, checkQ[0]);
	int last = 0;

	for(int n=1;n<l;n++){
		checkAgainstNonAlpha(checkQ[n]);
		if(strcmp(checkQ[n],"AND")==0){
			;
		}
		else if(strcmp(checkQ[n],"OR")==0){
			strcat(returnBuffer,"\n");
			last = 1;
		}
		else{
			if(last!=1){
				strcat(returnBuffer," ");
			}
			NormalizeWord(checkQ[n]);
			strcat(returnBuffer,checkQ[n]);
			last = 0;
		}
	}
	strcat(returnBuffer,"\n");
	for(int n=0;n<l;++n){
		free(checkQ[n]);
	}
	free(checkQ);
	return returnBuffer;
}

//=============================================

int main(int argc, char **argv){
	
	argBuffer = calloc(MAX_QUERY,sizeof(char));
	

	signal(SIGINT, sighandler);



	while(1){
		fflush(stdin);

		printf("Query:> ");

		fgets(argBuffer, MAX_QUERY, stdin);

		testedInput = processQueryString(argBuffer);

		if(!testedInput){
			//printf("Please provide valid query arguments\n");
			continue;
		}
		//printf("%s\n", testedInput);
		char *line;
		char *token;
		char *saveptr1; 
		char *saveptr2;

		line = strtok_r(testedInput,"\n",&saveptr1);
		while(line){
			//printf("%s\n", line );
			int counter=0;
			token = strtok_r(line," ", &saveptr2);
			while(token){
				counter +=1;
				printf("%s\n", token);
				token = strtok_r(NULL," ",&saveptr2);
			}
			printf("%d\n", counter);
			line = strtok_r(NULL,"\n",&saveptr1);

		}
	}

}

void sighandler(int signum){
   printf("we get here\n");
   free(testedInput);
   free(argBuffer);
   exit(1);
}