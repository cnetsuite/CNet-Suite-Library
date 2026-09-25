#pragma once

#ifndef CNET_BASE64_H
#define CNET_BASE64_H

#include "CNetGc.h"
#include "CNetApi.h"

/**
 * Encodes bytes as standard Base64 (RFC 4648 alphabet using '+' and '/',
 * with '=' padding).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param data Bytes to encode; borrowed. May be NULL when length is 0.
 * @param length Number of bytes in data.
 * @return NUL-terminated wide Base64 string allocated from gc.
 */
CNET_API wchar_t* base64Encode(Gc* gc, const unsigned char* data, int length);
/**
 * Decodes a standard Base64 string back to bytes.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param text NUL-terminated wide standard Base64 text; borrowed. Length
 *             must be a multiple of 4.
 * @param outLength Receives the number of decoded bytes; out param, must
 *                   not be NULL.
 * @return Decoded bytes allocated from gc; NULL (with *outLength set to 0)
 *         if text has an invalid length or contains characters outside
 *         the alphabet.
 */
CNET_API unsigned char* base64Decode(Gc* gc, const wchar_t* text, int* outLength);

/**
 * Encodes bytes as URL-and-filename-safe Base64 ('-' and '_' in place of
 * '+' and '/'; still uses '=' padding).
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param data Bytes to encode; borrowed. May be NULL when length is 0.
 * @param length Number of bytes in data.
 * @return NUL-terminated wide Base64 string allocated from gc.
 */
CNET_API wchar_t* base64UrlEncode(Gc* gc, const unsigned char* data, int length);
/**
 * Decodes a URL-and-filename-safe Base64 string back to bytes.
 *
 * @param gc Allocator the result is allocated from; caller owns gc.
 * @param text NUL-terminated wide URL-safe Base64 text; borrowed. Length
 *             must be a multiple of 4.
 * @param outLength Receives the number of decoded bytes; out param, must
 *                   not be NULL.
 * @return Decoded bytes allocated from gc; NULL (with *outLength set to 0)
 *         if text has an invalid length or contains characters outside
 *         the alphabet.
 */
CNET_API unsigned char* base64UrlDecode(Gc* gc, const wchar_t* text, int* outLength);

#endif
