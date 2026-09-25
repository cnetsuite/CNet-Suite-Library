#pragma once

#ifndef CNET_EVENT_H
#define CNET_EVENT_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef enum eEventResetMode {
    EVENT_RESET_MODE_AUTO,   /* signal clears automatically after exactly one waiter is released */
    EVENT_RESET_MODE_MANUAL  /* signal stays set until eventReset is called explicitly */
} EventResetMode;

typedef struct sEvent Event;

/**
 * Create a new event, initially unsignalled.
 *
 * @param resetMode  EVENT_RESET_MODE_AUTO to clear the signal after
 *                    releasing one waiter, or EVENT_RESET_MODE_MANUAL to
 *                    require an explicit eventReset.
 * @return New Event*, owned by the caller, or NULL on failure (error set).
 */
CNET_API Event* eventCreate(EventResetMode resetMode);
/**
 * Free an event's resources.
 *
 * @param event  Event to free.
 */
CNET_API void eventFree(Event* event);

/**
 * Set the event to the signalled state, releasing waiting thread(s) (one
 * for auto-reset, all for manual-reset until it is reset).
 *
 * @param event  Event to signal.
 * @return true on success; false (error set) on a native OS failure.
 */
CNET_API bool eventSignal(Event* event);
/**
 * Clear the signalled state. Mainly meaningful for manual-reset events.
 *
 * @param event  Event to reset.
 * @return true on success; false (error set) on a native OS failure.
 */
CNET_API bool eventReset(Event* event);
/**
 * Block indefinitely until the event is signalled. For auto-reset events,
 * clears the signal before returning.
 *
 * @param event  Event to wait on.
 * @return true on success; false (error set) on a native wait failure.
 */
CNET_API bool eventWait(Event* event);
/**
 * Wait for the event to be signalled, up to a bounded time. For
 * auto-reset events, clears the signal before returning on success.
 *
 * @param event          Event to wait on.
 * @param timeoutMillis  Maximum time to wait, in milliseconds; 0 polls
 *                        once without blocking.
 * @return 1 if signalled within the timeout; 0 if the wait timed out.
 */
CNET_API int eventWaitTimeout(Event* event, unsigned long timeoutMillis);
/**
 * Get a snapshot of whether the event is currently signalled.
 *
 * @param event  Event to check.
 * @return 1 if signalled; 0 if not (may race with concurrent signal/reset).
 */
CNET_API int eventIsSignalled(const Event* event);

#endif
