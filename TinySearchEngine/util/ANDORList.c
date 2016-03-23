/* ========================================================================== */
/* File: list.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Khizar Hussain
 * Date: Wed Feb 03, 2016
 *
 * Given here is an implementation of list data structure to be used for Crawler
 * List.c has functions to allow addition of listnodes, removal and return of listnodes
 * It basically acts as a queue.
 */
/* ========================================================================== */

 // ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>                          // free, calloc, realloc
#include <ctype.h>                           // tolower, isspace
#include <string.h>                          // strcasestr, strchr, strrchr,
                                             // strncasecmp, memset, strlen,
                                             // memcpy, strnlen, strncpy,
                                             // strcmp, strdup, strndup, strcat,
                                             // strpbrk

#include <curl/curl.h>                       // curl functionality
#include <ctype.h>

// ---------------- Local includes  e.g., "file.h"
//#include "web.h"                             // web functionality
//#include "common.h"                          // common functionality
#include "ANDORList.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

//====following function initializes the empty list so that we can add
 // === list nodes to it ============================================

void init_Innerlist(ANDORList *INNERList){

	INNERList->head = NULL;
	//URLList->tail = NULL;
}

void init_Majorlist(MAJORList *MAJORList){

	MAJORList->head = NULL;
	//URLList->tail = NULL;
}
/* ========================================================================== */
/* List Implementation with add and popOff functions:
*			
*
*
*/
/* ========================================================================== */

//=============================== First deal with add function inner list ===//

int addToINNERList(ANDORList *list, int DOC_ID, int Freq){
	INNERListNode *current = list->head;
	if(current==NULL){
		INNERListNode *new = calloc(1,sizeof(INNERListNode));
		new->DOC_ID = DOC_ID;
		new->Freq = Freq;
		new->visited = 1;
		new->next = NULL;
		list->head = new;
		return 1;
	}


	while(current->next && current->DOC_ID!=DOC_ID){
		current=current->next;
	}

	if(current->DOC_ID==DOC_ID){
		current->Freq+=Freq;
		current->visited += 1;
		return 1;
	}

	else{
		INNERListNode *new = calloc(1,sizeof(INNERListNode));
		new->DOC_ID = DOC_ID;
		new->Freq = Freq;
		new->visited = 1;
		new->next = NULL;
		current->next = new;
		return 1;
	}
	return 0;
}


int addToMajorList(MAJORList* list, INNERListNode* INNERListNode){
	MAJORListNode *newAdd = calloc(1,sizeof(MAJORListNode));
	newAdd->DOC_ID = INNERListNode->DOC_ID;
	newAdd->Freq = INNERListNode->Freq;
	newAdd->next = NULL;

	MAJORListNode* current = list->head;
	
	if(!current){
		list->head = newAdd;
		return 1;
	}

	else{

		while(current->next){
			current=current->next;
		}
		current->next = newAdd;
		return 1;
	}
	return 0;
}

//==========================Test function ===========================
// int main(int argc, char* argv[]){

// 	MAJORList* majorlist = calloc(1,sizeof(MAJORList));
// 	ANDORList* smalllist = calloc(1,sizeof(ANDORList));


// 	init_Majorlist(majorlist);

// 	init_Innerlist(smalllist);

// 	addToINNERList(smalllist, 162, 5);
// 	addToINNERList(smalllist, 162, 5);
// 	addToINNERList(smalllist, 163, 6);
// 	addToINNERList(smalllist, 163, 6);
// 	addToINNERList(smalllist, 163, 7);

// 	addToMajorList(majorlist, smalllist->head);
// 	addToMajorList(majorlist, smalllist->head->next);
	
// 	printf("%d\n", smalllist->head->Freq);
// 	printf("%d\n", smalllist->head->next->DOC_ID);
// 	printf("%d\n", smalllist->head->next->visited);
// 	printf("%d\n", majorlist->head->Freq);
// 	printf("%d\n", majorlist->head->next->DOC_ID);
// 	printf("%d\n", majorlist->head->next->Freq);

// }