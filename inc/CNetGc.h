#pragma once

#ifndef CNET_GC_H
#define CNET_GC_H

#include "CNetApi.h"

typedef struct sGc Gc;

/**
 * Create a new allocation arena.
 *
 * @return Newly allocated Gc, owned by the caller; free it with gcFree().
 *         Never returns NULL (aborts the process on allocation failure).
 */
CNET_API Gc* gcCreate(void);

/**
 * Allocate a block of memory tracked by the arena. The block is not
 * zero-initialized and cannot be freed individually; it is released only
 * when the whole arena is freed via gcFreeAll()/gcFree().
 *
 * @param gc    Arena to allocate from; borrowed, not modified structurally.
 * @param size  Number of bytes to allocate.
 * @return Pointer to the allocated block, owned by the arena (do not free
 *         it directly). Never returns NULL (aborts on allocation failure).
 */
CNET_API void* gcAlloc(Gc* gc, size_t size);

/**
 * Free every allocation previously made from the arena, without freeing
 * the arena itself. The arena remains valid and can be reused for further
 * gcAlloc() calls afterward.
 *
 * @param gc  Arena whose allocations should be released; borrowed.
 */
CNET_API void gcFreeAll(Gc* gc);

/**
 * Free every allocation made from the arena and the arena itself.
 *
 * @param gc  Arena to destroy; caller must not use it after this call.
 */
CNET_API void gcFree(Gc* gc);

#endif
