#pragma once

#ifndef CNET_LOG_H
#define CNET_LOG_H

#include "CNetApi.h"

typedef enum {
    LOG_LEVEL_DEBUG = 0,  /* Verbose diagnostic detail. */
    LOG_LEVEL_INFO = 1,   /* Normal informational messages. */
    LOG_LEVEL_WARN = 2,   /* Warnings about recoverable issues. */
    LOG_LEVEL_ERROR = 3,  /* Errors. */
    LOG_LEVEL_OFF = 4     /* Disables all logging when set as a logger's level. */
} LogLevel;

typedef struct sLogger Logger;

/**
 * Create a logger that writes to stdout. Default level is LOG_LEVEL_INFO.
 *
 * @param name Logger name, included in each log line; borrowed, copied internally.
 * @return New Logger the caller must release with loggerFree().
 */
CNET_API Logger* loggerCreate(const wchar_t* name);
/**
 * Create a logger that appends to a file, opened in text-append mode
 * (created if it does not exist). Default level is LOG_LEVEL_INFO. Log
 * lines are written to the file as UTF-8 regardless of the current
 * console/locale codepage.
 *
 * @param name Logger name, included in each log line; borrowed, copied internally.
 * @param path Path of the log file to append to; borrowed.
 * @return New Logger the caller must release with loggerFree() (which
 *         also closes the file), or NULL if the file could not be opened.
 */
CNET_API Logger* loggerCreateWithFile(const wchar_t* name, const wchar_t* path);
/**
 * Create a logger that writes to a caller-supplied, already-open stream.
 * The stream is not closed by loggerFree(); the caller retains ownership.
 * Default level is LOG_LEVEL_INFO.
 *
 * @param name   Logger name, included in each log line; borrowed, copied internally.
 * @param stream Open FILE* to write log lines to; borrowed, must outlive the logger.
 * @return New Logger the caller must release with loggerFree(), or NULL if stream is NULL.
 */
CNET_API Logger* loggerCreateWithStream(const wchar_t* name, FILE* stream);
/**
 * Release a logger. Closes the underlying file if it was opened via
 * loggerCreateWithFile(); a stream passed to loggerCreateWithStream() is
 * left open for the caller to manage.
 *
 * @param logger Logger to free; ownership is taken, do not use it afterward.
 */
CNET_API void loggerFree(Logger* logger);

/**
 * Get the logger's name.
 *
 * @param logger Logger to query; borrowed.
 * @return Name owned by logger; valid until loggerFree().
 */
CNET_API const wchar_t* loggerGetName(const Logger* logger);
/**
 * Set the minimum severity level the logger will write; messages below
 * this level are silently discarded. Use LOG_LEVEL_OFF to disable all logging.
 *
 * @param logger Logger to modify; borrowed.
 * @param level  New minimum level.
 */
CNET_API void loggerSetLevel(Logger* logger, LogLevel level);
/**
 * Get the logger's current minimum severity level.
 *
 * @param logger Logger to query; borrowed.
 * @return Current minimum level.
 */
CNET_API LogLevel loggerGetLevel(const Logger* logger);
/**
 * Check whether a given severity level would currently be written by this logger.
 *
 * @param logger Logger to query; borrowed.
 * @param level  Level to check.
 * @return 1 if level is at or above the logger's current level, 0 otherwise.
 */
CNET_API int loggerIsLevelEnabled(const Logger* logger, LogLevel level);

/**
 * Write a log line at an explicit severity level, if that level is
 * enabled, using a wprintf-style format string.
 *
 * @param logger Logger to write to; borrowed.
 * @param level  Severity level of this message.
 * @param format wprintf-style format string; borrowed.
 * @param ...    Arguments matching the format string's conversion specifiers.
 */
CNET_API void loggerLog(Logger* logger, LogLevel level, const wchar_t* format, ...);
/**
 * Write a log line at LOG_LEVEL_DEBUG, if enabled, using a wprintf-style format string.
 *
 * @param logger Logger to write to; borrowed.
 * @param format wprintf-style format string; borrowed.
 * @param ...    Arguments matching the format string's conversion specifiers.
 */
CNET_API void loggerDebug(Logger* logger, const wchar_t* format, ...);
/**
 * Write a log line at LOG_LEVEL_INFO, if enabled, using a wprintf-style format string.
 *
 * @param logger Logger to write to; borrowed.
 * @param format wprintf-style format string; borrowed.
 * @param ...    Arguments matching the format string's conversion specifiers.
 */
CNET_API void loggerInfo(Logger* logger, const wchar_t* format, ...);
/**
 * Write a log line at LOG_LEVEL_WARN, if enabled, using a wprintf-style format string.
 *
 * @param logger Logger to write to; borrowed.
 * @param format wprintf-style format string; borrowed.
 * @param ...    Arguments matching the format string's conversion specifiers.
 */
CNET_API void loggerWarn(Logger* logger, const wchar_t* format, ...);
/**
 * Write a log line at LOG_LEVEL_ERROR, if enabled, using a wprintf-style format string.
 *
 * @param logger Logger to write to; borrowed.
 * @param format wprintf-style format string; borrowed.
 * @param ...    Arguments matching the format string's conversion specifiers.
 */
CNET_API void loggerError(Logger* logger, const wchar_t* format, ...);

/**
 * Get the display name for a severity level (e.g. "INFO", "ERROR").
 *
 * @param level Level to name.
 * @return Static, non-owned string; "OFF" is also returned for any
 *         unrecognized value.
 */
CNET_API const wchar_t* logLevelName(LogLevel level);

#endif
