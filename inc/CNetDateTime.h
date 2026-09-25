#pragma once

#ifndef CNET_DATE_TIME_H
#define CNET_DATE_TIME_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sDateTime DateTime;

/**
 * Create a DateTime from calendar fields, validated as a real Gregorian
 * calendar date/time (via Win32 SystemTimeToFileTime).
 *
 * @param gc           Arena the returned DateTime is allocated from.
 * @param year         Calendar year (roughly 1601-30827, the FILETIME range).
 * @param month        Month, 1-12.
 * @param day          Day of month, 1-31 depending on month/year.
 * @param hour         Hour, 0-23.
 * @param minute       Minute, 0-59.
 * @param second       Second, 0-59.
 * @param millisecond  Millisecond, 0-999.
 * @return New DateTime owned by gc, or NULL if the fields do not form a
 *         valid date/time (check getLastError()/getLastErrorMessage()).
 */
CNET_API DateTime* dateTimeCreate(Gc* gc, int year, int month, int day, int hour, int minute, int second, int millisecond);

/**
 * Get the current date/time.
 *
 * @param gc  Arena the returned DateTime is allocated from.
 * @return New DateTime owned by gc, set to the current system time.
 */
CNET_API DateTime* dateTimeNow(Gc* gc);

/**
 * Create a DateTime from a Unix-style epoch timestamp.
 *
 * @param gc            Arena the returned DateTime is allocated from.
 * @param epochMillis   Milliseconds since 1970-01-01T00:00:00Z (UTC).
 * @return New DateTime owned by gc, representing that instant.
 */
CNET_API DateTime* dateTimeFromEpochMillis(Gc* gc, long long epochMillis);

/**
 * Get the calendar year component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Calendar year (e.g. 2024).
 */
CNET_API int dateTimeGetYear(const DateTime* dt);

/**
 * Get the calendar month component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Month, 1-12.
 */
CNET_API int dateTimeGetMonth(const DateTime* dt);

/**
 * Get the calendar day-of-month component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Day of month, 1-31.
 */
CNET_API int dateTimeGetDay(const DateTime* dt);

/**
 * Get the hour component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Hour, 0-23.
 */
CNET_API int dateTimeGetHour(const DateTime* dt);

/**
 * Get the minute component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Minute, 0-59.
 */
CNET_API int dateTimeGetMinute(const DateTime* dt);

/**
 * Get the second component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Second, 0-59.
 */
CNET_API int dateTimeGetSecond(const DateTime* dt);

/**
 * Get the millisecond component.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Millisecond, 0-999.
 */
CNET_API int dateTimeGetMillisecond(const DateTime* dt);

/**
 * Get the day of the week.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Day of week, 0 (Sunday) through 6 (Saturday).
 */
CNET_API int dateTimeGetDayOfWeek(const DateTime* dt);

/**
 * Convert to a Unix-style epoch timestamp.
 *
 * @param dt  DateTime to read; borrowed, not modified.
 * @return Milliseconds since 1970-01-01T00:00:00Z (UTC).
 */
CNET_API long long dateTimeToEpochMillis(const DateTime* dt);

/**
 * Add (or subtract, if negative) whole days.
 *
 * @param dt    Base DateTime; borrowed, not modified.
 * @param days  Number of days to add; negative moves backward.
 * @return New DateTime owned by dt's arena.
 */
CNET_API DateTime* dateTimePlusDays(const DateTime* dt, long long days);

/**
 * Add (or subtract, if negative) whole hours.
 *
 * @param dt     Base DateTime; borrowed, not modified.
 * @param hours  Number of hours to add; negative moves backward.
 * @return New DateTime owned by dt's arena.
 */
CNET_API DateTime* dateTimePlusHours(const DateTime* dt, long long hours);

/**
 * Add (or subtract, if negative) whole minutes.
 *
 * @param dt       Base DateTime; borrowed, not modified.
 * @param minutes  Number of minutes to add; negative moves backward.
 * @return New DateTime owned by dt's arena.
 */
CNET_API DateTime* dateTimePlusMinutes(const DateTime* dt, long long minutes);

/**
 * Add (or subtract, if negative) whole seconds.
 *
 * @param dt       Base DateTime; borrowed, not modified.
 * @param seconds  Number of seconds to add; negative moves backward.
 * @return New DateTime owned by dt's arena.
 */
CNET_API DateTime* dateTimePlusSeconds(const DateTime* dt, long long seconds);

/**
 * Add (or subtract, if negative) milliseconds.
 *
 * @param dt      Base DateTime; borrowed, not modified.
 * @param millis  Number of milliseconds to add; negative moves backward.
 * @return New DateTime owned by dt's arena.
 */
CNET_API DateTime* dateTimePlusMillis(const DateTime* dt, long long millis);

/**
 * Compare two date/times chronologically.
 *
 * @param a  First DateTime; borrowed.
 * @param b  Second DateTime; borrowed.
 * @return -1 if a is before b, 1 if a is after b, 0 if equal.
 */
CNET_API int dateTimeCompareTo(const DateTime* a, const DateTime* b);

/**
 * Check whether one date/time is strictly earlier than another.
 *
 * @param a  First DateTime; borrowed.
 * @param b  Second DateTime; borrowed.
 * @return Non-zero if a is before b, 0 otherwise.
 */
CNET_API int dateTimeIsBefore(const DateTime* a, const DateTime* b);

/**
 * Check whether one date/time is strictly later than another.
 *
 * @param a  First DateTime; borrowed.
 * @param b  Second DateTime; borrowed.
 * @return Non-zero if a is after b, 0 otherwise.
 */
CNET_API int dateTimeIsAfter(const DateTime* a, const DateTime* b);

/**
 * Check whether two date/times represent the same instant.
 *
 * @param a  First DateTime; borrowed.
 * @param b  Second DateTime; borrowed.
 * @return Non-zero if a and b are equal, 0 otherwise.
 */
CNET_API int dateTimeEquals(const DateTime* a, const DateTime* b);

/**
 * Format as "YYYY-MM-DD HH:MM:SS.mmm".
 *
 * @param gc  Arena the returned string is allocated from.
 * @param dt  DateTime to format; borrowed, not modified.
 * @return New NUL-terminated wide string owned by gc.
 */
CNET_API wchar_t* dateTimeToString(Gc* gc, const DateTime* dt);

/**
 * Parse a "YYYY-MM-DD HH:MM:SS.mmm" string, as produced by dateTimeToString().
 *
 * @param gc    Arena the returned DateTime is allocated from.
 * @param text  NUL-terminated wide string to parse; borrowed, not modified.
 * @return New DateTime owned by gc, or NULL if text is malformed or the
 *         parsed fields do not form a valid date/time.
 */
CNET_API DateTime* dateTimeParse(Gc* gc, const wchar_t* text);

#endif
