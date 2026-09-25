#pragma once

#ifndef CNET_TREE_SET_H
#define CNET_TREE_SET_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef int (*TreeSetCompareFn)(const void* a, const void* b);

typedef struct sTreeSet TreeSet;

/**
 * Create an empty set that keeps its elements sorted by a comparator.
 *
 * @param compare  Comparator called as compare(a, b); must return <0, 0, or
 *                 >0 for less-than, equal, or greater-than, consistent with
 *                 the desired ordering.
 * @return New empty TreeSet, owned by the caller and released with
 *         treeSetFree.
 */
CNET_API TreeSet* treeSetCreate(TreeSetCompareFn compare);
/**
 * Free a set and its internal storage. Stored elements are not freed.
 *
 * @param set  Set to free.
 */
CNET_API void treeSetFree(TreeSet* set);

/**
 * Add an element, keeping the set sorted. Stores the pointer by reference;
 * the set does not take ownership or copy the pointed-to data.
 *
 * @param set      Set to add to.
 * @param element  Element to add; caller retains ownership.
 * @return 1 if added, 0 if an equal element (per the comparator) was
 *         already present.
 */
CNET_API int treeSetAdd(TreeSet* set, void* element);
/**
 * Check whether an equal element (per the comparator) is present.
 *
 * @param set      Set to search.
 * @param element  Element to look for.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int treeSetContains(const TreeSet* set, const void* element);
/**
 * Remove an equal element (per the comparator) if present.
 *
 * @param set      Set to remove from.
 * @param element  Element to look for and remove.
 * @return 1 if an element was removed, 0 if none matched.
 */
CNET_API int treeSetRemove(TreeSet* set, const void* element);

/**
 * Get the smallest element in the set.
 *
 * @param set  Set to query.
 * @return Smallest element, or NULL (with an error set) if the set is
 *         empty.
 */
CNET_API void* treeSetFirst(const TreeSet* set);
/**
 * Get the largest element in the set.
 *
 * @param set  Set to query.
 * @return Largest element, or NULL (with an error set) if the set is
 *         empty.
 */
CNET_API void* treeSetLast(const TreeSet* set);

/**
 * Get the number of elements in the set.
 *
 * @param set  Set to query.
 * @return Element count.
 */
CNET_API int treeSetSize(const TreeSet* set);
/**
 * Check whether the set has no elements.
 *
 * @param set  Set to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int treeSetIsEmpty(const TreeSet* set);

/**
 * Create an iterator that visits elements in ascending sorted order.
 * Supports removal of the last-returned element via iteratorRemove.
 *
 * @param set  Set to iterate.
 * @return New Iterator; caller should free it with iteratorFree when done.
 */
CNET_API Iterator* treeSetIterator(const TreeSet* set);

#endif
