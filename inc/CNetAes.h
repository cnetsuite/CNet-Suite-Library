#pragma once

#ifndef CNET_AES_H
#define CNET_AES_H

#include "CNetGc.h"
#include "CNetApi.h"

#define AES_KEY_LENGTH 32
#define AES_NONCE_LENGTH 12
#define AES_TAG_LENGTH 16

/**
 * Generate a random AES-256 key using the system CSPRNG.
 *
 * @param outKey  Buffer of AES_KEY_LENGTH (32) bytes that receives the
 *                generated key; caller-owned, filled in place.
 * @return true on success; false if the OS random generator failed.
 */
CNET_API bool aesGenerateKey(unsigned char outKey[AES_KEY_LENGTH]);

/**
 * Encrypt a buffer with AES-256-GCM, producing a random nonce and an
 * authentication tag alongside the ciphertext. Requires the CRYPTO license
 * feature.
 *
 * @param gc                   Arena the returned ciphertext buffer is
 *                             allocated from; borrowed.
 * @param key                  AES_KEY_LENGTH (32) byte key; borrowed.
 * @param plaintext             Bytes to encrypt; borrowed.
 * @param plaintextLength       Length of plaintext in bytes.
 * @param outCiphertextLength  Receives the length of the returned buffer:
 *                             plaintextLength + AES_NONCE_LENGTH +
 *                             AES_TAG_LENGTH.
 * @return Newly allocated buffer laid out as nonce (AES_NONCE_LENGTH bytes)
 *         || ciphertext || tag (AES_TAG_LENGTH bytes), owned by gc; or NULL
 *         if the CRYPTO feature is not licensed or encryption failed.
 */
CNET_API unsigned char* aesEncrypt(Gc* gc, const unsigned char key[AES_KEY_LENGTH],
                          const unsigned char* plaintext, int plaintextLength, int* outCiphertextLength);
/**
 * Decrypt a buffer produced by aesEncrypt(), verifying its authentication
 * tag.
 *
 * @param gc                  Arena the returned plaintext buffer is
 *                            allocated from; borrowed.
 * @param key                 AES_KEY_LENGTH (32) byte key; borrowed.
 * @param ciphertext          Buffer laid out as nonce || ciphertext || tag,
 *                            as produced by aesEncrypt(); borrowed. Must be
 *                            at least AES_NONCE_LENGTH + AES_TAG_LENGTH
 *                            bytes long.
 * @param ciphertextLength    Length of ciphertext in bytes.
 * @param outPlaintextLength  Receives the length of the returned plaintext:
 *                            ciphertextLength - AES_NONCE_LENGTH -
 *                            AES_TAG_LENGTH.
 * @return Newly allocated plaintext buffer, owned by gc; or NULL if the
 *         buffer is too short, the key is wrong, or the data was tampered
 *         with (tag verification failure).
 */
CNET_API unsigned char* aesDecrypt(Gc* gc, const unsigned char key[AES_KEY_LENGTH],
                          const unsigned char* ciphertext, int ciphertextLength, int* outPlaintextLength);

#endif
