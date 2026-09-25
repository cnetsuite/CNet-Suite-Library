#pragma once

#ifndef CNET_HASH_SET_H
#define CNET_HASH_SET_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

/** Hash function for elements: return an int hash code for element (equal elements per equalsFn must hash equal). */
typedef int (*HashSetHashFn)(const void* element);
/** Equality function for elements: return nonzero if a and b are equal, 0 otherwise. */
typedef int (*HashSetEqualsFn)(const void* a, const void* b);

typedef struct sHashSet HashSet;

/**
 * Create an empty hash set using the given element hash and equality functions.
 *
 * @param hashFn    Function computing a hash code for an element; borrowed, must outlive the set.
 * @param equalsFn  Function comparing two elements for equality; borrowed, must outlive the set.
 * @return Newly allocated set, owned by the caller; free with hashSetFree().
 */
CNET_API HashSet* hashSetCreate(HashSetHashFn hashFn, HashSetEqualsFn equalsFn);
/**
 * Free a set and the internal memory it owns; does not free the elements it points to.
 *
 * @param set  Set to free; the pointer is invalid after this call.
 */
CNET_API void hashSetFree(HashSet* set);

/**
 * Add an element to the set if an equal element is not already present.
 *
 * @param set      Set to modify.
 * @param element  Item to store by reference; ownership stays with the caller.
 * @return 1 if the element was added, 0 if an equal element was already present.
 */
CNET_API int hashSetAdd(HashSet* set, void* element);
/**
 * Check whether an equal element is present in the set.
 *
 * @param set      Set to query.
 * @param element  Element to look up.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int hashSetContains(const HashSet* set, const void* element);
/**
 * Remove an equal element from the set, if present.
 *
 * @param set      Set to modify.
 * @param element  Element to remove.
 * @return 1 if an element was removed, 0 if no equal element was present.
 */
CNET_API int hashSetRemove(HashSet* set, const void* element);

/**
 * Get the number of elements currently in the set.
 *
 * @param set  Set to query.
 * @return Element count (>= 0).
 */
CNET_API int hashSetSize(const HashSet* set);
/**
 * Check whether the set has no elements.
 *
 * @param set  Set to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int hashSetIsEmpty(const HashSet* set);

/**
 * Create an iterator over the set's elements, in unspecified (bucket) order; supports removal.
 *
 * @param set  Set to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over set.
 */
CNET_API Iterator* hashSetIterator(HashSet* set);

#endif
