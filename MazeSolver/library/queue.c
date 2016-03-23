/*
 * Author: Douglas Tallmadge
 *
 * Created: Sunday February 27th 2016
 *
 * Script Name: queue.c
 *
 * Program Description: Simple genaric queue implementation
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>

// ---------------- Local includes
#include "queue.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */
/*
 * Allocates memory for a Queue struct, returns pointer.
 */
Queue* initializeQueue(){
	Queue* qt = malloc(sizeof(Queue));

	qt->front = NULL;
	qt->back = NULL;

	return qt;
}
/*
 * Adds a pointer to data to the end of the Queue.  FIFO.
 */
void enqueue(Queue* qt, void* data){

	// If this is the first value...
	if (qt->front == NULL){

		QueueNode* new = (QueueNode*) malloc(sizeof(QueueNode));
		new->data = data;
		new->next = NULL;

		qt->front = new;
		qt->back = new;

	} else{  // Otherwise add to the end.

		QueueNode* new = (QueueNode*) malloc(sizeof(QueueNode));
		new->data = data;
		new->next = NULL;

		qt->back->next = new;
		qt->back = new;
	}
}
/*
 * Returns and removes the pointer at the front of the queue.
 * User responsible for freeing the memory of returned data when done.
 * returns NULL if empty.
 */
void *dequeue(Queue* qt){

	// If it is empty, return null
	if (qt->front == NULL){
		return NULL;
	} else {

		QueueNode* temp = qt->front;
		void *data = temp->data;

		qt->front = qt->front->next;

		free(temp);

		return data;
	}

}
/*
 * Returns the pointer of item at front of queue without removing.
 * Warning! If this data pointer is freed before dequeueing, will 
 * cause undefined behavoir.
 */
void *peek(Queue* qt){

	// If it is empty, return null
	if (qt->front == NULL){
		return NULL;
	} else {

		return qt->front->data;
	}

}

