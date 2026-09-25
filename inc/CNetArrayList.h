#pragma once

#ifndef CNET_ARRAY_LIST_H
#define CNET_ARRAY_LIST_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sArrayList ArrayList;

/**
 * Create an empty array list with a default initial capacity.
 *
 * @return Newly allocated list, owned by the caller; free with arrayListFree().
 */
CNET_API ArrayList* arrayListCreate(void);
/**
 * Create an empty array list pre-sized to hold initialCapacity elements without reallocating.
 *
 * @param initialCapacity  Initial backing-array capacity; must be >= 0.
 * @return New list, owned by the caller, or NULL if initialCapacity is negative.
 */
CNET_API ArrayList* arrayListCreateWithCapacity(int initialCapacity);
/**
 * Free a list and the internal memory it owns; does not free the elements it points to.
 *
 * @param arr  List to free; the pointer is invalid after this call.
 */
CNET_API void arrayListFree(ArrayList* arr);

/**
 * Get the number of elements currently in the list.
 *
 * @param arr  List to query.
 * @return Element count (>= 0).
 */
CNET_API int arrayListSize(const ArrayList* arr);
/**
 * Check whether the list has no elements.
 *
 * @param arr  List to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int arrayListIsEmpty(const ArrayList* arr);
/**
 * Get the current backing-array capacity (slots available before the next growth).
 *
 * @param arr  List to query.
 * @return Current capacity (>= size).
 */
CNET_API int arrayListCapacity(const ArrayList* arr);

/**
 * Append an element to the end of the list, growing the backing array if needed.
 *
 * @param arr      List to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 */
CNET_API void arrayListAdd(ArrayList* arr, void* element);
/**
 * Insert an element at index, shifting later elements one position to the right.
 *
 * @param arr      List to modify.
 * @param index    Insertion position; valid range is 0..size (size means append).
 * @param element  Item to store by reference; ownership stays with the caller.
 * @return true on success; false if index is out of range.
 */
CNET_API bool arrayListInsert(ArrayList* arr, int index, void* element);
/**
 * Get the element stored at index without removing it.
 *
 * @param arr    List to query.
 * @param index  0-based index; must be in 0..size-1.
 * @return Element at index, or NULL if index is out of range.
 */
CNET_API void* arrayListGet(const ArrayList* arr, int index);
/**
 * Replace the element stored at index.
 *
 * @param arr      List to modify.
 * @param index    0-based index; must be in 0..size-1.
 * @param element  New value to store by reference; ownership stays with the caller.
 * @return Previous element at that index, or NULL if index is out of range.
 */
CNET_API void* arrayListSet(ArrayList* arr, int index, void* element);
/**
 * Remove the element at index, shifting later elements one position to the left.
 *
 * @param arr    List to modify.
 * @param index  0-based index; must be in 0..size-1.
 * @return Removed element, or NULL if index is out of range. The caller owns the returned pointer.
 */
CNET_API void* arrayListRemove(ArrayList* arr, int index);

/**
 * Find the first index holding a pointer equal (by identity) to element.
 *
 * @param arr      List to search.
 * @param element  Pointer to search for; compared by pointer identity, not by value.
 * @return Index of the first match, or -1 if not found.
 */
CNET_API int arrayListIndexOf(const ArrayList* arr, const void* element);
/**
 * Check whether element's pointer value is present in the list.
 *
 * @param arr      List to search.
 * @param element  Pointer to search for; compared by pointer identity, not by value.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int arrayListContains(const ArrayList* arr, const void* element);

/**
 * Remove all elements without freeing them; capacity is left unchanged.
 *
 * @param arr  List to clear.
 */
CNET_API void arrayListClear(ArrayList* arr);

/**
 * Create an iterator over the list, traversing elements from index 0 to size-1; supports removal.
 *
 * @param arr  List to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over arr.
 */
CNET_API Iterator* arrayListIterator(ArrayList* arr);

#endif
