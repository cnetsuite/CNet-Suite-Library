#pragma once

#ifndef CNET_LINKED_HASH_MAP_H
#define CNET_LINKED_HASH_MAP_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

typedef int (*LinkedHashMapHashFn)(const void* key);
typedef int (*LinkedHashMapEqualsFn)(const void* a, const void* b);

typedef struct sLinkedHashMapEntry LinkedHashMapEntry;
typedef struct sLinkedHashMap LinkedHashMap;

/**
 * Create an empty hash map that preserves insertion order during iteration.
 *
 * @param hashFn    Callback called as hashFn(key); must return an int hash
 *                  code for the key, consistent with equalsFn.
 * @param equalsFn  Callback called as equalsFn(a, b); must return nonzero
 *                  if the two keys are equal, 0 otherwise.
 * @return New empty LinkedHashMap, owned by the caller and released with
 *         linkedHashMapFree.
 */
CNET_API LinkedHashMap* linkedHashMapCreate(LinkedHashMapHashFn hashFn, LinkedHashMapEqualsFn equalsFn);
/**
 * Free a map and its internal storage. Stored keys and values are not
 * freed.
 *
 * @param map  Map to free.
 */
CNET_API void linkedHashMapFree(LinkedHashMap* map);

/**
 * Insert or update a key/value pair. Keys and values are stored by
 * reference; the map does not take ownership or copy the pointed-to data.
 * A new key is appended to the end of the iteration order; updating an
 * existing key keeps its current position.
 *
 * @param map    Map to insert into.
 * @param key    Key to associate the value with; caller retains ownership.
 * @param value  Value to store; caller retains ownership.
 * @return Previous value associated with the key, or NULL if the key was
 *         new.
 */
CNET_API void* linkedHashMapPut(LinkedHashMap* map, void* key, void* value);
/**
 * Look up the value associated with a key.
 *
 * @param map  Map to search.
 * @param key  Key to look up.
 * @return Associated value, or NULL if the key is not present.
 */
CNET_API void* linkedHashMapGet(const LinkedHashMap* map, const void* key);
/**
 * Check whether a key is present.
 *
 * @param map  Map to search.
 * @param key  Key to look up.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int linkedHashMapContainsKey(const LinkedHashMap* map, const void* key);
/**
 * Remove a key and its value if present.
 *
 * @param map  Map to remove from.
 * @param key  Key to look up and remove.
 * @return Removed value, or NULL if the key was not present.
 */
CNET_API void* linkedHashMapRemove(LinkedHashMap* map, const void* key);

/**
 * Get the number of key/value pairs in the map.
 *
 * @param map  Map to query.
 * @return Entry count.
 */
CNET_API int linkedHashMapSize(const LinkedHashMap* map);
/**
 * Check whether the map has no entries.
 *
 * @param map  Map to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int linkedHashMapIsEmpty(const LinkedHashMap* map);

/**
 * Get the key held by an entry returned from a map iterator.
 *
 * @param entry  Entry to read.
 * @return The entry's key.
 */
CNET_API void* linkedHashMapEntryKey(const LinkedHashMapEntry* entry);
/**
 * Get the value held by an entry returned from a map iterator.
 *
 * @param entry  Entry to read.
 * @return The entry's value.
 */
CNET_API void* linkedHashMapEntryValue(const LinkedHashMapEntry* entry);

/**
 * Create an iterator that visits LinkedHashMapEntry* items in insertion
 * order. Supports removal of the last-returned entry via iteratorRemove.
 *
 * @param map  Map to iterate.
 * @return New Iterator; caller should free it with iteratorFree when done.
 */
CNET_API Iterator* linkedHashMapIterator(LinkedHashMap* map);

#endif
