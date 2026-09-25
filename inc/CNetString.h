#pragma once

#ifndef CNET_STRING_H
#define CNET_STRING_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sString String;

/**
 * Creates a new String by copying a NUL-terminated UTF-16 (wide) C string.
 *
 * @param wstr NUL-terminated wide text; borrowed, not stored.
 * @return New owned String* with its own Gc; free with stringFree().
 */
CNET_API String* stringCreate(const wchar_t* wstr);
/**
 * Creates a new String by copying an explicit-length UTF-16 buffer (need
 * not be NUL-terminated).
 *
 * @param buffer Pointer to length code units of wide text; borrowed, not
 *               stored. May be NULL when length is 0.
 * @param length Number of code units in buffer to copy; 0 for an empty
 *               string.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringCreateFromChars(const wchar_t* buffer, int length);
/**
 * Frees a String previously returned by this module, releasing its
 * underlying Gc.
 *
 * @param str String to free; ownership is taken, must not be used afterward.
 */
CNET_API void stringFree(String* str);

/**
 * Returns the length of a String in UTF-16 code units (surrogate pairs for
 * supplementary-plane characters count as 2), not Unicode codepoints.
 *
 * @param str String to query; borrowed.
 * @return Number of UTF-16 code units.
 */
CNET_API int stringLength(const String* str);
/**
 * Returns the UTF-16 code unit at a given index (may be one half of a
 * surrogate pair for supplementary-plane characters).
 *
 * @param str String to query; borrowed.
 * @param index 0-based code-unit index; must satisfy 0 <= index < stringLength(str).
 * @return The code unit at index, or L'\0' if index is out of range.
 */
CNET_API wchar_t stringCharAt(const String* str, int index);
/**
 * Returns a borrowed pointer to str's underlying NUL-terminated UTF-16
 * buffer.
 *
 * @param str String to query; borrowed.
 * @return Wide text valid only until str is freed with stringFree() - do
 *         not free this pointer separately.
 */
CNET_API const wchar_t* stringGetChars(const String* str);

/**
 * Returns a new String containing the UTF-16 code units of str in the
 * range [beginIndex, endIndex).
 *
 * @param str Source String; borrowed.
 * @param beginIndex Inclusive start code-unit index; 0 <= beginIndex <= endIndex.
 * @param endIndex Exclusive end code-unit index; endIndex <= stringLength(str).
 * @return New owned String*; free with stringFree(). NULL if the range is
 *         out of bounds.
 */
CNET_API String* stringSubstring(const String* str, int beginIndex, int endIndex);
/**
 * Concatenates two strings.
 *
 * @param str First String; borrowed.
 * @param other String appended after str; borrowed.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringConcat(const String* str, const String* other);

/**
 * Tests two strings for exact (case-sensitive) equality of UTF-16 content.
 *
 * @param str First String; borrowed.
 * @param other Second String; borrowed.
 * @return 1 if equal, 0 otherwise.
 */
CNET_API int stringEquals(const String* str, const String* other);
/**
 * Tests two strings for case-insensitive equality (per-code-unit case
 * folding).
 *
 * @param str First String; borrowed.
 * @param other Second String; borrowed.
 * @return 1 if equal ignoring case, 0 otherwise.
 */
CNET_API int stringEqualsIgnoreCase(const String* str, const String* other);
/**
 * Lexicographically compares two strings by UTF-16 code unit.
 *
 * @param str First String; borrowed.
 * @param other Second String; borrowed.
 * @return Negative if str < other, 0 if equal, positive if str > other.
 */
CNET_API int stringCompareTo(const String* str, const String* other);

/**
 * Finds the first occurrence of a UTF-16 code unit.
 *
 * @param str String to search; borrowed.
 * @param ch Code unit to search for.
 * @return Index of the first occurrence, or -1 if not found.
 */
CNET_API int stringIndexOf(const String* str, wchar_t ch);
/**
 * Finds the first occurrence of needle as a substring.
 *
 * @param str String to search; borrowed.
 * @param needle Substring to search for; borrowed. An empty needle matches
 *               at index 0.
 * @return Index of the first occurrence, or -1 if not found.
 */
CNET_API int stringIndexOfString(const String* str, const String* needle);
/**
 * Tests whether needle occurs anywhere within str.
 *
 * @param str String to search; borrowed.
 * @param needle Substring to look for; borrowed.
 * @return 1 if found, 0 otherwise.
 */
CNET_API int stringContains(const String* str, const String* needle);
/**
 * Tests whether str begins with prefix.
 *
 * @param str String to test; borrowed.
 * @param prefix Prefix to check for; borrowed.
 * @return 1 if str starts with prefix, 0 otherwise.
 */
CNET_API int stringStartsWith(const String* str, const String* prefix);
/**
 * Tests whether str ends with suffix.
 *
 * @param str String to test; borrowed.
 * @param suffix Suffix to check for; borrowed.
 * @return 1 if str ends with suffix, 0 otherwise.
 */
CNET_API int stringEndsWith(const String* str, const String* suffix);

/**
 * Converts to uppercase, per UTF-16 code unit (Win32 CharUpperBuffW).
 *
 * @param str String to convert; borrowed.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringToUpperCase(const String* str);
/**
 * Converts to lowercase, per UTF-16 code unit (Win32 CharLowerBuffW).
 *
 * @param str String to convert; borrowed.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringToLowerCase(const String* str);
/**
 * Removes leading and trailing whitespace code units (per iswspace()).
 *
 * @param str String to trim; borrowed.
 * @return New owned String* with whitespace stripped; free with
 *         stringFree().
 */
CNET_API String* stringTrim(const String* str);
/**
 * Replaces every occurrence of one UTF-16 code unit with another.
 *
 * @param str String to transform; borrowed.
 * @param oldChar Code unit to replace.
 * @param newChar Code unit to substitute in its place.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringReplace(const String* str, wchar_t oldChar, wchar_t newChar);

/**
 * Splits str into pieces separated by delimiter. Unlike a regex split,
 * trailing empty pieces are kept (e.g. "a,b," with ',' yields 3 pieces,
 * the last one empty).
 *
 * @param str String to split; borrowed.
 * @param delimiter Code unit to split on.
 * @param outCount Receives the number of pieces produced; out param, must
 *                  not be NULL.
 * @return Array of *outCount new String* pointers, all sharing one Gc; free
 *         the whole array with stringArrayFree() - do not call stringFree()
 *         on individual elements.
 */
CNET_API String** stringSplit(const String* str, wchar_t delimiter, int* outCount);
/**
 * Frees an array of Strings previously returned by stringSplit(),
 * releasing the Gc shared by all of its elements (and the array itself).
 *
 * @param array Array returned by stringSplit(); ownership is taken.
 * @param count Number of elements in array, as returned via stringSplit()'s
 *              outCount.
 */
CNET_API void stringArrayFree(String** array, int count);

/**
 * Parses str as a base-10 signed integer (optional leading '+'/'-',
 * digits only, checked against the 32-bit int range).
 *
 * @param str String to parse; borrowed.
 * @param outValue Receives the parsed value on success; left unchanged on
 *                 failure. Out param, must not be NULL.
 * @return 1 on success, 0 if str is empty, malformed, or out of range.
 */
CNET_API int stringToInt(const String* str, int* outValue);
/**
 * Formats an integer as a decimal String.
 *
 * @param value Integer to format.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringFromInt(int value);

/**
 * Computes a hash (31*hash + code unit,
 * accumulated over UTF-16 code units, wrapping at 32 bits).
 *
 * @param str String to hash; borrowed.
 * @return Hash code (may be negative).
 */
CNET_API int stringHashCode(const String* str);

typedef struct sStringBuilder StringBuilder;

/**
 * Creates a new, empty StringBuilder for efficiently accumulating text.
 *
 * @return New owned StringBuilder* with its own Gc; free with
 *         stringBuilderFree().
 */
CNET_API StringBuilder* stringBuilderCreate(void);
/**
 * Frees a StringBuilder and its internal buffer.
 *
 * @param builder StringBuilder to free; ownership is taken.
 */
CNET_API void stringBuilderFree(StringBuilder* builder);
/**
 * Appends a NUL-terminated wide string to the builder's buffer.
 *
 * @param builder Builder to append to; borrowed.
 * @param chars NUL-terminated wide text to append; borrowed.
 */
CNET_API void stringBuilderAppendChars(StringBuilder* builder, const wchar_t* chars);
/**
 * Appends a String's code units to the builder's buffer.
 *
 * @param builder Builder to append to; borrowed.
 * @param str String to append; borrowed.
 */
CNET_API void stringBuilderAppendString(StringBuilder* builder, const String* str);
/**
 * Appends the decimal representation of an integer to the builder's
 * buffer.
 *
 * @param builder Builder to append to; borrowed.
 * @param value Integer to format and append.
 */
CNET_API void stringBuilderAppendInt(StringBuilder* builder, int value);
/**
 * Builds a new String from the builder's accumulated code units. The
 * builder itself is left intact and may still be used or freed
 * separately.
 *
 * @param builder Builder to read from; borrowed.
 * @return New owned String*; free with stringFree().
 */
CNET_API String* stringBuilderToString(const StringBuilder* builder);

#endif
