#pragma once

#ifndef CNET_MUTEX_H
#define CNET_MUTEX_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sMutex Mutex;

/**
 * Create a new, unnamed Win32 mutex. On Linux/macOS, backed by a named
 * POSIX semaphore under a throwaway, process-and-pointer-unique name that
 * is unlinked immediately after opening (see Mutex.posix.c's header
 * comment) — there is no unnamed-kernel-object equivalent there.
 *
 * @return New Mutex*, owned by the caller, or NULL on failure (error set).
 */
CNET_API Mutex* mutexCreate(void);
/**
 * Create (or open a handle to) a system-wide named mutex. On Linux/macOS,
 * backed by a named POSIX semaphore (sem_open, binary-lock semantics)
 * instead of a Win32 kernel mutex object, since POSIX has no portable named
 * mutex — see Mutex.posix.c's header comment.
 *
 * @param name  Name of the mutex; borrowed, not retained after the
 *              call returns. Threads/processes using the same name contend
 *              for the same underlying mutex.
 * @return New Mutex*, owned by the caller, or NULL on failure (error set).
 */
CNET_API Mutex* mutexCreateNamed(const wchar_t* name);
/**
 * Free a mutex's resources.
 *
 * @param mutex  Mutex to free; must not currently be held.
 * @return true on success; false (error set) if the mutex is still held.
 */
CNET_API bool mutexFree(Mutex* mutex);

/**
 * Block indefinitely until the mutex is acquired.
 *
 * @param mutex  Mutex to acquire.
 * @return true on success; false (error set) on a native wait failure.
 */
CNET_API bool mutexAcquire(Mutex* mutex);
/**
 * Attempt to acquire the mutex without blocking.
 *
 * @param mutex  Mutex to try to acquire.
 * @return 1 if acquired; 0 if it is currently held elsewhere.
 */
CNET_API int mutexTryAcquire(Mutex* mutex);
/**
 * Attempt to acquire the mutex, waiting up to a bounded time.
 *
 * @param mutex          Mutex to acquire.
 * @param timeoutMillis  Maximum time to wait, in milliseconds; 0 polls
 *                        once without blocking.
 * @return 1 if acquired within the timeout; 0 if the wait timed out.
 */
CNET_API int mutexAcquireTimeout(Mutex* mutex, unsigned long timeoutMillis);
/**
 * Release one level of the calling thread's hold on the mutex.
 *
 * @param mutex  Mutex to release; must currently be held by this thread.
 * @return true on success; false (error set) if the current thread does
 *         not hold the mutex.
 */
CNET_API bool mutexRelease(Mutex* mutex);

/**
 * Check whether the calling thread currently holds the mutex.
 *
 * @param mutex  Mutex to check.
 * @return 1 if the current thread holds the mutex; 0 otherwise.
 */
CNET_API int mutexIsHeldByCurrentThread(const Mutex* mutex);
/**
 * Get how many times the calling thread has acquired the mutex.
 *
 * @param mutex  Mutex to check.
 * @return The current thread's hold count, or 0 if it does not hold the
 *         mutex.
 */
CNET_API int mutexGetHoldCount(const Mutex* mutex);

#endif
