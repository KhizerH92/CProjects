/* ========================================================================== */
/* File: Query.c - Tiny Search Engine Component: Query
 *
 * Author: Khizar Hussain
 * Date: 24/02/2016
 *
 * Input: Word combinations to be looked up.
 *
 *
 * Output: Document ranked from greatest relevance to the query to the least printed
 * to the screen.
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
//-------------------------------------Unnecessary Includes to Be dealt in library -------------------

#include <funcFile.h>
#include <ANDORList.h>
#include <hashtable.h>


//===================Borrowed Functions ===============================================================

// ========================= Check the Main arguments before proceeding ===============================

 void checkArguments(int argc, char **argv){
 	if (argc != 3) {
		printf("Query is to be run with arguments as : ./query [Index data file] [crawler directory with URLS]\n");
		exit(1);
	}

	// Check if the indexed data file and the crawler directory of URLS+HTMLs exists.
	if (!IsFile(argv[1])){
		printf("File of indexed words doesn't exist.\n");
		exit(1);
	}

	if (!IsDir(argv[2])) {
		printf("Directory does not exist.\n");
		exit(1);
	}
 }

//============================================================================
//------------------------ Add to the inner list from wordNodes --------------
int addWordNodeToInner(ANDORList* inner, WordNode* wordTo){
	DocumentNode* docs = wordTo->page;
	// if(!docs){
	// 	printf("There are no docs holding the word %s\n", wordTo->word );
	// 	return -1;
	// }

	while(docs){
		addToINNERList(inner,docs->doc_id,docs->freq);
		docs = docs->next;
	}
	return 1;
}
//--------------------- Second Step to add to the Major List -----------------

int addInnerToMajor(MAJORList* list, ANDORList* insider, int countVal){
	INNERListNode* current = insider->head;
	while(current){
		if(current->visited==countVal){
			//printf("visited value: %d\n", current->visited );
			addToMajorList(list,current);
		}
		current=current->next;
	}
	return 1;
}

//================== Cleaning Functions Go Here: =============================

int cleanHashTableOut(GenHashTable *table){
	for(int j=0; j<MAX_HASH_SLOT; ++j){
		if(!table->table[j]) continue;


	WordNode *wdNd = table->table[j]->wordNode;
	WordNode *freeWord = NULL;

	while(wdNd){
		freeWord = wdNd->next;
		DocumentNode *pageToadd = wdNd->page;
		DocumentNode *nextN = NULL;

		while(pageToadd){
			nextN = pageToadd->next;
			free(pageToadd);
			pageToadd = NULL;
			pageToadd = nextN;
		}
		free(wdNd->word);
		free(wdNd);
		wdNd = NULL;
		wdNd = freeWord;
	}
		
	free(table->table[j]);
	table->table[j]=NULL;

	}
	return 1;
}
// ============================== Let's Sort the list ==========================

void swap(MAJORListNode* n1, MAJORListNode* n2){
	int tempDoc = n1->DOC_ID;
	int tempFreq = n1->Freq;
	n1->DOC_ID = n2->DOC_ID;
	n1->Freq = n2->Freq;
	n2->Freq = tempFreq;
	n2->DOC_ID = tempDoc;
}

void sortTheMajor(MAJORListNode* head){
	MAJORListNode* start = head;
	MAJORListNode* current;
	MAJORListNode* min;

	while(start->next){
		min = start;
		current = start->next;

		while(current){
			if(min->Freq<current->Freq){
				min = current;
			}

			current = current->next;
		}

		swap(start,min);
		start=start->next;
	}
}
//-------------------------------- Linked List cleaner is here -------------------
void freeList(INNERListNode* head)
{
   INNERListNode* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

void freeMajorList(MAJORListNode* head)
{
   MAJORListNode* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

//--------------------------------------------------------------------------------


//------------------------GLOBAL DELARATIONS OF STRUCTURES -----------------------
/* Since CLeanup has to happpen on receiving a quit signal */

char *argBuffer;
char *testedInput;
GenHashTable* LookUpIndex;
char** URLArray;

MAJORList* Majorlist = NULL;
ANDORList* Innerlist= NULL;

void sighandler(int);

int main(int argc, char **argv){
	
	//-------------making sure files and paths are valid ---------
	checkArguments(argc,argv);
	//------------------------------------------------------------


	// ====== This needs to be modified ==========================
	FILE *reBuilt = fopen(argv[1],"r");
	LookUpIndex = calloc(1, sizeof(GenHashTable));
	reBuildFromFile(LookUpIndex, reBuilt);
	fclose(reBuilt);
	// ===========================================================
	// =============== Process files in directory ================
	int docID;
	char **filenames;
	int numOfFiles;
	int counter=0;
	FILE *fp;

	if((numOfFiles=GetFilenamesInDir(argv[2],&filenames))==-1 || numOfFiles==0){
	 	printf("Something is wrong with Directory provided");
	 	return 1;
	 }

	//printf("%d\n", numOfFiles);
	URLArray = calloc(BUFSIZ, sizeof(char*));


	 while(*filenames && counter<numOfFiles){
	 	//printf("%s\n", filenames[counter]);

	 	if((docID = GetDocumentID(filenames[counter]))==-1) {
	 		free(filenames[counter]);
	 		counter+=1;
	 		continue;
	 	}

	 	char *fileLoc = constructPath(filenames[counter],argv[2]);

	 	fp = fopen(fileLoc, "r");
	 	char bufferDiscard[1000];
	 	
		
		if(fgets(bufferDiscard, 1000, fp)!=NULL){
			//printf("%d  %s\n", docID ,bufferDiscard);

			// memset(URLArray[docID],'0',strlen(bufferDiscard)+1);

			URLArray[docID] = calloc(strlen(bufferDiscard)+2, sizeof(char));
			sprintf(URLArray[docID], "%s", bufferDiscard);
			free(fileLoc);
			free(filenames[counter]);
			counter+=1;
			fclose(fp);
			continue;
		}
		free(fileLoc);
		free(filenames[counter]);
		counter+=1;
		fclose(fp);
	}
	free(filenames);


	//----------------------------------------------------------------
	argBuffer = calloc(MAX_QUERY,sizeof(char));

	signal(SIGINT, sighandler);

	while(1){
		fflush(stdin);

		printf("Query:> ");

		fgets(argBuffer, MAX_QUERY, stdin);

		testedInput = processQueryString(argBuffer);

		if(!testedInput){
			continue;
		}

		//printf("%s\n", testedInput);
				//printf("%s\n", testedInput);
		char *line;
		char *word;
		char *saveptr1; 
		char *saveptr2;
		Majorlist = calloc(1,sizeof(MAJORList));
		Innerlist = calloc(1,sizeof(ANDORList));
		
		init_Majorlist(Majorlist);
		//init_Innerlist(Innerlist); this it eh initial placement

		line = strtok_r(testedInput,"\n",&saveptr1);

		while(line){
			//printf("%s\n", line );
			int ANDedCount = 0;
			init_Innerlist(Innerlist);
			word = strtok_r(line," ", &saveptr2);

			while(word){

				ANDedCount += 1;

				WordNode* wordToLook = getHashVal(LookUpIndex,word);
				// if(addWordNodeToInner(Innerlist, wordToLook)==-1) continue; //in this current placement will go into an infinite loop
				if(!wordToLook){
					printf("%s was not found in the index\n", word);
					word = strtok_r(NULL," ",&saveptr2);
					continue;
				}
				//printf("%s\n", wordToLook->word);
				//printf("%d\n", wordToLook->page->freq);
				addWordNodeToInner(Innerlist, wordToLook);
				word = strtok_r(NULL," ",&saveptr2);
			}

			addInnerToMajor(Majorlist,Innerlist,ANDedCount);
			//================temporary list clearing solution ====
			//Innerlist->head = NULL;
			
			freeList(Innerlist->head);
			//-------------------------------------------------------

			line = strtok_r(NULL,"\n",&saveptr1);

		}


		//==================Gotta think about the sorting of the list according to Freq ===========
		if(!(Majorlist->head)){
			freeMajorList(Majorlist->head);	
			continue;
		}

		sortTheMajor(Majorlist->head);

		//--------------------------------- Test Print Stuff Goes here --------------------------
		MAJORListNode* current = Majorlist->head;
		while(current){
			printf("DOC_ID:%d  URL:%s\n", current->DOC_ID, URLArray[current->DOC_ID]);
			current=current->next;

		}

		// ================ Free the major List ===================================================
		if (Majorlist)freeMajorList(Majorlist->head);
		Majorlist->head = NULL;
		free(Innerlist);
		Innerlist = NULL;
		free(Majorlist);
		Majorlist = NULL;

	}


}

void sighandler(int signum){
   //printf("we get here\n");
	for(int n=0;n<BUFSIZ;n++){
		free(URLArray[n]);
	}
	free(URLArray);
	free(testedInput);
	free(argBuffer);
	cleanHashTableOut(LookUpIndex);
	free(LookUpIndex);

	if(Majorlist && Majorlist->head) freeMajorList(Majorlist->head);
	if(Innerlist)free(Innerlist);
	if(Majorlist)free(Majorlist);


	exit(1);
}