#pragma once

#ifndef CNET_SERVER_SOCKET_H
#define CNET_SERVER_SOCKET_H

#include "CNetGc.h"
#include "CNetSocket.h"
#include "CNetApi.h"

typedef struct sServerSocket ServerSocket;

/**
 * Create a TCP listening socket bound to all local interfaces. Requires
 * the NETWORK license feature.
 *
 * @param port  TCP port to bind to (1-65535), or 0 to let the OS choose
 *              any available free port (retrieve it afterward with
 *              serverSocketGetLocalPort()).
 * @return Newly created ServerSocket owning its own arena, already
 *         listening; free it with serverSocketFree() when done. NULL if
 *         the NETWORK feature is not licensed or the socket could not be
 *         bound/listened on.
 */
CNET_API ServerSocket* serverSocketCreate(int port);
/**
 * Free a ServerSocket: releases any TLS credentials enabled on it, closes
 * the listening socket, and frees its own arena.
 *
 * @param serverSocket  Server socket to free; caller must not use it
 *                       afterward.
 */
CNET_API void serverSocketFree(ServerSocket* serverSocket);

/**
 * Block until an inbound connection arrives, then accept it.
 *
 * @param serverSocket  Server socket to accept on; borrowed.
 * @return Newly created, non-TLS Socket wrapping the accepted connection,
 *         owning its own arena; free it with socketFree() when done. NULL
 *         on error.
 */
CNET_API Socket* serverSocketAccept(ServerSocket* serverSocket);
/**
 * Get the local TCP port this server socket is bound to; useful for
 * discovering the OS-assigned port when serverSocketCreate() was called
 * with port 0.
 *
 * @param serverSocket  Server socket to inspect; borrowed.
 * @return The local port number; 0 if it could not be determined.
 */
CNET_API int serverSocketGetLocalPort(const ServerSocket* serverSocket);
/**
 * Poll for a pending inbound connection without blocking indefinitely.
 *
 * @param serverSocket   Server socket to poll; borrowed.
 * @param timeoutMillis  Maximum time to wait, in milliseconds.
 * @return Nonzero if a connection is pending (ready to be accepted) before
 *         the timeout elapses; 0 if the timeout elapses first.
 */
CNET_API int serverSocketWaitForConnection(const ServerSocket* serverSocket, int timeoutMillis);

/**
 * Generate and install a self-signed certificate for this server socket to
 * present on future TLS connections accepted via serverSocketAcceptTls().
 *
 * @param serverSocket  Server socket to configure; must not already have
 *                       TLS enabled; borrowed.
 * @param commonName    Certificate subject common name (CN), e.g. a
 *                       hostname, wide (UTF-16); borrowed.
 * @return true on success; false if TLS was already enabled on this
 *         server socket or certificate generation failed.
 */
CNET_API bool serverSocketEnableTls(ServerSocket* serverSocket, const wchar_t* commonName);
/**
 * Read the SHA-1 thumbprint of the certificate enabled on this server
 * socket via serverSocketEnableTls().
 *
 * @param serverSocket   Server socket to inspect; borrowed.
 * @param outThumbprint  Buffer of exactly 20 bytes that receives the
 *                       SHA-1 thumbprint; caller-owned, filled in place.
 * @return true on success; false if TLS has not been enabled on this
 *         server socket.
 */
CNET_API bool serverSocketGetCertThumbprint(const ServerSocket* serverSocket, unsigned char outThumbprint[20]);
/**
 * Block until an inbound connection arrives, accept it, and immediately
 * perform the TLS server handshake on it using the credentials enabled via
 * serverSocketEnableTls().
 *
 * @param serverSocket  Server socket to accept on; must have TLS enabled;
 *                       borrowed.
 * @return Newly created Socket with TLS already established, owning its
 *         own arena; free it with socketFree() when done. NULL if TLS is
 *         not enabled on this server socket, accept fails, or the TLS
 *         handshake fails.
 */
CNET_API Socket* serverSocketAcceptTls(ServerSocket* serverSocket);

#endif
