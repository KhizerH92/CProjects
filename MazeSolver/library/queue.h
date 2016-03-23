/* ========================================================================== */
/* File: queue.h
 *
 * Contains function prototypes for a simple genaric queue implementation
 */
/* ========================================================================== */
#ifndef QUEUE_H
#define QUEUE_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types

typedef struct QueueNode{
	void* data;
	struct QueueNode *next;
	
} QueueNode;

typedef struct Queue {
	struct QueueNode *front;
	struct QueueNode *back;

} Queue;


// ---------------- Public Variables

// ---------------- Prototypes/Macros
/*
 * Allocates memory for a Queue struct, returns pointer.
 */
Queue* initializeQueue();
/*
 * Adds a pointer to data to the end of the Queue.  FIFO.
 */
void enqueue(Queue* qt, void* data);
/*
 * Returns and removes the pointer at the front of the queue.
 * User responsible for freeing the memory of returned data when done.
 * returns NULL if empty.
 */
void *dequeue(Queue* qt);
/*
 * Returns the pointer of item at front of queue without removing.
 * Warning! If this data pointer is freed before dequeueing, will 
 * cause undefined behavoir.
 */
void *peek(Queue* qt);

#endif // QUEUE_H

