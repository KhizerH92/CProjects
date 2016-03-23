/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a hashtable of urls.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types

typedef struct GenHashTableNode {
    void *hashKey;		// key Could be anything now
   	//int visited;
    WordNode *wordNode;
    //struct GenHashTableNode *next;    // pointer to next node
} GenHashTableNode;

typedef struct HashTable {
    GenHashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} GenHashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

/*The function adds a word to the hashtable if it hasn't been added before and
if it has been added before it either updates the document ID of that word or 
it's frequency in a certain document depending on the case*/

int GenHashInsert(char *word, GenHashTable* hashTable, int DOC_ID);

WordNode* getHashVal(GenHashTable *hashtable, char *word);


#endif // HASHTABLE_H
