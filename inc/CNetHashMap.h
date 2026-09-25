#pragma once

#ifndef CNET_HASH_MAP_H
#define CNET_HASH_MAP_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

/** Hash function for keys: return an int hash code for key (equal keys per equalsFn must hash equal). */
typedef int (*HashMapHashFn)(const void* key);
/** Equality function for keys: return nonzero if a and b are equal, 0 otherwise. */
typedef int (*HashMapEqualsFn)(const void* a, const void* b);

typedef struct sHashMapEntry HashMapEntry;

typedef struct sHashMap HashMap;

/**
 * Create an empty hash map using the given key hash and equality functions.
 *
 * @param hashFn    Function computing a hash code for a key; borrowed, must outlive the map.
 * @param equalsFn  Function comparing two keys for equality; borrowed, must outlive the map.
 * @return Newly allocated map, owned by the caller; free with hashMapFree().
 */
CNET_API HashMap* hashMapCreate(HashMapHashFn hashFn, HashMapEqualsFn equalsFn);
/**
 * Free a map and the internal memory it owns; does not free the keys or values it points to.
 *
 * @param map  Map to free; the pointer is invalid after this call.
 */
CNET_API void hashMapFree(HashMap* map);

/**
 * Insert a key/value pair, or replace the value if the key is already present.
 *
 * @param map    Map to modify.
 * @param key    Key to store by reference; ownership stays with the caller.
 * @param value  Value to store by reference; ownership stays with the caller.
 * @return Previous value for key, or NULL if the key was not already present.
 */
CNET_API void* hashMapPut(HashMap* map, void* key, void* value);
/**
 * Look up the value associated with a key.
 *
 * @param map  Map to query.
 * @param key  Key to look up.
 * @return Value for key, or NULL if the key is not present.
 */
CNET_API void* hashMapGet(const HashMap* map, const void* key);
/**
 * Check whether a key is present in the map.
 *
 * @param map  Map to query.
 * @param key  Key to look up.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int hashMapContainsKey(const HashMap* map, const void* key);
/**
 * Remove a key and its value from the map, if present.
 *
 * @param map  Map to modify.
 * @param key  Key to remove.
 * @return Removed value, or NULL if the key was not present.
 */
CNET_API void* hashMapRemove(HashMap* map, const void* key);

/**
 * Get the number of key/value pairs currently in the map.
 *
 * @param map  Map to query.
 * @return Entry count (>= 0).
 */
CNET_API int hashMapSize(const HashMap* map);
/**
 * Check whether the map has no entries.
 *
 * @param map  Map to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int hashMapIsEmpty(const HashMap* map);

/**
 * Get the key stored in a map entry, as returned by hashMapIterator().
 *
 * @param entry  Entry to query, obtained from iterating the map.
 * @return The entry's key.
 */
CNET_API void* hashMapEntryKey(const HashMapEntry* entry);
/**
 * Get the value stored in a map entry, as returned by hashMapIterator().
 *
 * @param entry  Entry to query, obtained from iterating the map.
 * @return The entry's value.
 */
CNET_API void* hashMapEntryValue(const HashMapEntry* entry);

/**
 * Create an iterator over the map's entries, in unspecified (bucket) order; supports removal.
 * Each call to iteratorNext() returns a HashMapEntry*; use hashMapEntryKey()/hashMapEntryValue() to read it.
 *
 * @param map  Map to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over map, yielding HashMapEntry* values.
 */
CNET_API Iterator* hashMapIterator(HashMap* map);

#endif
