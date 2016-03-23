/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Khizar Hussain
 * Date: 10/02/2016
 *
 *Description: This file implements all the functions given in the hastbale header file 
 *				Basically it should be able to hash url strings and add the hashtable nodes
 *				based on those to different slots. It should allow chaining and then it should
 *				allow recovery of these urls as well as allow us to mark the pages as 'visited'.
 *
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

//=====================Another approach! ================================

//================== Modified HashInstert Table ============================

int GenHashInsert(char *wordtoAdd, GenHashTable* hashTable, int docID){
	
	int hashCode = JenkinsHash(wordtoAdd, MAX_HASH_SLOT); //get the hashcode/Index

	//GenHashTableNode *current = calloc(1,sizeof(GenHashTableNode)); //intialize a current Node
	GenHashTableNode *current;

	current = hashTable->table[hashCode]; //assign the new index to it

	if(!current) {
		GenHashTableNode *newHashNode = calloc(1,sizeof(GenHashTableNode));
		WordNode *NodeToAdd = calloc(1,sizeof(WordNode));
		NodeToAdd->word = calloc(strlen(wordtoAdd)+2,sizeof(char));
		sprintf(NodeToAdd->word,"%s",wordtoAdd);
		NodeToAdd->next = NULL;
		NodeToAdd->page = calloc(1,sizeof(DocumentNode));
		NodeToAdd->page->doc_id = docID;
		NodeToAdd->page->freq = 1;
		NodeToAdd->page->next = NULL;
		intptr_t x = hashCode;
		newHashNode->hashKey = (void *)x;
		newHashNode->wordNode = NodeToAdd;
		hashTable->table[hashCode] = newHashNode;
		
		
		return 1;
	}


	WordNode *currentWordNode = NULL;
	currentWordNode = current->wordNode;

	while(currentWordNode && currentWordNode->word && strcmp(wordtoAdd, currentWordNode->word)){ //find out if the slot already has a value
		currentWordNode = currentWordNode->next;
	}

	if(!currentWordNode) {
		
		currentWordNode = current->wordNode;
		
		while(currentWordNode->next){currentWordNode = currentWordNode->next;}
		
		WordNode *NodeToAdd = calloc(1,sizeof(WordNode));
		NodeToAdd->word = calloc(strlen(wordtoAdd)+2,sizeof(char));
		sprintf(NodeToAdd->word,"%s",wordtoAdd);
		NodeToAdd->next = NULL;
		NodeToAdd->page = calloc(1,sizeof(DocumentNode));
		NodeToAdd->page->doc_id = docID;
		NodeToAdd->page->freq = 1;
		NodeToAdd->page->next = NULL;
		currentWordNode->next = NodeToAdd;

		return 1;
	}

	DocumentNode *currentPage = currentWordNode->page;

	while(currentPage && currentPage->doc_id && currentPage->doc_id!=docID){ //find out if the slot already has a value
		currentPage = currentPage->next;
	}

	if(currentPage && currentPage->doc_id==docID){
		currentPage->freq += 1;
		return 1;
	}

	else{
		DocumentNode *currentPage = currentWordNode->page;
		while(currentPage->next){currentPage=currentPage->next;}
		DocumentNode *DocToAdd = calloc(1,sizeof(DocumentNode));
		DocToAdd->next = NULL;
		DocToAdd->doc_id = docID;
		DocToAdd->freq = 1;
		currentPage->next = DocToAdd;
		return 1;
	}

	return 0;
}

//======Following function helps retrieve hashtable node from the hashtable=

WordNode* getHashVal(GenHashTable *hashtable, char *word){
	int hashCode = JenkinsHash(word, MAX_HASH_SLOT);

	GenHashTableNode* h = hashtable->table[hashCode];
	//printf("%s\n", h->wordNode->word);
	if(!h) return NULL;

	WordNode *getNode;
	getNode = h->wordNode;
	//you gotta chekc if url matches before returning the value!

	//printf("this is the get Node word%s\n", getNode->word );
	while(getNode && strcmp(getNode->word, word)){
		getNode = getNode->next;
	}
	return getNode;
	}


// //==========Test function =================================================

// int main(int argc, char* argv[]){
// 	GenHashTable* hashtable = calloc(1, sizeof(GenHashTable));



// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 1));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 2));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 1));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 1));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 2));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 2));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 2));
// 	printf("%d\n", GenHashInsert("whywouldyou", hashtable, 1));
// 	printf("%d\n", GenHashInsert("hashTable", hashtable, 2));

// 	WordNode *returnNode = getHashVal(hashtable,"hashTable");

// 	printf("%s\n", returnNode->word);
	
// 	printf("%d\n", returnNode->page->freq);
// 	printf("%d\n", returnNode->page->doc_id);	
// 	printf("%d\n", returnNode->page->next->doc_id);
// 	printf("%d\n", returnNode->page->next->freq);
// }