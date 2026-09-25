#pragma once

#ifndef CNET_SHA_H
#define CNET_SHA_H

#include "CNetGc.h"
#include "CNetApi.h"

#define SHA256_HASH_LENGTH 32

/**
 * Compute the SHA-256 hash of a buffer. Requires the CRYPTO license
 * feature.
 *
 * @param data     Bytes to hash; borrowed.
 * @param length   Length of data in bytes.
 * @param outHash  Buffer of SHA256_HASH_LENGTH (32) bytes that receives the
 *                 hash; caller-owned, filled in place.
 * @return true on success; false if the CRYPTO feature is not licensed or
 *         the hash operation failed.
 */
CNET_API bool sha256Hash(const unsigned char* data, int length, unsigned char outHash[SHA256_HASH_LENGTH]);

#endif
