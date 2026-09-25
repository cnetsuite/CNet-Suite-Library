#pragma once

#ifndef CNET_DICTIONARY_H
#define CNET_DICTIONARY_H

#include "CNetIterator.h"
#include "CNetApi.h"

typedef int (*DictionaryHashFn)(const void* key);
typedef int (*DictionaryEqualsFn)(const void* a, const void* b);

typedef struct sDictionary Dictionary;

/**
 * Create an empty dictionary (hash map). Null keys and null values are not
 * supported.
 *
 * @param hashFn    Callback called as hashFn(key); must return an int hash
 *                  code for the key, consistent with equalsFn.
 * @param equalsFn  Callback called as equalsFn(a, b); must return nonzero
 *                  if the two keys are equal, 0 otherwise.
 * @return New empty Dictionary, owned by the caller and released with
 *         dictionaryFree.
 */
CNET_API Dictionary* dictionaryCreate(DictionaryHashFn hashFn, DictionaryEqualsFn equalsFn);
/**
 * Free a dictionary and its internal storage. Stored keys and values are
 * not freed.
 *
 * @param dictionary  Dictionary to free.
 */
CNET_API void dictionaryFree(Dictionary* dictionary);

/**
 * Get the number of key/value pairs in the dictionary.
 *
 * @param dictionary  Dictionary to query.
 * @return Entry count.
 */
CNET_API int dictionarySize(const Dictionary* dictionary);
/**
 * Check whether the dictionary has no entries.
 *
 * @param dictionary  Dictionary to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int dictionaryIsEmpty(const Dictionary* dictionary);

/**
 * Look up the value associated with a key.
 *
 * @param dictionary  Dictionary to search.
 * @param key         Key to look up; must not be NULL.
 * @return Associated value, or NULL if the key is not present (or NULL,
 *         with an error set, if key is NULL).
 */
CNET_API void* dictionaryGet(const Dictionary* dictionary, const void* key);
/**
 * Insert or update a key/value pair. Keys and values are stored by
 * reference; the dictionary does not take ownership or copy the
 * pointed-to data.
 *
 * @param dictionary  Dictionary to insert into.
 * @param key         Key to associate the value with; must not be NULL;
 *                    caller retains ownership.
 * @param value       Value to store; must not be NULL; caller retains
 *                    ownership.
 * @return Previous value associated with the key, or NULL if the key was
 *         new (or if key/value was NULL, with an error set).
 */
CNET_API void* dictionaryPut(Dictionary* dictionary, void* key, void* value);
/**
 * Remove a key and its value if present.
 *
 * @param dictionary  Dictionary to remove from.
 * @param key         Key to look up and remove; must not be NULL.
 * @return Removed value, or NULL if the key was not present (or NULL,
 *         with an error set, if key is NULL).
 */
CNET_API void* dictionaryRemove(Dictionary* dictionary, const void* key);

/**
 * Create an iterator over the dictionary's keys. Removal via
 * iteratorRemove is not supported.
 *
 * @param dictionary  Dictionary to iterate.
 * @return New Iterator over keys; caller should free it with iteratorFree
 *         when done.
 */
CNET_API Iterator* dictionaryKeys(Dictionary* dictionary);
/**
 * Create an iterator over the dictionary's values. Removal via
 * iteratorRemove is not supported.
 *
 * @param dictionary  Dictionary to iterate.
 * @return New Iterator over values; caller should free it with
 *         iteratorFree when done.
 */
CNET_API Iterator* dictionaryElements(Dictionary* dictionary);

#endif
