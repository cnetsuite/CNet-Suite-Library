#pragma once

#ifndef CNET_SERIAL_H
#define CNET_SERIAL_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef enum {
    SERIAL_PARITY_NONE = 0,  /* No parity bit. */
    SERIAL_PARITY_ODD = 1,   /* Odd parity. */
    SERIAL_PARITY_EVEN = 2,  /* Even parity. */
    SERIAL_PARITY_MARK = 3,  /* Parity bit always 1 ("mark"). */
    SERIAL_PARITY_SPACE = 4  /* Parity bit always 0 ("space"). */
} SerialParity;

typedef enum {
    SERIAL_STOP_BITS_1 = 0,    /* 1 stop bit. */
    SERIAL_STOP_BITS_1_5 = 1,  /* 1.5 stop bits. */
    SERIAL_STOP_BITS_2 = 2     /* 2 stop bits. */
} SerialStopBits;

typedef struct sSerialPort SerialPort;

/**
 * Open a COM port and configure its line settings. Sets a default read
 * timeout of ~1000ms and write timeout of ~1000ms (adjustable afterward
 * via serialPortSetReadTimeout()/serialPortSetWriteTimeout()). On
 * Linux/macOS, backed by termios instead of DCB/COMMTIMEOUTS — portName is
 * expected to already be a full device path (e.g. L"/dev/ttyUSB0") rather
 * than a bare COM-port name, since there's no bare-name-plus-prefix
 * convention there; see Serial.posix.c's header comment for the handful of
 * other termios-vs-DCB approximations (1.5 stop bits, mark/space parity).
 *
 * @param portName Bare port name, e.g. L"COM3" (internally opened as
 *                 L"\\\\.\\COM3" so ports above COM9 work); borrowed.
 * @param baudRate Baud rate in bits/second (e.g. 9600, 115200); passed
 *                 directly to the driver, which rejects unsupported rates.
 * @param dataBits Number of data bits per byte (typically 7 or 8).
 * @param stopBits Number of stop bits; see SerialStopBits.
 * @param parity   Parity mode; see SerialParity.
 * @return New SerialPort the caller must release with serialPortFree(),
 *         or NULL if the Serial license feature is not enabled, the port
 *         could not be opened, or the settings could not be applied.
 */
CNET_API SerialPort* serialPortOpen(const wchar_t* portName, int baudRate, int dataBits, SerialStopBits stopBits, SerialParity parity);
/**
 * Close the underlying OS handle without freeing the SerialPort struct.
 * Safe to call more than once.
 *
 * @param port Port to close; borrowed.
 */
CNET_API void serialPortClose(SerialPort* port);
/**
 * Close (if still open) and release a SerialPort.
 *
 * @param port Port to free; ownership is taken, do not use it afterward.
 */
CNET_API void serialPortFree(SerialPort* port);

/**
 * Write bytes to the port, blocking until they are sent or the write
 * timeout elapses.
 *
 * @param port   Port to write to; borrowed.
 * @param data   Bytes to write; borrowed.
 * @param length Number of bytes in data.
 * @return Number of bytes actually written, or 0 on failure.
 */
CNET_API int serialPortWrite(SerialPort* port, const void* data, int length);
/**
 * Read bytes from the port, blocking until data arrives or the read
 * timeout elapses.
 *
 * @param port         Port to read from; borrowed.
 * @param buffer       Buffer to receive the data; borrowed.
 * @param bufferLength Capacity of buffer in bytes.
 * @return Number of bytes actually read (0 on timeout with no data), or
 *         0 on failure.
 */
CNET_API int serialPortRead(SerialPort* port, void* buffer, int bufferLength);

/**
 * Set how long serialPortRead() waits for data before giving up.
 *
 * @param port          Port to configure; borrowed.
 * @param timeoutMillis Maximum wait per read, in milliseconds.
 * @return true on success, false on failure.
 */
CNET_API bool serialPortSetReadTimeout(SerialPort* port, int timeoutMillis);
/**
 * Set how long serialPortWrite() waits for the write to complete before
 * giving up.
 *
 * @param port          Port to configure; borrowed.
 * @param timeoutMillis Maximum wait per write, in milliseconds.
 * @return true on success, false on failure.
 */
CNET_API bool serialPortSetWriteTimeout(SerialPort* port, int timeoutMillis);

/**
 * Get the number of bytes currently sitting in the input buffer.
 *
 * @param port Port to query; borrowed.
 * @return Number of bytes available to read, or 0 on failure.
 */
CNET_API int serialPortBytesAvailable(const SerialPort* port);
/**
 * Discard any buffered but not-yet-transmitted/received data in both
 * directions.
 *
 * @param port Port to flush; borrowed.
 * @return true on success, false on failure.
 */
CNET_API bool serialPortFlush(SerialPort* port);

/**
 * Raise or lower the DTR (Data Terminal Ready) line.
 *
 * @param port  Port to control; borrowed.
 * @param value Nonzero to set DTR, 0 to clear it.
 * @return true on success, false on failure.
 */
CNET_API bool serialPortSetDtr(SerialPort* port, int value);
/**
 * Raise or lower the RTS (Request To Send) line.
 *
 * @param port  Port to control; borrowed.
 * @param value Nonzero to set RTS, 0 to clear it.
 * @return true on success, false on failure.
 */
CNET_API bool serialPortSetRts(SerialPort* port, int value);

/**
 * Get the port name it was opened with.
 *
 * @param port Port to query; borrowed.
 * @return Name (e.g. L"COM3") owned by port; valid until serialPortFree().
 */
CNET_API const wchar_t* serialPortGetName(const SerialPort* port);

#endif
