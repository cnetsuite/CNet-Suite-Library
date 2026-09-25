#pragma once

#ifndef CNET_LOCK_H
#define CNET_LOCK_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sLock Lock;

/**
 * Create a new reentrant lock with a built-in condition variable.
 *
 * @return New Lock*, owned by the caller; free with lockFree.
 */
CNET_API Lock* lockCreate(void);
/**
 * Free a lock's resources.
 *
 * @param lock  Lock to free; must not currently be held by any thread.
 * @return true on success; false (error set) if the lock is still held.
 */
CNET_API bool lockFree(Lock* lock);

/**
 * Block indefinitely until the lock is acquired. Reentrant: the same
 * thread may call this repeatedly, incrementing a hold count.
 *
 * @param lock  Lock to acquire.
 */
CNET_API void lockAcquire(Lock* lock);
/**
 * Release one level of the calling thread's hold on the lock.
 *
 * @param lock  Lock to release; must currently be held by this thread.
 * @return true on success; false (error set) if the current thread does
 *         not hold the lock.
 */
CNET_API bool lockRelease(Lock* lock);
/**
 * Attempt to acquire the lock without blocking.
 *
 * @param lock  Lock to try to acquire.
 * @return 1 if acquired; 0 if it is currently held by another thread.
 */
CNET_API int lockTryAcquire(Lock* lock);

/**
 * Check whether the calling thread currently holds the lock.
 *
 * @param lock  Lock to check.
 * @return 1 if the current thread holds the lock (one or more times);
 *         0 otherwise.
 */
CNET_API int lockIsHeldByCurrentThread(const Lock* lock);
/**
 * Get how many times the calling thread has reentrantly acquired the lock.
 *
 * @param lock  Lock to check.
 * @return The current thread's hold count, or 0 if it does not hold the
 *         lock.
 */
CNET_API int lockGetHoldCount(const Lock* lock);

/**
 * Atomically release the lock and block the calling thread until signalled,
 * then reacquire the lock before returning. Waits indefinitely. Requires
 * the lock to be held exactly once (no nested reentrant holds).
 *
 * @param lock  Lock to wait on; must be held exactly once by this thread.
 * @return true on success; false (error set) if the lock is not held
 *         exactly once by the current thread.
 */
CNET_API bool lockAwait(Lock* lock);
/**
 * Wake one thread waiting in lockAwait on this lock. Caller must hold the
 * lock.
 *
 * @param lock  Lock whose waiters should be signalled.
 * @return true on success; false (error set) if the current thread does
 *         not hold the lock.
 */
CNET_API bool lockSignal(Lock* lock);
/**
 * Wake all threads waiting in lockAwait on this lock. Caller must hold the
 * lock.
 *
 * @param lock  Lock whose waiters should be signalled.
 * @return true on success; false (error set) if the current thread does
 *         not hold the lock.
 */
CNET_API bool lockSignalAll(Lock* lock);

#endif
