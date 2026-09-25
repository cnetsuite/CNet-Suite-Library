#pragma once

#ifndef CNET_LINKED_LIST_H
#define CNET_LINKED_LIST_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sLinkedListNode LinkedListNode;

typedef struct sLinkedList LinkedList;

/**
 * Create an empty doubly linked list.
 *
 * @return Newly allocated list, owned by the caller; free with linkedListFree().
 */
CNET_API LinkedList* linkedListCreate(void);
/**
 * Free a list and its nodes; does not free the elements it points to.
 *
 * @param list  List to free; the pointer is invalid after this call.
 */
CNET_API void linkedListFree(LinkedList* list);

/**
 * Insert an element at the head of the list in O(1).
 *
 * @param list     List to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 */
CNET_API void linkedListAddFirst(LinkedList* list, void* element);
/**
 * Insert an element at the tail of the list in O(1).
 *
 * @param list     List to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 */
CNET_API void linkedListAddLast(LinkedList* list, void* element);
/**
 * Remove and return the element at the head of the list.
 *
 * @param list  List to modify.
 * @return Removed element, or NULL if the list is empty.
 */
CNET_API void* linkedListRemoveFirst(LinkedList* list);
/**
 * Remove and return the element at the tail of the list.
 *
 * @param list  List to modify.
 * @return Removed element, or NULL if the list is empty.
 */
CNET_API void* linkedListRemoveLast(LinkedList* list);
/**
 * Return the element at the head of the list without removing it.
 *
 * @param list  List to query.
 * @return Head element, or NULL if the list is empty.
 */
CNET_API void* linkedListPeekFirst(const LinkedList* list);
/**
 * Return the element at the tail of the list without removing it.
 *
 * @param list  List to query.
 * @return Tail element, or NULL if the list is empty.
 */
CNET_API void* linkedListPeekLast(const LinkedList* list);

/**
 * Get the element at index, traversing from whichever end is closer.
 *
 * @param list   List to query.
 * @param index  0-based index; must be in 0..size-1.
 * @return Element at index, or NULL if index is out of range.
 */
CNET_API void* linkedListGet(const LinkedList* list, int index);
/**
 * Replace the element at index.
 *
 * @param list     List to modify.
 * @param index    0-based index; must be in 0..size-1.
 * @param element  New value to store by reference; ownership stays with the caller.
 * @return Previous element at that index, or NULL if index is out of range.
 */
CNET_API void* linkedListSet(LinkedList* list, int index, void* element);
/**
 * Insert an element at index, shifting later elements back by one.
 *
 * @param list     List to modify.
 * @param index    Insertion position; valid range is 0..size (size means append at tail).
 * @param element  Item to store by reference; ownership stays with the caller.
 * @return true on success; false if index is out of range.
 */
CNET_API bool linkedListInsert(LinkedList* list, int index, void* element);
/**
 * Remove and return the element at index.
 *
 * @param list   List to modify.
 * @param index  0-based index; must be in 0..size-1.
 * @return Removed element, or NULL if index is out of range.
 */
CNET_API void* linkedListRemoveAt(LinkedList* list, int index);

/**
 * Find the first index holding a pointer equal (by identity) to element.
 *
 * @param list     List to search.
 * @param element  Pointer to search for; compared by pointer identity, not by value.
 * @return Index of the first match, or -1 if not found.
 */
CNET_API int linkedListIndexOf(const LinkedList* list, const void* element);
/**
 * Check whether element's pointer value is present in the list.
 *
 * @param list     List to search.
 * @param element  Pointer to search for; compared by pointer identity, not by value.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int linkedListContains(const LinkedList* list, const void* element);

/**
 * Get the number of elements currently in the list.
 *
 * @param list  List to query.
 * @return Element count (>= 0).
 */
CNET_API int linkedListSize(const LinkedList* list);
/**
 * Check whether the list has no elements.
 *
 * @param list  List to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int linkedListIsEmpty(const LinkedList* list);

/**
 * Create an iterator over the list, traversing from head to tail; supports removal.
 *
 * @param list  List to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over list.
 */
CNET_API Iterator* linkedListIterator(LinkedList* list);

#endif
