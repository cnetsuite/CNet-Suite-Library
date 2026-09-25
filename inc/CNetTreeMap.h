#pragma once

#ifndef CNET_TREE_MAP_H
#define CNET_TREE_MAP_H

#include "CNetIterator.h"
#include "CNetGc.h"
#include "CNetApi.h"

/** Comparator for keys: return < 0 if a orders before b, 0 if equal, > 0 if a orders after b. */
typedef int (*TreeMapCompareFn)(const void* a, const void* b);

typedef struct sTreeMapNode TreeMapNode;

typedef struct sTreeMap TreeMap;

/**
 * Create an empty ordered map keyed and ordered by compare (an unbalanced binary search tree).
 *
 * @param compare  Comparator used to order keys; borrowed, must outlive the map.
 * @return Newly allocated map, owned by the caller; free with treeMapFree().
 */
CNET_API TreeMap* treeMapCreate(TreeMapCompareFn compare);
/**
 * Free a map and the internal memory it owns; does not free the keys or values it points to.
 *
 * @param map  Map to free; the pointer is invalid after this call.
 */
CNET_API void treeMapFree(TreeMap* map);

/**
 * Insert a key/value pair, or replace the value if a key comparing equal is already present.
 *
 * @param map    Map to modify.
 * @param key    Key to store by reference; ownership stays with the caller.
 * @param value  Value to store by reference; ownership stays with the caller.
 * @return Previous value for the key, or NULL if the key was not already present.
 */
CNET_API void* treeMapPut(TreeMap* map, void* key, void* value);
/**
 * Look up the value associated with a key.
 *
 * @param map  Map to query.
 * @param key  Key to look up, compared via the map's comparator.
 * @return Value for key, or NULL if the key is not present.
 */
CNET_API void* treeMapGet(const TreeMap* map, const void* key);
/**
 * Check whether a key is present in the map.
 *
 * @param map  Map to query.
 * @param key  Key to look up, compared via the map's comparator.
 * @return 1 if present, 0 otherwise.
 */
CNET_API int treeMapContainsKey(const TreeMap* map, const void* key);
/**
 * Remove a key and its value from the map, if present.
 *
 * @param map  Map to modify.
 * @param key  Key to remove, compared via the map's comparator.
 * @return Removed value, or NULL if the key was not present.
 */
CNET_API void* treeMapRemove(TreeMap* map, const void* key);

/**
 * Get the smallest key in the map according to its comparator.
 *
 * @param map  Map to query.
 * @return Smallest key, or NULL if the map is empty.
 */
CNET_API void* treeMapFirstKey(const TreeMap* map);
/**
 * Get the largest key in the map according to its comparator.
 *
 * @param map  Map to query.
 * @return Largest key, or NULL if the map is empty.
 */
CNET_API void* treeMapLastKey(const TreeMap* map);

/**
 * Get the number of key/value pairs currently in the map.
 *
 * @param map  Map to query.
 * @return Entry count (>= 0).
 */
CNET_API int treeMapSize(const TreeMap* map);
/**
 * Check whether the map has no entries.
 *
 * @param map  Map to query.
 * @return 1 if empty, 0 otherwise.
 */
CNET_API int treeMapIsEmpty(const TreeMap* map);

/**
 * Get the key stored in a tree node, as returned by treeMapIterator().
 *
 * @param node  Node to query, obtained from iterating the map.
 * @return The node's key.
 */
CNET_API void* treeMapNodeKey(const TreeMapNode* node);
/**
 * Get the value stored in a tree node, as returned by treeMapIterator().
 *
 * @param node  Node to query, obtained from iterating the map.
 * @return The node's value.
 */
CNET_API void* treeMapNodeValue(const TreeMapNode* node);

/**
 * Create an iterator over the map's entries in ascending key order; supports removal.
 * Each call to iteratorNext() returns a TreeMapNode*; use treeMapNodeKey()/treeMapNodeValue() to read it.
 *
 * @param map  Map to iterate; the iterator's lifetime and validity are tied to it.
 * @return New Iterator over map, yielding TreeMapNode* values.
 */
CNET_API Iterator* treeMapIterator(const TreeMap* map);

#endif
