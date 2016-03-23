/* ========================================================================== */
/* File: Unit Test for Directory Processing Query.c - Tiny Search Engine Component: Query
 *
 * Author: Khizar Hussain
 * Date: 24/02/2016
 * Input : Takes a directory and gives file names and urls as output
 *
 * Command line options: '--help' to get the instructions on usage
 *
 * Output: Document ranked from greatest relevance to the query to the least printed
 * to the screen.
 */
/* ========================================================================== */

// #define _GNU_SOURCE#
#define _DEFAULT_SOURCE 

// #define MAIN_DATA_FILE ../util/index.dat //need to make sure this works
// #define MAX_QUERY 20000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>

//-------------------------------------Unnecessary Includes to Be dealt in library -------------------
#include "file.c"
//#include "hashtable.h"
//#include "ANDORList.h"
#include "../util/hashtable.c"
#include "../util/ANDORList.c"


int GetDocumentID(char* fileName){
	for(int i=0; i<strlen(fileName); ++i){
		if(!isdigit(fileName[i])){
			//printf("wrong name");
			return -1;
		}
	}
	int x = atoi(fileName);
	return x;
}

char* constructPath(char *fileadd, char* mainPath){
	char* path = calloc(1,strlen(fileadd));
	int pathLen = strlen(mainPath);
	if(mainPath[pathLen-1]!='/'){
		strcpy(path,mainPath);
		strcat(path,"/");
		strcat(path,fileadd);	
	}
	else{
		strcpy(path,mainPath);
		strcat(path,fileadd);
	}
	return path;
}

int main(int argc, char **argv){

	int docID;
	char **filenames;
	int numOfFiles;
	int counter=0;
	FILE *fp;




	if((numOfFiles=GetFilenamesInDir(argv[1],&filenames))==-1 || numOfFiles==0){
	 	printf("Something is wrong with Directory provided");
	 	return 1;
	 }

	char checkQ[numOfFiles+500][10000];

	 while(*filenames && counter<numOfFiles){
	 	//printf("%s\n", filenames[counter]);

	 	if((docID = GetDocumentID(filenames[counter]))==-1) {
	 		free(filenames[counter]);
	 		counter+=1;
	 		continue;
	 	}

	 	char *fileLoc = constructPath(filenames[counter],argv[1]);

	 	fp = fopen(fileLoc, "r");
	 	char bufferDiscard[1000];
		
		if(fgets(bufferDiscard, 1000, fp)!=NULL){
			printf("%d  %s\n", docID ,bufferDiscard);

			strcpy(checkQ[docID],bufferDiscard);
			
			free(filenames[counter]);
			counter+=1;
			fclose(fp);
			continue;
		}
		
		free(filenames[counter]);
		counter+=1;
		fclose(fp);
	}

	for(int i=0; i<numOfFiles;++i){
		printf("here kind of confusing\n");
		if(checkQ[i]!=NULL){
			printf("%s\n", checkQ[i]);

		}
	}


}