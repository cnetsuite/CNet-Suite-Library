#pragma once

#ifndef CNET_BITSET_H
#define CNET_BITSET_H

#include "CNetApi.h"

typedef struct sBitSet BitSet;

/**
 * Create a bit vector with a default initial capacity (64 bits). Grows
 * automatically as higher indexes are set.
 *
 * @return New BitSet, owned by the caller and released with bitSetFree.
 */
CNET_API BitSet* bitSetCreate(void);
/**
 * Create a bit vector with a given initial capacity. Grows automatically
 * as higher indexes are set.
 *
 * @param numBits  Initial capacity in bits; must be >= 0. 0 means use the
 *                 default capacity (64 bits).
 * @return New BitSet, owned by the caller and released with bitSetFree,
 *         or NULL (with an error set) if numBits is negative.
 */
CNET_API BitSet* bitSetCreateWithSize(int numBits);
/**
 * Free a bit vector and its internal storage.
 *
 * @param bits  Bit vector to free.
 */
CNET_API void bitSetFree(BitSet* bits);

/**
 * Set a single bit to 1, growing the vector's capacity if needed.
 *
 * @param bits   Bit vector to modify.
 * @param index  Bit index to set; must be >= 0.
 * @return true on success, false (with an error set) if index is
 *         negative.
 */
CNET_API bool bitSetSet(BitSet* bits, int index);
/**
 * Set a single bit to a given value, growing the vector's capacity if
 * needed.
 *
 * @param bits   Bit vector to modify.
 * @param index  Bit index to set; must be >= 0.
 * @param value  0 clears the bit, any nonzero value sets it.
 * @return true on success, false (with an error set) if index is
 *         negative.
 */
CNET_API bool bitSetSetValue(BitSet* bits, int index, int value);
/**
 * Clear a single bit to 0. An index beyond the current capacity is
 * treated as already clear and does not grow the vector.
 *
 * @param bits   Bit vector to modify.
 * @param index  Bit index to clear; must be >= 0.
 * @return true on success, false (with an error set) if index is
 *         negative.
 */
CNET_API bool bitSetClear(BitSet* bits, int index);
/**
 * Clear every bit without changing the vector's capacity.
 *
 * @param bits  Bit vector to modify.
 */
CNET_API void bitSetClearAll(BitSet* bits);
/**
 * Toggle a single bit, growing the vector's capacity if needed.
 *
 * @param bits   Bit vector to modify.
 * @param index  Bit index to flip; must be >= 0.
 * @return true on success, false (with an error set) if index is
 *         negative.
 */
CNET_API bool bitSetFlip(BitSet* bits, int index);
/**
 * Read a single bit.
 *
 * @param bits   Bit vector to query.
 * @param index  Bit index to read; must be >= 0.
 * @return 1 if set, 0 if clear or if index is beyond the current
 *         capacity (or negative, with an error set).
 */
CNET_API int bitSetGet(const BitSet* bits, int index);

/**
 * Count the number of bits currently set to 1.
 *
 * @param bits  Bit vector to query.
 * @return Number of set bits.
 */
CNET_API int bitSetCardinality(const BitSet* bits);
/**
 * Check whether no bits are set.
 *
 * @param bits  Bit vector to query.
 * @return 1 if no bits are set, 0 otherwise.
 */
CNET_API int bitSetIsEmpty(const BitSet* bits);
/**
 * Get the logical length of the vector: one past the index of the
 * highest set bit.
 *
 * @param bits  Bit vector to query.
 * @return Highest set bit index + 1, or 0 if no bits are set.
 */
CNET_API int bitSetLength(const BitSet* bits);
/**
 * Get the current physical capacity of the vector in bits.
 *
 * @param bits  Bit vector to query.
 * @return Current capacity in bits (may grow on future set/flip calls).
 */
CNET_API int bitSetSize(const BitSet* bits);

/**
 * Find the next set bit at or after a given index.
 *
 * @param bits       Bit vector to search.
 * @param fromIndex  Index to start searching from; must be >= 0.
 * @return Index of the next set bit, or -1 if none (or -1, with an error
 *         set, if fromIndex is negative).
 */
CNET_API int bitSetNextSetBit(const BitSet* bits, int fromIndex);
/**
 * Find the next clear bit at or after a given index. Since the vector is
 * conceptually infinite, this always succeeds within (or just past) the
 * current capacity.
 *
 * @param bits       Bit vector to search.
 * @param fromIndex  Index to start searching from; must be >= 0.
 * @return Index of the next clear bit (or -1, with an error set, if
 *         fromIndex is negative).
 */
CNET_API int bitSetNextClearBit(const BitSet* bits, int fromIndex);

/**
 * Intersect this vector with another in place (bitwise AND). Bits beyond
 * other's capacity are cleared.
 *
 * @param bits   Bit vector to modify.
 * @param other  Bit vector to intersect with; not modified.
 */
CNET_API void bitSetAnd(BitSet* bits, const BitSet* other);
/**
 * Union this vector with another in place (bitwise OR), growing capacity
 * if needed.
 *
 * @param bits   Bit vector to modify.
 * @param other  Bit vector to union with; not modified.
 */
CNET_API void bitSetOr(BitSet* bits, const BitSet* other);
/**
 * XOR this vector with another in place, growing capacity if needed.
 *
 * @param bits   Bit vector to modify.
 * @param other  Bit vector to XOR with; not modified.
 */
CNET_API void bitSetXor(BitSet* bits, const BitSet* other);
/**
 * Clear every bit in this vector that is also set in another (bitwise
 * AND NOT), up to the shorter of the two capacities.
 *
 * @param bits   Bit vector to modify.
 * @param other  Bit vector whose set bits should be cleared; not
 *               modified.
 */
CNET_API void bitSetAndNot(BitSet* bits, const BitSet* other);

/**
 * Compare the logical bit patterns of two vectors (differing physical
 * capacities are treated as zero-padded).
 *
 * @param a  First bit vector.
 * @param b  Second bit vector.
 * @return 1 if both vectors have the same bits set, 0 otherwise.
 */
CNET_API int bitSetEquals(const BitSet* a, const BitSet* b);

#endif
