#pragma once

#ifndef CNET_RSA_H
#define CNET_RSA_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sRsaKeyPair RsaKeyPair;

/**
 * Generate a new RSA key pair.
 *
 * @param gc       Arena the returned key pair is allocated from; borrowed.
 * @param keyBits  Modulus size in bits (e.g. 2048); larger values produce
 *                 stronger but slower keys. Must be at least 2048.
 * @return Newly created key pair holding both the public and private key,
 *         owned by gc; or NULL if keyBits is below 2048 or key generation
 *         failed. Release the underlying key handle with rsaKeyPairFree()
 *         when done.
 */
CNET_API RsaKeyPair* rsaKeyPairCreate(Gc* gc, int keyBits);
/**
 * Release the native key handle backing a key pair. The RsaKeyPair struct
 * itself remains valid (it is owned by its Gc arena) but must no longer be
 * used for cryptographic operations after this call.
 *
 * @param keyPair  Key pair to release; borrowed.
 */
CNET_API void rsaKeyPairFree(RsaKeyPair* keyPair);

/**
 * Encrypt a buffer with RSA-OAEP (SHA-256) using the key pair's public key.
 * Requires the CRYPTO license feature. Plaintext must be short enough to
 * fit the OAEP padding scheme for the key's modulus size.
 *
 * @param gc                   Arena the returned ciphertext is allocated
 *                             from; borrowed.
 * @param keyPair              Key pair providing the public key; borrowed.
 * @param plaintext            Bytes to encrypt; borrowed.
 * @param plaintextLength      Length of plaintext in bytes.
 * @param outCiphertextLength  Receives the length of the returned buffer
 *                             (equal to the RSA modulus size in bytes).
 * @return Newly allocated ciphertext buffer, owned by gc; or NULL if the
 *         CRYPTO feature is not licensed or encryption failed.
 */
CNET_API unsigned char* rsaEncrypt(Gc* gc, RsaKeyPair* keyPair, const unsigned char* plaintext, int plaintextLength, int* outCiphertextLength);
/**
 * Decrypt a buffer produced by rsaEncrypt() using the key pair's private
 * key.
 *
 * @param gc                  Arena the returned plaintext is allocated
 *                            from; borrowed.
 * @param keyPair             Key pair providing the private key; borrowed.
 * @param ciphertext          Bytes to decrypt, as produced by rsaEncrypt();
 *                            borrowed.
 * @param ciphertextLength    Length of ciphertext in bytes.
 * @param outPlaintextLength  Receives the length of the recovered
 *                            plaintext.
 * @return Newly allocated plaintext buffer, owned by gc; or NULL if
 *         decryption failed (wrong key or malformed ciphertext).
 */
CNET_API unsigned char* rsaDecrypt(Gc* gc, RsaKeyPair* keyPair, const unsigned char* ciphertext, int ciphertextLength, int* outPlaintextLength);

/**
 * Sign a payload with RSA-PSS over its SHA-256 hash, using the key pair's
 * private key.
 *
 * @param gc                    Arena the returned signature is allocated
 *                              from; borrowed.
 * @param keyPair               Key pair providing the private key;
 *                              borrowed.
 * @param data                  Bytes to sign; borrowed.
 * @param dataLength            Length of data in bytes.
 * @param outSignatureLength    Receives the length of the returned
 *                              signature (equal to the RSA modulus size in
 *                              bytes).
 * @return Newly allocated signature buffer, owned by gc; or NULL on
 *         failure.
 */
CNET_API unsigned char* rsaSign(Gc* gc, RsaKeyPair* keyPair, const unsigned char* data, int dataLength, int* outSignatureLength);
/**
 * Verify an RSA-PSS signature over the SHA-256 hash of a payload, using
 * the key pair's public key.
 *
 * @param keyPair         Key pair providing the public key; borrowed.
 * @param data            Bytes that were signed; borrowed.
 * @param dataLength      Length of data in bytes.
 * @param signature       Signature to verify, as produced by rsaSign();
 *                        borrowed.
 * @param signatureLength Length of signature in bytes.
 * @return 1 if the signature is valid for data and this key pair; 0 if
 *         invalid or malformed, or if the verification operation itself
 *         could not be performed (check getLastErrorMessage() to
 *         distinguish the latter from a genuinely invalid signature).
 */
CNET_API int rsaVerify(RsaKeyPair* keyPair, const unsigned char* data, int dataLength, const unsigned char* signature, int signatureLength);

/**
 * Export the public key as a BCRYPT_RSAPUBLIC_BLOB-format byte blob,
 * suitable for later import with rsaKeyPairImportPublicBlob().
 *
 * @param gc         Arena the returned blob is allocated from; borrowed.
 * @param keyPair    Key pair to export from; borrowed.
 * @param outLength  Receives the length of the returned blob in bytes.
 * @return Newly allocated blob, owned by gc; or NULL on failure.
 */
CNET_API unsigned char* rsaKeyPairExportPublicBlob(Gc* gc, RsaKeyPair* keyPair, int* outLength);
/**
 * Export the full key pair (public and private key material) as a
 * BCRYPT_RSAFULLPRIVATE_BLOB-format byte blob, suitable for later import
 * with rsaKeyPairImportPrivateBlob(). The blob contains private key
 * material and must be handled and stored as securely as the key itself.
 *
 * @param gc         Arena the returned blob is allocated from; borrowed.
 * @param keyPair    Key pair to export from; borrowed.
 * @param outLength  Receives the length of the returned blob in bytes.
 * @return Newly allocated blob, owned by gc; or NULL on failure.
 */
CNET_API unsigned char* rsaKeyPairExportPrivateBlob(Gc* gc, RsaKeyPair* keyPair, int* outLength);
/**
 * Import a public key previously exported with
 * rsaKeyPairExportPublicBlob(). The resulting key pair can be used for
 * rsaEncrypt() and rsaVerify() but not rsaDecrypt() or rsaSign().
 *
 * @param gc      Arena the returned key pair is allocated from; borrowed.
 * @param blob    BCRYPT_RSAPUBLIC_BLOB-format bytes; borrowed.
 * @param length  Length of blob in bytes.
 * @return Newly created key pair, owned by gc; or NULL on failure.
 */
CNET_API RsaKeyPair* rsaKeyPairImportPublicBlob(Gc* gc, const unsigned char* blob, int length);
/**
 * Import a key pair previously exported with
 * rsaKeyPairExportPrivateBlob(). The resulting key pair can be used for
 * all RSA operations, including rsaDecrypt() and rsaSign().
 *
 * @param gc      Arena the returned key pair is allocated from; borrowed.
 * @param blob    BCRYPT_RSAFULLPRIVATE_BLOB-format bytes; borrowed.
 * @param length  Length of blob in bytes.
 * @return Newly created key pair, owned by gc; or NULL on failure.
 */
CNET_API RsaKeyPair* rsaKeyPairImportPrivateBlob(Gc* gc, const unsigned char* blob, int length);

#endif
