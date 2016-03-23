/* ========================================================================== */
/* File: ANDORList.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * This file contains the definitions for components of a singly linked list.
 *
 */
/* ========================================================================== */

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types


typedef struct INNERListNode {
    int DOC_ID;     
    int Freq;
    int visited;                		      // the data for a given page
    //struct INNERListNode *prev;                   // pointer to previous node
    struct INNERListNode *next;                   // pointer to next node
} INNERListNode;

typedef struct MAJORListNode {
    int DOC_ID;  
    int Freq;                		      // the data for a given page
    //struct INNERListNode *prev;                   // pointer to previous node
    struct MAJORListNode *next;                   // pointer to next node
} MAJORListNode;

typedef struct ANDORList {
    INNERListNode *head;                          // "beginning" of the list
    //ListNode *tail;                          // "end" of the list
} ANDORList;

typedef struct MAJORList {
    MAJORListNode *head;                          // "beginning" of the list
    //ListNode *tail;                          // "end" of the list
} MAJORList;



int addToINNERList(ANDORList *list, int DOC_ID, int Freq);

int addToMajorList(MAJORList* list, INNERListNode* node);

//INNERListNode* popOff(List *list);


void init_Innerlist(ANDORList *list);

void init_Majorlist(MAJORList *list);
// ---------------- Public Variables

// ---------------- Prototypes/Macros

// #endif // LIST_H
