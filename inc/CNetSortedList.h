#pragma once

#ifndef CNET_SORTED_LIST_H
#define CNET_SORTED_LIST_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

/** Comparator for elements: return < 0 if a orders before b, 0 if equal, > 0 if a orders after b. */
typedef int (*SortedListCompareFn)(const void* a, const void* b);

typedef struct sSortedList SortedList;

/**
 * Create an empty list that maintains ascending order via compare as elements are inserted.
 *
 * @param compare  Comparator used to order elements; borrowed, must outlive the list.
 * @return Newly allocated list, owned by the caller; free with sortedListFree().
 */
CNET_API SortedList* sortedListCreate(SortedListCompareFn compare);
/**
 * Free a list and the internal memory it owns; does not free the elements it points to.
 *
 * @param list  List to free; the pointer is invalid after this call.
 */
CNET_API void sortedListFree(SortedList* list);

/**
 * Insert an element at the position that keeps the list in ascending order (stable: after equal elements).
 *
 * @param list     List to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 */
CNET_API void sortedListInsert(SortedList* list, void* element);
/**
 * Get the element at index without removing it.
 *
 * @param list   List to query.
 * @param index  0-based index; must be in 0..size-1.
 * @return Element at index, or NULL if index is out of range.
 */
CNET_API void* sortedListGet(const SortedList* list, int index);
/**
 * Remove the element at index, shifting later elements one position to the left.
 *
 * @param list   List to modify.
 * @param index  0-based index; must be in 0..size-1.
 * @return Removed element, or NULL if index is out of range.
 */
CNET_API void* sortedListRemove(SortedList* list, int index);

/**
 * Find the index of an element equal to element per the list's comparator, using binary search.
 *
 * @param list     List to search.
 * @param element  Value to search for, compared via the list's comparator.
 * @return Index of a matching element, or -1 if not found.
 */
CNET_API int sortedListIndexOf(const SortedList* list, const void* element);
/**
 * Check whether an element equal to element per the list's comparator is present.
 *
 * @param list     List to search.
 * @param element  Value to search for, compared via the list's comparator.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int sortedListContains(const SortedList* list, const void* element);

/**
 * Get the number of elements currently in the list.
 *
 * @param list  List to query.
 * @return Element count (>= 0).
 */
CNET_API int sortedListSize(const SortedList* list);
/**
 * Check whether the list has no elements.
 *
 * @param list  List to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int sortedListIsEmpty(const SortedList* list);

/**
 * Create an iterator over the list in ascending order; supports removal.
 *
 * @param list  List to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over list.
 */
CNET_API Iterator* sortedListIterator(SortedList* list);

#endif
