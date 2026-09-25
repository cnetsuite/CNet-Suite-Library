#pragma once

#ifndef CNET_SOCKET_H
#define CNET_SOCKET_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sSocket Socket;

/**
 * Resolve a hostname and open a TCP (IPv4) connection to it. Requires the
 * NETWORK license feature.
 *
 * @param host  Hostname or IPv4 address to connect to, wide (UTF-16); borrowed.
 * @param port  TCP port to connect to (1-65535).
 * @return Newly created Socket owning its own arena; free it with
 *         socketFree() when done. NULL if the NETWORK feature is not
 *         licensed, the host cannot be resolved, or the connection fails.
 */
CNET_API Socket* socketCreate(const wchar_t* host, int port);
/**
 * Wrap an already-connected native socket handle (e.g. one produced by
 * serverSocketAccept()) in a Socket.
 *
 * @param handle  Native OS SOCKET handle, passed as void*; ownership
 *                transfers to the returned Socket (it is closed by
 *                socketClose()/socketFree()).
 * @return Newly created Socket owning its own arena; free it with
 *         socketFree() when done.
 */
CNET_API Socket* socketWrap(void* handle);
/**
 * Close (if not already closed) and free a Socket, including its TLS
 * connection (if any) and its own arena.
 *
 * @param socket  Socket to free; caller must not use it afterward.
 */
CNET_API void socketFree(Socket* socket);

/**
 * Shut down any TLS session and close the underlying OS socket handle.
 * Does not free the Socket structure itself; socketFree() may still be
 * called afterward.
 *
 * @param socket  Socket to close; borrowed.
 */
CNET_API void socketClose(Socket* socket);
/**
 * Send bytes on the connection. If TLS has been started on this socket,
 * the data is encrypted first.
 *
 * @param socket  Socket to send on; borrowed.
 * @param data    Bytes to send; borrowed.
 * @param length  Length of data in bytes.
 * @return Number of bytes sent; a negative/error sentinel value if the
 *         send failed.
 */
CNET_API int socketSend(Socket* socket, const void* data, int length);
/**
 * Receive bytes from the connection. If TLS has been started on this
 * socket, the data is decrypted first.
 *
 * @param socket        Socket to receive from; borrowed.
 * @param buffer        Destination buffer; caller-owned, filled in place.
 * @param bufferLength  Capacity of buffer in bytes.
 * @return Number of bytes copied into buffer; 0 if the peer has closed the
 *         connection; a negative/error sentinel value on error.
 */
CNET_API int socketReceive(Socket* socket, void* buffer, int bufferLength);
/**
 * Get the remote peer's TCP port.
 *
 * @param socket  Socket to inspect; borrowed.
 * @return The peer's port number; 0 if it could not be determined.
 */
CNET_API int socketGetPort(const Socket* socket);
/**
 * Get the local TCP port this socket is bound to.
 *
 * @param socket  Socket to inspect; borrowed.
 * @return The local port number; 0 if it could not be determined.
 */
CNET_API int socketGetLocalPort(const Socket* socket);
/**
 * Set both the send and receive timeouts on the underlying OS socket.
 *
 * @param socket         Socket to configure; borrowed.
 * @param timeoutMillis  Timeout in milliseconds applied to both send and
 *                       receive operations; a blocked send/receive fails
 *                       once this much time has elapsed.
 */
CNET_API void socketSetTimeouts(Socket* socket, int timeoutMillis);

/**
 * Upgrade an already-connected plain socket to TLS as the client side.
 * Requires the NETWORK license feature. The peer certificate's chain,
 * hostname (against serverName), and expiry are always validated.
 * expectedThumbprint is not a way to skip validation: it only overrides an
 * otherwise-untrusted root (e.g. a self-signed certificate) when the peer
 * certificate's exact SHA-1 thumbprint matches it; any other validation
 * failure is rejected even when a thumbprint is supplied.
 *
 * @param socket              Socket to upgrade; must not already have TLS
 *                            started; borrowed.
 * @param serverName          Server name used for the TLS handshake
 *                            (SNI/target name) and for hostname
 *                            validation, wide (UTF-16); borrowed.
 * @param expectedThumbprint  SHA-1 thumbprint used to trust an otherwise
 *                            untrusted (e.g. self-signed) root, or NULL to
 *                            rely solely on normal chain trust; borrowed.
 * @param thumbprintLength    Length of expectedThumbprint in bytes; must
 *                            be 20 when expectedThumbprint is non-NULL.
 * @return true if the handshake succeeded; false if TLS was already
 *         started on this socket, the NETWORK feature is not licensed, or
 *         the peer certificate fails validation.
 */
CNET_API bool socketStartTlsClient(Socket* socket, const wchar_t* serverName,
                          const unsigned char* expectedThumbprint, int thumbprintLength);

#endif
