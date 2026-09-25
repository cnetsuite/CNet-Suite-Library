#pragma once

#ifndef CNET_SEMAPHORE_H
#define CNET_SEMAPHORE_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sSemaphore Semaphore;

/**
 * Create a counting semaphore.
 *
 * @param initialCount  Starting number of available permits; must be
 *                       between 0 and maxCount.
 * @param maxCount      Maximum number of permits the semaphore can hold.
 * @return New Semaphore*, owned by the caller, or NULL if initialCount is
 *         out of range or creation failed (error set).
 */
CNET_API Semaphore* semaphoreCreate(int initialCount, int maxCount);
/**
 * Free a semaphore's resources.
 *
 * @param semaphore  Semaphore to free.
 */
CNET_API void semaphoreFree(Semaphore* semaphore);

/**
 * Block indefinitely until a permit is available, then take it.
 *
 * @param semaphore  Semaphore to acquire a permit from.
 * @return true on success; false (error set) on a native wait failure.
 */
CNET_API bool semaphoreAcquire(Semaphore* semaphore);
/**
 * Attempt to take a permit without blocking.
 *
 * @param semaphore  Semaphore to try to acquire.
 * @return 1 if a permit was taken; 0 if none were available.
 */
CNET_API int semaphoreTryAcquire(Semaphore* semaphore);
/**
 * Attempt to take a permit, waiting up to a bounded time.
 *
 * @param semaphore      Semaphore to acquire a permit from.
 * @param timeoutMillis  Maximum time to wait, in milliseconds; 0 polls
 *                        once without blocking.
 * @return 1 if a permit was taken within the timeout; 0 if the wait timed
 *         out.
 */
CNET_API int semaphoreAcquireTimeout(Semaphore* semaphore, unsigned long timeoutMillis);
/**
 * Return a single permit to the pool. Equivalent to semaphoreReleaseMany
 * with count 1.
 *
 * @param semaphore  Semaphore to release a permit back to.
 * @return true on success; false (error set) if releasing would exceed
 *         maxCount.
 */
CNET_API bool semaphoreRelease(Semaphore* semaphore);
/**
 * Return multiple permits to the pool at once.
 *
 * @param semaphore  Semaphore to release permits back to.
 * @param count      Number of permits to release; must be positive.
 * @return true on success; false (error set) if count is not positive or
 *         releasing would exceed maxCount.
 */
CNET_API bool semaphoreReleaseMany(Semaphore* semaphore, int count);

/**
 * Get a snapshot of how many permits are currently available.
 *
 * @param semaphore  Semaphore to query.
 * @return Approximate current available permit count (may be stale under
 *         concurrent access).
 */
CNET_API int semaphoreGetAvailableCount(const Semaphore* semaphore);
/**
 * Get the maximum permit count the semaphore was created with.
 *
 * @param semaphore  Semaphore to query.
 * @return The maxCount passed to semaphoreCreate.
 */
CNET_API int semaphoreGetMaxCount(const Semaphore* semaphore);

#endif
