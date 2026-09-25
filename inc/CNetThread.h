#pragma once

#ifndef CNET_THREAD_H
#define CNET_THREAD_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef void (*ThreadRunFn)(void* arg);

typedef enum eThreadState {
    THREAD_STATE_NEW,
    THREAD_STATE_STARTED
} ThreadState;

typedef struct sThread Thread;

/**
 * Create a new thread object in the NEW state; does not start executing yet.
 *
 * @param run  Function the thread will run once started; borrowed, called
 *             on the new OS thread.
 * @param arg  Value passed to run on the new thread; caller keeps ownership
 *             and must keep it valid until run returns.
 * @return New Thread*, owned by the caller; free with threadFree once the
 *         thread is no longer alive.
 */
CNET_API Thread* threadCreate(ThreadRunFn run, void* arg);
/**
 * Free a thread object's resources.
 *
 * @param thread  Thread to free; must not still be alive (running).
 * @return true on success; false (error set) if the thread is still alive.
 */
CNET_API bool threadFree(Thread* thread);

/**
 * Start executing a thread that was created but not yet started.
 *
 * @param thread  Thread to start; must be in the NEW state.
 * @return true on success; false (error set) if already started.
 */
CNET_API bool threadStart(Thread* thread);
/**
 * Block the calling thread until the given thread finishes running. Waits
 * indefinitely (no timeout).
 *
 * @param thread  Thread to wait on; must have already been started.
 * @return true on success; false (error set) if the thread was never
 *         started.
 */
CNET_API bool threadJoin(Thread* thread);
/**
 * Check whether a thread is still running.
 *
 * @param thread  Thread to check.
 * @return 1 if the thread was started and has not yet finished; 0 if it was
 *         never started or has already finished.
 */
CNET_API int threadIsAlive(const Thread* thread);
/**
 * Get the native Win32 thread ID assigned when the thread was started. On
 * Linux/macOS, pthread_t has no portable notion of a small nonzero integer
 * ID, so this returns a process-local, atomically-assigned counter value
 * instead of a real OS thread ID — still unique per thread and still 0
 * before the thread is started.
 *
 * @param thread  Thread to query.
 * @return The OS thread ID, or 0 if the thread has never been started.
 */
CNET_API unsigned long threadGetId(const Thread* thread);

/**
 * Suspend the calling thread for the given duration.
 *
 * @param milliseconds  How long to sleep, in milliseconds.
 */
CNET_API void threadSleep(unsigned long milliseconds);

#endif
