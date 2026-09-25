#pragma once

#ifndef CNET_JSON_H
#define CNET_JSON_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef enum {
    JSON_NULL = 0,   // The JSON literal `null`.
    JSON_BOOL = 1,   // A boolean, `true` or `false`.
    JSON_NUMBER = 2, // A numeric value, stored as a double.
    JSON_STRING = 3, // A wide-text string.
    JSON_ARRAY = 4,  // An ordered list of values.
    JSON_OBJECT = 5  // An ordered set of key/value members.
} JsonType;

typedef struct sJsonValue JsonValue;

/**
 * Create a JSON null value.
 *
 * @param gc  Arena the value is allocated from; borrowed.
 * @return New JsonValue of type JSON_NULL, owned by the arena.
 */
CNET_API JsonValue* jsonCreateNull(Gc* gc);

/**
 * Create a JSON boolean value.
 *
 * @param gc     Arena the value is allocated from; borrowed.
 * @param value  Nonzero for true, zero for false.
 * @return New JsonValue of type JSON_BOOL, owned by the arena.
 */
CNET_API JsonValue* jsonCreateBool(Gc* gc, int value);

/**
 * Create a JSON number value.
 *
 * @param gc     Arena the value is allocated from; borrowed.
 * @param value  Numeric payload.
 * @return New JsonValue of type JSON_NUMBER, owned by the arena.
 */
CNET_API JsonValue* jsonCreateNumber(Gc* gc, double value);

/**
 * Create a JSON string value. The text is copied, so the caller retains
 * ownership of `value` and may free or modify it after this call.
 *
 * @param gc     Arena the value is allocated from; borrowed.
 * @param value  Wide text to copy; must not be NULL.
 * @return New JsonValue of type JSON_STRING, owned by the arena; NULL if
 *         `value` is NULL.
 */
CNET_API JsonValue* jsonCreateString(Gc* gc, const wchar_t* value);

/**
 * Create an empty JSON array.
 *
 * @param gc  Arena the value is allocated from; borrowed.
 * @return New JsonValue of type JSON_ARRAY, owned by the arena.
 */
CNET_API JsonValue* jsonCreateArray(Gc* gc);

/**
 * Create an empty JSON object.
 *
 * @param gc  Arena the value is allocated from; borrowed.
 * @return New JsonValue of type JSON_OBJECT, owned by the arena.
 */
CNET_API JsonValue* jsonCreateObject(Gc* gc);

/**
 * Get the type tag of a value.
 *
 * @param value  Value to inspect; borrowed.
 * @return The value's JsonType.
 */
CNET_API JsonType jsonGetType(const JsonValue* value);

/**
 * Test whether a value is JSON null.
 *
 * @param value  Value to inspect; borrowed.
 * @return Nonzero if the value's type is JSON_NULL, zero otherwise.
 */
CNET_API int jsonIsNull(const JsonValue* value);

/**
 * Get the boolean payload of a value.
 *
 * @param value  Value to read; borrowed; must be of type JSON_BOOL.
 * @return 1 or 0; 0 (with an error set) if `value` is not a JSON_BOOL.
 */
CNET_API int jsonGetBool(const JsonValue* value);

/**
 * Get the numeric payload of a value.
 *
 * @param value  Value to read; borrowed; must be of type JSON_NUMBER.
 * @return The number; 0.0 (with an error set) if `value` is not a
 *         JSON_NUMBER.
 */
CNET_API double jsonGetNumber(const JsonValue* value);

/**
 * Get the string payload of a value.
 *
 * @param value  Value to read; borrowed; must be of type JSON_STRING.
 * @return Pointer to the wide text, owned by `value`/its arena (do not
 *         free); NULL (with an error set) if `value` is not a JSON_STRING.
 */
CNET_API const wchar_t* jsonGetString(const JsonValue* value);

/**
 * Get the number of elements in a JSON array.
 *
 * @param array  Value to inspect; borrowed; must be of type JSON_ARRAY.
 * @return Element count; 0 (with an error set) if `array` is not a
 *         JSON_ARRAY.
 */
CNET_API int jsonArraySize(const JsonValue* array);

/**
 * Get the element at a position in a JSON array.
 *
 * @param array  Array to read; borrowed; must be of type JSON_ARRAY.
 * @param index  Zero-based position.
 * @return The element, borrowed from `array`; NULL if `array` is not a
 *         JSON_ARRAY or `index` is out of range.
 */
CNET_API JsonValue* jsonArrayGet(const JsonValue* array, int index);

/**
 * Append an item to the end of a JSON array.
 *
 * @param array  Array to modify; borrowed; must be of type JSON_ARRAY.
 * @param item   Value to append; ownership stays with its arena, `array`
 *               only stores a reference.
 * @return true on success; false if `array` is not a JSON_ARRAY.
 */
CNET_API bool jsonArrayAdd(JsonValue* array, JsonValue* item);

/**
 * Get the number of members in a JSON object.
 *
 * @param object  Value to inspect; borrowed; must be of type JSON_OBJECT.
 * @return Member count; 0 (with an error set) if `object` is not a
 *         JSON_OBJECT.
 */
CNET_API int jsonObjectSize(const JsonValue* object);

/**
 * Get the key of the member at a position, in insertion order.
 *
 * @param object  Object to read; borrowed; must be of type JSON_OBJECT.
 * @param index   Zero-based position.
 * @return The key, borrowed from `object`; NULL if `object` is not a
 *         JSON_OBJECT or `index` is out of range.
 */
CNET_API const wchar_t* jsonObjectKeyAt(const JsonValue* object, int index);

/**
 * Get the value of the member at a position, in insertion order.
 *
 * @param object  Object to read; borrowed; must be of type JSON_OBJECT.
 * @param index   Zero-based position.
 * @return The value, borrowed from `object`; NULL if `object` is not a
 *         JSON_OBJECT or `index` is out of range.
 */
CNET_API JsonValue* jsonObjectValueAt(const JsonValue* object, int index);

/**
 * Look up a member on a JSON object by key.
 *
 * @param object  Object to read; borrowed; must be of type JSON_OBJECT.
 * @param key     Key to look up.
 * @return The member's value, borrowed from `object`; NULL if `object` is
 *         not a JSON_OBJECT or no member has that key.
 */
CNET_API JsonValue* jsonObjectGet(const JsonValue* object, const wchar_t* key);

/**
 * Test whether a JSON object has a member with the given key.
 *
 * @param object  Object to inspect; borrowed.
 * @param key     Key to look up.
 * @return Nonzero if a member with that key exists, zero otherwise.
 */
CNET_API int jsonObjectHasKey(const JsonValue* object, const wchar_t* key);

/**
 * Insert or overwrite a member on a JSON object. If `key` already exists,
 * its value is replaced; otherwise a new member is appended. The key text
 * is copied, so the caller retains ownership of `key`.
 *
 * @param object  Object to modify; borrowed; must be of type JSON_OBJECT.
 * @param key     Member key to set.
 * @param value   Value to store; ownership stays with its arena, `object`
 *                only stores a reference.
 * @return true on success; false if `object` is not a JSON_OBJECT.
 */
CNET_API bool jsonObjectPut(JsonValue* object, const wchar_t* key, JsonValue* value);

/**
 * Parse JSON text into a value tree.
 *
 * @param gc    Arena the resulting tree is allocated from; borrowed.
 * @param text  NUL-terminated wide JSON text to parse; caller retains
 *              ownership.
 * @return Root of the parsed value tree, owned by `gc`; NULL on parse
 *         failure (see getLastError()/getLastErrorMessage()).
 */
CNET_API JsonValue* jsonParse(Gc* gc, const wchar_t* text);

/**
 * Serialize a value tree back to compact JSON text (no inserted
 * whitespace).
 *
 * @param gc     Arena the returned string is allocated from; borrowed.
 * @param value  Root value to serialize; borrowed.
 * @return NUL-terminated wide JSON text, owned by `gc`.
 */
CNET_API wchar_t* jsonStringify(Gc* gc, const JsonValue* value);

#endif
