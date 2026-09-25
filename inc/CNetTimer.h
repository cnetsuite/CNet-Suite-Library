#pragma once

#ifndef CNET_TIMER_H
#define CNET_TIMER_H

#include "CNetApi.h"

typedef void (*TimerTaskFn)(void* arg);

typedef struct sTimer Timer;
typedef struct sTimerTask TimerTask;

/**
 * Create a new timer, starting its background worker thread that services
 * scheduled tasks.
 *
 * @return New Timer*, owned by the caller; free with timerFree after
 *         calling timerCancel.
 */
CNET_API Timer* timerCreate(void);
/**
 * Free a timer's resources.
 *
 * @param timer  Timer to free; must already have been cancelled.
 * @return true on success; false (error set) if the timer has not been
 *         cancelled.
 */
CNET_API bool timerFree(Timer* timer);

/* TimerTask nodes have no individual free function - they are owned by
   the Timer that created them and are released when timerFree runs. */
/**
 * Schedule a one-shot task to run once after a delay.
 *
 * @param timer        Timer to schedule the task on.
 * @param run          Callback invoked on the timer's worker thread;
 *                      borrowed.
 * @param arg          Value passed to run; caller keeps ownership.
 * @param delayMillis  Delay in milliseconds before the task's single run.
 * @return New TimerTask*, owned by the Timer (do not free individually),
 *         or NULL if the timer has already been cancelled.
 */
CNET_API TimerTask* timerSchedule(Timer* timer, TimerTaskFn run, void* arg, long long delayMillis);
/**
 * Schedule a task that runs repeatedly at a fixed rate.
 *
 * @param timer         Timer to schedule the task on.
 * @param run           Callback invoked on the timer's worker thread on
 *                       each firing; borrowed.
 * @param arg           Value passed to run on each firing; caller keeps
 *                       ownership.
 * @param delayMillis   Delay in milliseconds before the first run.
 * @param periodMillis  Interval in milliseconds between subsequent runs.
 * @return New TimerTask*, owned by the Timer (do not free individually),
 *         or NULL if the timer has already been cancelled.
 */
CNET_API TimerTask* timerScheduleAtFixedRate(Timer* timer, TimerTaskFn run, void* arg, long long delayMillis, long long periodMillis);

/**
 * Cancel a scheduled task so it will not fire again; safe to call even
 * after a one-shot task has already fired.
 *
 * @param task  Task to cancel.
 */
CNET_API void timerTaskCancel(TimerTask* task);
/**
 * Check whether a task has been cancelled (explicitly, or automatically
 * after a one-shot task fires).
 *
 * @param task  Task to check.
 * @return 1 if cancelled; 0 otherwise.
 */
CNET_API int timerTaskIsCancelled(const TimerTask* task);

/**
 * Stop the timer's background worker thread, preventing any further tasks
 * from firing. Idempotent; blocks until the worker thread exits.
 *
 * @param timer  Timer to cancel.
 */
CNET_API void timerCancel(Timer* timer);

#endif
