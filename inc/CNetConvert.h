#pragma once

#ifndef CNET_CONVERT_H
#define CNET_CONVERT_H

#include "CNetGc.h"
#include "CNetApi.h"

/**
 * Converts a NUL-terminated UTF-8 string to a NUL-terminated wide string,
 * for use with Win32 wide-character APIs — UTF-16 on Windows (via
 * MultiByteToWideChar), UTF-32 on Linux/macOS (via this module's own
 * convertUtf8ToUtf32(), since wchar_t is natively UTF-32 there
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param utf8 NUL-terminated UTF-8 text; borrowed, not stored.
 * @return Newly allocated wchar_t* allocated from gc. NULL if utf8 is not
 *         valid UTF-8 or the conversion fails.
 */
CNET_API wchar_t* convertUtf8ToWide(Gc* gc, const char* utf8);
/**
 * Converts a NUL-terminated wide string to NUL-terminated UTF-8 — UTF-16 on
 * Windows, UTF-32 on Linux/macOS (see convertUtf8ToWide() above).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param wide NUL-terminated wide string; borrowed, not stored.
 * @return Newly allocated char* allocated from gc. NULL if the conversion
 *         fails.
 */
CNET_API char* convertWideToUtf8(Gc* gc, const wchar_t* wide);
/**
 * Decodes a NUL-terminated UTF-8 string into an array of UTF-32 codepoints.
 * There is no native Win32 API for UTF-32, so this is a hand-written
 * multi-byte decoder.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param utf8 NUL-terminated UTF-8 text; borrowed, not stored.
 * @param outLength Receives the number of codepoints decoded (not counting
 *                   a terminator); out param, must not be NULL.
 * @return Codepoint array allocated from gc, NUL-codepoint-terminated for
 *         convenience; NULL (with *outLength set to 0) if utf8 is not valid
 *         UTF-8.
 */
CNET_API unsigned int* convertUtf8ToUtf32(Gc* gc, const char* utf8, int* outLength);
/**
 * Encodes an array of UTF-32 codepoints back to NUL-terminated UTF-8.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param codepoints Codepoints to encode; borrowed. May be NULL when
 *                    length is 0.
 * @param length Number of codepoints in codepoints.
 * @return NUL-terminated UTF-8 string allocated from gc; NULL if any
 *         codepoint is not a valid Unicode scalar value.
 */
CNET_API char* convertUtf32ToUtf8(Gc* gc, const unsigned int* codepoints, int length);

/**
 * Encodes a byte buffer as a lowercase hexadecimal wide string (two hex
 * digits per byte). Wide-string, Gc-allocated counterpart to CNetBase64.h's
 * base64Encode, for embedding binary data in JSON as hex instead.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param data Bytes to encode; borrowed. May be NULL when length is 0.
 * @param length Number of bytes in data.
 * @return NUL-terminated lowercase hex wide string allocated from gc.
 */
CNET_API wchar_t* convertBytesToHex(Gc* gc, const unsigned char* data, int length);
/**
 * Decodes a hexadecimal wide string (case-insensitive) back to bytes. Wide
 * counterpart to CNetBase64.h's base64Decode.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param text NUL-terminated hex text to decode; borrowed.
 * @param outLength Receives the number of bytes decoded; out param, must
 *                  not be NULL.
 * @return Byte buffer allocated from gc; NULL (with *outLength set to 0) if
 *         text has an odd number of characters or contains a character
 *         that is not a hex digit.
 */
CNET_API unsigned char* convertHexToBytes(Gc* gc, const wchar_t* text, int* outLength);

/**
 * Formats a byte as a decimal string. Raw-char* counterpart to numeric
 * formatting elsewhere in the library (e.g. CNetString.h's stringFromInt,
 * which works on the String object type instead).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal string allocated from gc.
 */
CNET_API char* convertByteToString(Gc* gc, unsigned char value);
/**
 * Parses a decimal string as a byte (0-255).
 *
 * @param text NUL-terminated text to parse; borrowed. Optional leading
 *             '+'/'-' sign, then one or more decimal digits.
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty, malformed, or out of range for
 *         unsigned char.
 */
CNET_API int convertStringToByte(const char* text, unsigned char* outValue);
/**
 * Formats an int as a decimal string. Raw-char* counterpart to
 * CNetString.h's stringFromInt (which returns a String*, not a char*).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal string allocated from gc.
 */
CNET_API char* convertIntToString(Gc* gc, int value);
/**
 * Parses a decimal string as an int. Raw-char* counterpart to
 * CNetString.h's stringToInt (which takes a String*, not a char*).
 *
 * @param text NUL-terminated text to parse; borrowed. Optional leading
 *             '+'/'-' sign, then one or more decimal digits.
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty, malformed, or out of range for
 *         int.
 */
CNET_API int convertStringToInt(const char* text, int* outValue);
/**
 * Formats a long long as a decimal string.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal string allocated from gc.
 */
CNET_API char* convertLongToString(Gc* gc, long long value);
/**
 * Parses a decimal string as a long long.
 *
 * @param text NUL-terminated text to parse; borrowed. Optional leading
 *             '+'/'-' sign, then one or more decimal digits.
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty, malformed, or out of range for
 *         long long.
 */
CNET_API int convertStringToLong(const char* text, long long* outValue);
/**
 * Formats a double as a decimal string (via the C library's default
 * "%g"-style precision).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal string allocated from gc.
 */
CNET_API char* convertDoubleToString(Gc* gc, double value);
/**
 * Parses a decimal string as a double.
 *
 * @param text NUL-terminated text to parse; borrowed. Any format accepted
 *             by the C library's strtod (optional sign, digits, decimal
 *             point, exponent).
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty or does not parse as a valid
 *         double.
 */
CNET_API int convertStringToDouble(const char* text, double* outValue);
/**
 * Formats a boolean as "true" or "false".
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format; treated as a boolean (0 = false, nonzero
 *              = true).
 * @return NUL-terminated "true" or "false" string allocated from gc.
 */
CNET_API char* convertBoolToString(Gc* gc, int value);
/**
 * Parses "true"/"false" (case-insensitive) as a boolean.
 *
 * @param text NUL-terminated text to parse; borrowed.
 * @param outValue Receives 0 or 1 on success; out param, must not be NULL.
 * @return 1 on success, 0 if text is neither "true" nor "false"
 *         (case-insensitive).
 */
CNET_API int convertStringToBool(const char* text, int* outValue);

/**
 * Formats a byte as a decimal wide string. Wide counterpart to
 * convertByteToString, for callers whose text is wchar_t*-based.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal wide string allocated from gc.
 */
CNET_API wchar_t* convertByteToWideString(Gc* gc, unsigned char value);
/**
 * Parses a decimal wide string as a byte (0-255).
 *
 * @param text NUL-terminated wide text to parse; borrowed. Optional
 *             leading '+'/'-' sign, then one or more decimal digits.
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty, malformed, or out of range for
 *         unsigned char.
 */
CNET_API int convertWideStringToByte(const wchar_t* text, unsigned char* outValue);
/**
 * Formats an int as a decimal wide string. Wide counterpart to
 * convertIntToString.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal wide string allocated from gc.
 */
CNET_API wchar_t* convertIntToWideString(Gc* gc, int value);
/**
 * Parses a decimal wide string as an int.
 *
 * @param text NUL-terminated wide text to parse; borrowed. Optional
 *             leading '+'/'-' sign, then one or more decimal digits.
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty, malformed, or out of range for
 *         int.
 */
CNET_API int convertWideStringToInt(const wchar_t* text, int* outValue);
/**
 * Formats a long long as a decimal wide string.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal wide string allocated from gc.
 */
CNET_API wchar_t* convertLongToWideString(Gc* gc, long long value);
/**
 * Parses a decimal wide string as a long long.
 *
 * @param text NUL-terminated wide text to parse; borrowed. Optional
 *             leading '+'/'-' sign, then one or more decimal digits.
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty, malformed, or out of range for
 *         long long.
 */
CNET_API int convertWideStringToLong(const wchar_t* text, long long* outValue);
/**
 * Formats a double as a decimal wide string (via the C library's default
 * "%g"-style precision).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format.
 * @return NUL-terminated decimal wide string allocated from gc.
 */
CNET_API wchar_t* convertDoubleToWideString(Gc* gc, double value);
/**
 * Parses a decimal wide string as a double.
 *
 * @param text NUL-terminated wide text to parse; borrowed. Any format
 *             accepted by the C library's wcstod (optional sign, digits,
 *             decimal point, exponent).
 * @param outValue Receives the parsed value on success; out param, must
 *                  not be NULL.
 * @return 1 on success, 0 if text is empty or does not parse as a valid
 *         double.
 */
CNET_API int convertWideStringToDouble(const wchar_t* text, double* outValue);
/**
 * Formats a boolean as L"true" or L"false".
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param value Value to format; treated as a boolean (0 = false, nonzero
 *              = true).
 * @return NUL-terminated L"true" or L"false" string allocated from gc.
 */
CNET_API wchar_t* convertBoolToWideString(Gc* gc, int value);
/**
 * Parses L"true"/L"false" (case-insensitive) as a boolean.
 *
 * @param text NUL-terminated wide text to parse; borrowed.
 * @param outValue Receives 0 or 1 on success; out param, must not be NULL.
 * @return 1 on success, 0 if text is neither "true" nor "false"
 *         (case-insensitive).
 */
CNET_API int convertWideStringToBool(const wchar_t* text, int* outValue);

/**
 * Correctly-typed wrapper for the C library's strlen. Kept as the
 * conventional way to call this function in samples/CNetTests code,
 * independent of any particular header's include path.
 *
 * @param text NUL-terminated narrow string; borrowed, must not be NULL.
 * @return Number of characters before the terminating NUL.
 */
CNET_API size_t convertStrLen(const char* text);
/**
 * Correctly-typed wrapper for the C library's strcmp.
 *
 * @param a NUL-terminated narrow string; borrowed, must not be NULL.
 * @param b NUL-terminated narrow string; borrowed, must not be NULL.
 * @return <0, 0, or >0 as a compares less than, equal to, or greater than b.
 */
CNET_API int convertStrCmp(const char* a, const char* b);
/**
 * Correctly-typed wrapper for the C library's strcpy.
 *
 * @param dest Destination buffer; borrowed, must be large enough for src
 *             plus its NUL terminator.
 * @param src NUL-terminated narrow string to copy; borrowed.
 * @return dest.
 */
CNET_API char* convertStrCpy(char* dest, const char* src);
/**
 * Correctly-typed wrapper for the C library's strcat.
 *
 * @param dest NUL-terminated destination buffer; borrowed, must be large
 *             enough for its own contents plus src plus a NUL terminator.
 * @param src NUL-terminated narrow string to append; borrowed.
 * @return dest.
 */
CNET_API char* convertStrCat(char* dest, const char* src);
/**
 * Correctly-typed wrapper for the C library's strstr.
 *
 * @param haystack NUL-terminated narrow string to search; borrowed.
 * @param needle NUL-terminated narrow string to find; borrowed.
 * @return Pointer into haystack at the first occurrence of needle, or NULL
 *         if needle does not occur in haystack.
 */
CNET_API char* convertStrStr(const char* haystack, const char* needle);
/**
 * Correctly-typed wrapper for the C library's memcpy. The source and
 * destination ranges must not overlap; use convertMemMove otherwise.
 *
 * @param dest Destination buffer; borrowed, must be at least count bytes.
 * @param src Source buffer; borrowed, must be at least count bytes.
 * @param count Number of bytes to copy.
 * @return dest.
 */
CNET_API void* convertMemCpy(void* dest, const void* src, size_t count);
/**
 * Correctly-typed wrapper for the C library's memmove. Unlike
 * convertMemCpy, the source and destination ranges may overlap.
 *
 * @param dest Destination buffer; borrowed, must be at least count bytes.
 * @param src Source buffer; borrowed, must be at least count bytes.
 * @param count Number of bytes to copy.
 * @return dest.
 */
CNET_API void* convertMemMove(void* dest, const void* src, size_t count);
/**
 * Correctly-typed wrapper for the C library's memset.
 *
 * @param dest Buffer to fill; borrowed, must be at least count bytes.
 * @param value Byte value to write, truncated to unsigned char.
 * @param count Number of bytes to write.
 * @return dest.
 */
CNET_API void* convertMemSet(void* dest, int value, size_t count);
/**
 * Correctly-typed wrapper for the C library's memcmp.
 *
 * @param a First buffer; borrowed, must be at least count bytes.
 * @param b Second buffer; borrowed, must be at least count bytes.
 * @param count Number of bytes to compare.
 * @return <0, 0, or >0 as the first count bytes of a compare less than,
 *         equal to, or greater than those of b.
 */
CNET_API int convertMemCmp(const void* a, const void* b, size_t count);

#endif
