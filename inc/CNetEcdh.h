#pragma once

#ifndef CNET_ECDH_H
#define CNET_ECDH_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sEcdhKeyPair EcdhKeyPair;

/**
 * Generate a new ephemeral ECDH key pair over NIST P-256. Requires the
 * CRYPTO license feature.
 *
 * @param gc  Arena the returned key pair is allocated from; borrowed.
 * @return Newly created key pair, owned by gc; or NULL if the CRYPTO
 *         feature is not licensed or key generation failed. Release the
 *         underlying key handle with ecdhKeyPairFree() when done.
 */
CNET_API EcdhKeyPair* ecdhKeyPairCreate(Gc* gc);
/**
 * Release the native key handle backing a key pair. The EcdhKeyPair struct
 * itself remains valid (it is owned by its Gc arena) but must no longer be
 * used after this call.
 *
 * @param keyPair  Key pair to release; borrowed.
 */
CNET_API void ecdhKeyPairFree(EcdhKeyPair* keyPair);

/**
 * Export the public key as a BCRYPT_ECCPUBLIC_BLOB-format byte blob,
 * suitable for sending to a peer and later passed to
 * ecdhDeriveSharedSecret().
 *
 * @param gc         Arena the returned blob is allocated from; borrowed.
 * @param keyPair    Key pair to export from; borrowed.
 * @param outLength  Receives the length of the returned blob in bytes.
 * @return Newly allocated blob, owned by gc; or NULL on failure.
 */
CNET_API unsigned char* ecdhKeyPairExportPublicBlob(Gc* gc, EcdhKeyPair* keyPair, int* outLength);

/**
 * Derive the shared secret from this key pair's private key and a peer's
 * public key blob (as produced by ecdhKeyPairExportPublicBlob() on the
 * other side of the exchange). Both sides of an exchange, given each
 * other's public blob, derive the same secret bytes.
 *
 * @param gc                     Arena the returned secret is allocated
 *                                from; borrowed.
 * @param keyPair                This side's key pair, providing the
 *                                private key; borrowed.
 * @param peerPublicBlob         BCRYPT_ECCPUBLIC_BLOB-format bytes from the
 *                                peer; borrowed.
 * @param peerPublicBlobLength   Length of peerPublicBlob in bytes.
 * @param outLength              Receives the length of the returned secret
 *                                in bytes.
 * @return Newly allocated shared secret buffer, owned by gc; or NULL if
 *         peerPublicBlob is malformed or the derivation failed.
 */
CNET_API unsigned char* ecdhDeriveSharedSecret(Gc* gc, EcdhKeyPair* keyPair,
                          const unsigned char* peerPublicBlob, int peerPublicBlobLength, int* outLength);

#endif
