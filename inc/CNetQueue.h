#pragma once

#ifndef CNET_QUEUE_H
#define CNET_QUEUE_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sQueueNode QueueNode;

typedef struct sQueue Queue;

/**
 * Create an empty queue.
 *
 * @return Newly allocated queue, owned by the caller; free with queueFree().
 */
CNET_API Queue* queueCreate(void);
/**
 * Free a queue and its nodes; does not free the elements it points to.
 *
 * @param queue  Queue to free; the pointer is invalid after this call.
 */
CNET_API void queueFree(Queue* queue);

/**
 * Add an element to the back of the queue.
 *
 * @param queue    Queue to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 */
CNET_API void queueEnqueue(Queue* queue, void* element);
/**
 * Remove and return the element at the front of the queue.
 *
 * @param queue  Queue to modify.
 * @return Removed front element, or NULL if the queue is empty.
 */
CNET_API void* queueDequeue(Queue* queue);
/**
 * Return the element at the front of the queue without removing it.
 *
 * @param queue  Queue to query.
 * @return Front element, or NULL if the queue is empty.
 */
CNET_API void* queuePeek(const Queue* queue);

/**
 * Check whether the queue has no elements.
 *
 * @param queue  Queue to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int queueIsEmpty(const Queue* queue);
/**
 * Get the number of elements currently in the queue.
 *
 * @param queue  Queue to query.
 * @return Element count (>= 0).
 */
CNET_API int queueSize(const Queue* queue);

/**
 * Create an iterator over the queue, traversing from front to back; supports removal.
 *
 * @param queue  Queue to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over queue.
 */
CNET_API Iterator* queueIterator(Queue* queue);

#endif
