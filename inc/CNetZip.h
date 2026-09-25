#pragma once

#ifndef CNET_ZIP_H
#define CNET_ZIP_H

#include "CNetGc.h"
#include "CNetApi.h"

/**
 * Compress a byte buffer using the MSZIP algorithm (via the Windows
 * Compression API). On Linux/macOS, uses zlib's deflate (compress2())
 * instead; the round-trip contract with zipDecompress() below is preserved
 * but the compressed bytes are not wire-compatible across platforms.
 *
 * @param gc                    Arena the returned buffer is allocated
 *                               from; borrowed.
 * @param data                  Bytes to compress; borrowed; may be NULL
 *                               only when `length` is 0.
 * @param length                Number of bytes in `data`.
 * @param outCompressedLength   Set to the length of the returned buffer
 *                               (0 if `length` is 0); must not be NULL.
 * @return Compressed bytes, owned by `gc`; NULL on failure (see
 *         getLastError()/getLastErrorMessage()).
 */
CNET_API unsigned char* zipCompress(Gc* gc, const unsigned char* data, int length, int* outCompressedLength);

/**
 * Decompress a buffer previously produced by zipCompress().
 *
 * @param gc                 Arena the returned buffer is allocated from;
 *                            borrowed.
 * @param compressedData     Compressed bytes; borrowed; may be NULL only
 *                            when `compressedLength` is 0.
 * @param compressedLength   Number of bytes in `compressedData`.
 * @param outLength          Set to the length of the returned buffer (0
 *                            if `compressedLength` is 0); must not be
 *                            NULL.
 * @return Decompressed bytes, owned by `gc`; NULL on failure (see
 *         getLastError()/getLastErrorMessage()).
 */
CNET_API unsigned char* zipDecompress(Gc* gc, const unsigned char* compressedData, int compressedLength, int* outLength);

#endif
