#pragma once

#ifndef CNET_STACK_H
#define CNET_STACK_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sStack Stack;

/**
 * Create an empty stack with a default initial capacity.
 *
 * @return Newly allocated stack, owned by the caller; free with stackFree().
 */
CNET_API Stack* stackCreate(void);
/**
 * Free a stack and the internal memory it owns; does not free the elements it points to.
 *
 * @param stack  Stack to free; the pointer is invalid after this call.
 */
CNET_API void stackFree(Stack* stack);

/**
 * Push an element onto the top of the stack, growing the backing array if needed.
 *
 * @param stack    Stack to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 */
CNET_API void stackPush(Stack* stack, void* element);
/**
 * Remove and return the element on top of the stack.
 *
 * @param stack  Stack to modify.
 * @return Removed top element, or NULL if the stack is empty.
 */
CNET_API void* stackPop(Stack* stack);
/**
 * Return the element on top of the stack without removing it.
 *
 * @param stack  Stack to query.
 * @return Top element, or NULL if the stack is empty.
 */
CNET_API void* stackPeek(const Stack* stack);

/**
 * Check whether the stack has no elements.
 *
 * @param stack  Stack to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int stackIsEmpty(const Stack* stack);
/**
 * Get the number of elements currently on the stack.
 *
 * @param stack  Stack to query.
 * @return Element count (>= 0).
 */
CNET_API int stackSize(const Stack* stack);

/**
 * Create an iterator over the stack, traversing from the bottom (oldest push) to the top; supports removal.
 *
 * @param stack  Stack to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over stack.
 */
CNET_API Iterator* stackIterator(Stack* stack);

#endif
