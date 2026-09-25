#pragma once

#ifndef CNET_TLS_H
#define CNET_TLS_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sTlsConnection TlsConnection;
typedef struct sTlsServerCredentials TlsServerCredentials;

/**
 * Create ephemeral server credentials backed by a freshly generated
 * 2048-bit RSA key and a self-signed certificate for the given common
 * name. The private key is held in a temporary CSP key container that is
 * deleted when the credentials are freed. On Linux/macOS, backed by
 * OpenSSL instead of SChannel/CryptoAPI (no CSP/key-container concept
 * there — see Tls.posix.c's header comment for the full API mapping,
 * including how tlsServerCredentialsLoadFromStore()'s Windows certificate
 * store becomes a fixed directory of "<thumbprint>.pem" files).
 *
 * @param gc          Arena the returned credentials are allocated from;
 *                    borrowed.
 * @param commonName  Certificate subject common name (CN), e.g. a
 *                    hostname, wide (UTF-16); borrowed, copied.
 * @return Newly created credentials, owned by gc; free the underlying
 *         native handles with tlsServerCredentialsFree() when done. NULL
 *         on failure.
 */
CNET_API TlsServerCredentials* tlsServerCredentialsCreateSelfSigned(Gc* gc, const wchar_t* commonName);
/**
 * Load server credentials from a PKCS#12 (.pfx) file, using the first
 * certificate in the file that has an associated private key.
 *
 * @param gc        Arena the returned credentials are allocated from;
 *                  borrowed.
 * @param pfxPath   Path to the .pfx file, wide (UTF-16); borrowed.
 * @param password  Password protecting the .pfx file, wide (UTF-16);
 *                  borrowed. May be an empty string if the file is
 *                  unprotected.
 * @return Newly created credentials, owned by gc; free the underlying
 *         native handles with tlsServerCredentialsFree() when done. NULL
 *         if the file cannot be read/decrypted or contains no certificate
 *         with a private key.
 */
CNET_API TlsServerCredentials* tlsServerCredentialsLoadFromPfxFile(Gc* gc, const wchar_t* pfxPath, const wchar_t* password);
/**
 * Load server credentials from a certificate already installed in the
 * current user's "MY" certificate store, looked up by thumbprint.
 *
 * @param gc                Arena the returned credentials are allocated
 *                          from; borrowed.
 * @param thumbprint        SHA-1 thumbprint identifying the certificate;
 *                          borrowed.
 * @param thumbprintLength  Length of thumbprint in bytes; must be 20 to
 *                          match a certificate's SHA-1 hash.
 * @return Newly created credentials, owned by gc; free the underlying
 *         native handles with tlsServerCredentialsFree() when done. NULL
 *         if no matching certificate is found in the store.
 */
CNET_API TlsServerCredentials* tlsServerCredentialsLoadFromStore(Gc* gc, const unsigned char* thumbprint, int thumbprintLength);
/**
 * Release the native credential/certificate handles backing a
 * TlsServerCredentials, and delete the key container if it was created by
 * tlsServerCredentialsCreateSelfSigned().
 *
 * @param creds  Credentials to release; borrowed.
 */
CNET_API void tlsServerCredentialsFree(TlsServerCredentials* creds);
/**
 * Read the SHA-1 thumbprint of the certificate backing these credentials.
 *
 * @param creds           Credentials to inspect; borrowed.
 * @param outThumbprint   Buffer of exactly 20 bytes that receives the
 *                        SHA-1 thumbprint; caller-owned, filled in place.
 * @return true on success; false if the thumbprint could not be computed.
 */
CNET_API bool tlsServerCredentialsGetThumbprint(const TlsServerCredentials* creds, unsigned char outThumbprint[20]);

/**
 * Perform a TLS client-side handshake over an already-connected raw socket
 * handle. Requires the NETWORK license feature. The peer certificate's
 * chain, hostname (against serverName), and expiry are always validated.
 * expectedThumbprint is not a way to skip validation: it only overrides an
 * otherwise-untrusted root (e.g. a self-signed certificate) when the peer
 * certificate's exact SHA-1 thumbprint matches it; any other validation
 * failure (expired, wrong hostname, revoked, or any chain error unrelated
 * to root trust) is rejected even when a thumbprint is supplied.
 *
 * @param gc                   Arena the returned connection is allocated
 *                             from; borrowed.
 * @param socketHandle         Native OS socket handle (SOCKET, passed as
 *                             void*) of an already-connected TCP socket;
 *                             borrowed, remains owned by the caller.
 * @param serverName           Server name used for the TLS handshake
 *                             (SNI/target name) and for hostname
 *                             validation, wide (UTF-16); borrowed.
 * @param expectedThumbprint   SHA-1 thumbprint used to trust an otherwise
 *                             untrusted (e.g. self-signed) root, or NULL to
 *                             rely solely on normal chain trust; borrowed.
 * @param thumbprintLength     Length of expectedThumbprint in bytes; must
 *                             be 20 when expectedThumbprint is non-NULL.
 * @return Newly established connection, owned by gc; free it with
 *         tlsConnectionFree() when done. NULL if the NETWORK feature is
 *         not licensed, the handshake fails, or the peer certificate fails
 *         validation.
 */
CNET_API TlsConnection* tlsClientHandshake(Gc* gc, void* socketHandle, const wchar_t* serverName,
                                  const unsigned char* expectedThumbprint, int thumbprintLength);
/**
 * Perform a TLS server-side handshake over an already-accepted raw socket
 * handle, using the given server credentials. Requires the NETWORK
 * license feature.
 *
 * @param gc            Arena the returned connection is allocated from;
 *                      borrowed.
 * @param socketHandle  Native OS socket handle (SOCKET, passed as void*)
 *                      of an already-accepted TCP connection; borrowed,
 *                      remains owned by the caller.
 * @param creds         Server credentials (certificate/key) to present to
 *                      the client; borrowed.
 * @return Newly established connection, owned by gc; free it with
 *         tlsConnectionFree() when done. NULL if the NETWORK feature is
 *         not licensed or the handshake fails.
 */
CNET_API TlsConnection* tlsServerHandshake(Gc* gc, void* socketHandle, TlsServerCredentials* creds);
/**
 * Shut down a TLS connection (exchanging a close-notify token with the
 * peer where possible) and release its security context. Does not close
 * the underlying socket handle.
 *
 * @param conn  Connection to close; borrowed.
 */
CNET_API void tlsConnectionFree(TlsConnection* conn);

/**
 * Encrypt and send data over an established TLS connection, splitting it
 * into records no larger than the negotiated maximum message size.
 *
 * @param conn    Connection to send on; borrowed.
 * @param data    Bytes to send; borrowed.
 * @param length  Length of data in bytes.
 * @return Number of bytes actually sent (may be less than length only on
 *         error); check the process error state if the result is short.
 */
CNET_API int tlsSend(TlsConnection* conn, const void* data, int length);
/**
 * Receive and decrypt data from an established TLS connection.
 *
 * @param conn          Connection to receive from; borrowed.
 * @param buffer        Destination buffer; caller-owned, filled in place.
 * @param bufferLength  Capacity of buffer in bytes.
 * @return Number of plaintext bytes copied into buffer (0 to
 *         bufferLength); 0 if the connection was cleanly closed by the
 *         peer; -1 on error.
 */
CNET_API int tlsReceive(TlsConnection* conn, void* buffer, int bufferLength);

#endif
