/*
 * Author: Douglas Tallmadge
 *
 * Created: Sunday February 20th 2016
 *
 * Script Name: stack.c
 *
 * Program Description: Simple genaric stack implementation
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>

// ---------------- Local includes
#include "stack.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */
/*
 * Allocates memory for a Stack struct, returns pointer.
 */
Stack* initializeStack(){
	Stack* st = (Stack*) malloc(sizeof(Stack));
	st->top = NULL;
	return st;
}
/*
 * Adds a pointer to data to the stack.  FIFO.
 */
void push(Stack* st, void* data){

	// If top is null, this is first time.
	if (st->top == NULL){
		StackNode* sn = (StackNode*) malloc(sizeof(StackNode));
		sn->data = data;
		sn->next = NULL;

		st->top = sn;
		return;

	} else{ 

		StackNode* sn = (StackNode*) malloc(sizeof(StackNode));
		sn->data = data;		

		sn->next = st->top;
		st->top = sn;
		return;
	}
}
/*
 * Returns and removes the pointer at the top of the stack.
 * returns null if stack is empty.  User responsible for freeing stack
 * when empty.  User also must remove all items before doing so if looking
 * to free before removing all items.
 */
void *pop(Stack* st){
	
	if (st->top == NULL){
		return NULL;

	} else{

		StackNode* temp = st->top;
		void *data = temp->data;

		if (st->top->next != NULL){
			st->top = st->top->next;
		} else {
			st->top = NULL;
		}
		free(temp);
		return data;
	}
}
