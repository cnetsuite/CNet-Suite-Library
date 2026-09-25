#pragma once

#ifndef CNET_LINQ_H
#define CNET_LINQ_H

#include "CNetIterator.h"
#include "CNetArrayList.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef int (*LinqPredicateFn)(const void* element, void* context);
typedef void* (*LinqSelectorFn)(const void* element, void* context);
typedef int (*LinqCompareFn)(const void* a, const void* b);
typedef void* (*LinqAccumulatorFn)(void* accumulator, const void* element, void* context);

/**
 * Filter elements matching a predicate, fully consuming the iterator.
 *
 * @param gc         Allocation arena for the returned list.
 * @param it         Source iterator; consumed by this call.
 * @param predicate  Callback called as predicate(element, context); must
 *                    return nonzero to include the element.
 * @param context    Opaque value passed through to predicate unchanged;
 *                    may be NULL.
 * @return New ArrayList of the matching elements, in source order.
 */
CNET_API ArrayList* linqWhere(Gc* gc, Iterator* it, LinqPredicateFn predicate, void* context);
/**
 * Project each element to a new value, fully consuming the iterator.
 *
 * @param gc        Allocation arena for the returned list.
 * @param it        Source iterator; consumed by this call.
 * @param selector  Callback called as selector(element, context); must
 *                   return the transformed value for that element.
 * @param context   Opaque value passed through to selector unchanged;
 *                   may be NULL.
 * @return New ArrayList of the projected values, in source order.
 */
CNET_API ArrayList* linqSelect(Gc* gc, Iterator* it, LinqSelectorFn selector, void* context);
/**
 * Sort elements, fully consuming the iterator.
 *
 * @param gc       Allocation arena for the returned list.
 * @param it       Source iterator; consumed by this call.
 * @param compare  Comparator called as compare(a, b); must return <0, 0,
 *                 or >0 for less-than, equal, or greater-than.
 * @return New ArrayList with all source elements in sorted order.
 */
CNET_API ArrayList* linqOrderBy(Gc* gc, Iterator* it, LinqCompareFn compare);
/**
 * Remove duplicate elements, fully consuming the iterator, keeping the
 * first occurrence of each distinct value in source order.
 *
 * @param gc       Allocation arena for the returned list.
 * @param it       Source iterator; consumed by this call.
 * @param compare  Comparator called as compare(a, b); must return 0 when
 *                  the two elements are considered duplicates.
 * @return New ArrayList of distinct elements, in source order.
 */
CNET_API ArrayList* linqDistinct(Gc* gc, Iterator* it, LinqCompareFn compare);
/**
 * Take up to the first count elements, consuming that many (or fewer, if
 * the sequence is shorter) from the iterator.
 *
 * @param gc     Allocation arena for the returned list.
 * @param it     Source iterator; consumed up to count elements.
 * @param count  Maximum number of elements to take; 0 returns an empty
 *               list.
 * @return New ArrayList with at most count elements, in source order.
 */
CNET_API ArrayList* linqTake(Gc* gc, Iterator* it, int count);
/**
 * Skip the first count elements, fully consuming the iterator, and
 * collect the rest.
 *
 * @param gc     Allocation arena for the returned list.
 * @param it     Source iterator; consumed by this call.
 * @param count  Number of leading elements to discard; 0 keeps all
 *               elements.
 * @return New ArrayList of the remaining elements, in source order.
 */
CNET_API ArrayList* linqSkip(Gc* gc, Iterator* it, int count);
/**
 * Collect elements in reverse order, fully consuming the iterator.
 *
 * @param gc  Allocation arena for the returned list.
 * @param it  Source iterator; consumed by this call.
 * @return New ArrayList with the source elements in reverse order.
 */
CNET_API ArrayList* linqReverse(Gc* gc, Iterator* it);
/**
 * Materialize all remaining elements into a list, fully consuming the
 * iterator.
 *
 * @param gc  Allocation arena for the returned list.
 * @param it  Source iterator; consumed by this call.
 * @return New ArrayList with the source elements, in source order.
 */
CNET_API ArrayList* linqToArrayList(Gc* gc, Iterator* it);

/**
 * Count the remaining elements, fully consuming the iterator.
 *
 * @param it  Source iterator; consumed by this call.
 * @return Number of elements visited.
 */
CNET_API int linqCount(Iterator* it);
/**
 * Check whether any remaining element matches a predicate. Stops
 * consuming the iterator as soon as a match is found.
 *
 * @param it         Source iterator; partially or fully consumed.
 * @param predicate  Callback called as predicate(element, context); must
 *                    return nonzero for a match.
 * @param context    Opaque value passed through to predicate unchanged;
 *                    may be NULL.
 * @return 1 if any element matches, 0 if none do.
 */
CNET_API int linqAny(Iterator* it, LinqPredicateFn predicate, void* context);
/**
 * Check whether every remaining element matches a predicate, fully
 * consuming the iterator (unless a non-match is found first).
 *
 * @param it         Source iterator; consumed by this call.
 * @param predicate  Callback called as predicate(element, context); must
 *                    return nonzero for a match.
 * @param context    Opaque value passed through to predicate unchanged;
 *                    may be NULL.
 * @return 1 if all elements match (including an empty sequence), 0 if any
 *         element does not.
 */
CNET_API int linqAll(Iterator* it, LinqPredicateFn predicate, void* context);
/**
 * Get the first remaining element, consuming just that one element.
 *
 * @param it  Source iterator.
 * @return First element, or NULL (with an error set) if the sequence is
 *         empty.
 */
CNET_API void* linqFirst(Iterator* it);
/**
 * Get the first remaining element, or a default if there is none.
 *
 * @param it            Source iterator.
 * @param defaultValue  Value to return if the sequence is empty.
 * @return First element, or defaultValue if the sequence is empty.
 */
CNET_API void* linqFirstOrDefault(Iterator* it, void* defaultValue);
/**
 * Fold the remaining elements into a single accumulated value, fully
 * consuming the iterator.
 *
 * @param it           Source iterator; consumed by this call.
 * @param seed         Initial accumulator value.
 * @param accumulator  Callback called as accumulator(acc, element,
 *                      context); must return the updated accumulator.
 * @param context      Opaque value passed through to accumulator
 *                      unchanged; may be NULL.
 * @return Final accumulator value (equal to seed if the sequence was
 *         empty).
 */
CNET_API void* linqAggregate(Iterator* it, void* seed, LinqAccumulatorFn accumulator, void* context);

#endif
