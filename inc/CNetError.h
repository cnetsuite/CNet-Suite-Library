#pragma once

#ifndef CNET_ERROR_H
#define CNET_ERROR_H

#include "CNetApi.h"

typedef enum {
    ErrorNone = 0,             /**< No error; the last checked condition succeeded. */
    ErrorInvalidArgument = -1, /**< A function argument was invalid (e.g. out of domain, negative size). */
    ErrorInvalidState = -2,    /**< The object/operation was not in a state that allows this call. */
    ErrorOutOfRange = -3,      /**< An index or value fell outside its valid bounds. */
    ErrorOperationFailed = -4, /**< An underlying operation (I/O, crypto, syscall, ...) failed. */
    ErrorUnsupported = -5,     /**< The requested operation or feature is not supported. */
    ErrorNotFound = -6         /**< A requested item (key, file, entry, ...) was not found. */
} ErrorCode;

/**
 * Record the outcome of a checked condition into the thread-local last
 * error, clearing it on success. Intended to be used as
 * `if (!setError(cond, code, message)) { ...handle failure... }`.
 *
 * @param cond     Success condition; non-zero means "no error occurred".
 * @param code     Error code to record when cond is zero; ignored otherwise.
 * @param message  Human-readable wide-text message to record when cond is
 *                 zero; borrowed, copied into an internal buffer (truncated
 *                 to its capacity); ignored when cond is non-zero.
 * @return 1 if cond was non-zero (success, error state cleared), 0 if
 *         cond was zero (failure, error state recorded).
 */
CNET_API int setError(int cond, ErrorCode code, const wchar_t* message);

/**
 * Get the most recent error code recorded on the calling thread.
 *
 * @return The last ErrorCode set via setError(), or ErrorNone if the last
 *         recorded outcome was a success (or none has been recorded yet).
 */
CNET_API ErrorCode getLastError(void);

/**
 * Get the most recent error message recorded on the calling thread.
 *
 * @return Pointer to an internal thread-local wide-text buffer; owned by
 *         the library, valid until the next setError() call on this thread,
 *         do not free it. Empty string if there is no current error.
 */
CNET_API const wchar_t* getLastErrorMessage(void);

#endif
