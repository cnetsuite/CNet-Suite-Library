#pragma once

#ifndef CNET_ITERATOR_H
#define CNET_ITERATOR_H

#include "CNetApi.h"

typedef struct sIterator Iterator;

typedef int (*IteratorHasNextFn)(Iterator* it);
typedef void* (*IteratorNextFn)(Iterator* it);
typedef void (*IteratorRemoveFn)(Iterator* it);
typedef void (*IteratorFreeStateFn)(void* state);

struct sIterator {
    void* state;                        /* Opaque per-iterator cursor data owned by the producing collection */
    IteratorHasNextFn hasNextFn;        /* Called by iteratorHasNext() */
    IteratorNextFn nextFn;              /* Called by iteratorNext() */
    IteratorRemoveFn removeFn;          /* Called by iteratorRemove(); NULL if removal is not supported */
    IteratorFreeStateFn freeStateFn;    /* Called by iteratorFree(); NULL if state needs no extra cleanup */
};

/**
 * Check whether the iterator has another element to visit. This is the
 * shared cursor type returned by TreeSet, LinkedHashMap, Dictionary,
 * TreeNode, Graph and other collections for uniform traversal.
 *
 * @param it  Iterator to query.
 * @return 1 if iteratorNext() would return another element, 0 otherwise.
 */
CNET_API int iteratorHasNext(Iterator* it);
/**
 * Advance the iterator and return the next element.
 *
 * @param it  Iterator to advance.
 * @return Next element (borrowed from the underlying collection), or NULL
 *         (with an error set) if there are no more elements.
 */
CNET_API void* iteratorNext(Iterator* it);
/**
 * Remove the last element returned by iteratorNext() from the underlying
 * collection. Not every iterator supports this.
 *
 * @param it  Iterator whose last-returned element should be removed.
 * @return true if removal is supported and was performed, false (with an
 *         error set) if this iterator does not support removal.
 */
CNET_API bool iteratorRemove(Iterator* it);
/**
 * Release any extra internal resources owned by the iterator's state
 * (e.g. a helper queue or visited-set allocated for a traversal). Safe to
 * call on iterators that need no such cleanup.
 *
 * @param it  Iterator to release.
 */
CNET_API void iteratorFree(Iterator* it);

#endif
