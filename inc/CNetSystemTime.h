#pragma once

#ifndef CNET_SYSTEM_TIME_H
#define CNET_SYSTEM_TIME_H

#include "CNetApi.h"

/**
 * Get the current wall-clock time as a Unix-style epoch timestamp.
 *
 * @return Milliseconds since 1970-01-01T00:00:00Z (UTC), from
 *         GetSystemTimeAsFileTime(); subject to the OS clock being
 *         changed/synced, so not guaranteed monotonic across calls.
 */
CNET_API long long systemTimeMillis(void);

/**
 * Get a high-resolution, monotonic timestamp for measuring elapsed time.
 *
 * @return Nanoseconds from an arbitrary starting point (backed by
 *         QueryPerformanceCounter), not tied to wall-clock time. Only
 *         differences between two calls are meaningful.
 */
CNET_API long long systemTimeNanos(void);

#endif
