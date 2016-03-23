/* ========================================================================== */
/* File: stack.h
 *
 * Contains function prototypes for a simple genaric stack implementation
 */
/* ========================================================================== */
#ifndef STACK_H
#define STACK_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types

typedef struct StackNode{
	void* data;
	struct StackNode *next;
	
}StackNode;

typedef struct Stack {
	struct StackNode *top;
} Stack;


// ---------------- Public Variables

// ---------------- Prototypes/Macros
/*
 * Allocates memory for a Stack struct, returns pointer.
 */
Stack* initializeStack();
/*
 * Adds a pointer to data to the stack.  LIFO.
 */
void push(Stack* st, void* data);
/*
 * Returns and removes the pointer at the top of the stack.
 * User responsible for freeing the memory of returned data when done.
 */
void *pop(Stack* st);

#endif // STACK_H
