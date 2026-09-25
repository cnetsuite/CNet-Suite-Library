#pragma once

#ifndef CNET_CONSOLE_H
#define CNET_CONSOLE_H

#include "CNetGc.h"
#include "CNetApi.h"

/* Values match the Win32 console text-attribute color nibble (0-15). */
typedef enum {
    CONSOLE_COLOR_BLACK = 0,
    CONSOLE_COLOR_DARK_BLUE = 1,
    CONSOLE_COLOR_DARK_GREEN = 2,
    CONSOLE_COLOR_DARK_CYAN = 3,
    CONSOLE_COLOR_DARK_RED = 4,
    CONSOLE_COLOR_DARK_MAGENTA = 5,
    CONSOLE_COLOR_DARK_YELLOW = 6,
    CONSOLE_COLOR_GRAY = 7,
    CONSOLE_COLOR_DARK_GRAY = 8,
    CONSOLE_COLOR_BLUE = 9,
    CONSOLE_COLOR_GREEN = 10,
    CONSOLE_COLOR_CYAN = 11,
    CONSOLE_COLOR_RED = 12,
    CONSOLE_COLOR_MAGENTA = 13,
    CONSOLE_COLOR_YELLOW = 14,
    CONSOLE_COLOR_WHITE = 15
} ConsoleColor;

/* Result of consoleReadKey(). */
typedef struct {
    wchar_t keyChar; /* Translated character, or L'\0' for keys with no character (arrows, F-keys, ...). */
    int keyCode;     /* Win32 virtual-key code (VK_*). */
    bool shift;
    bool alt;
    bool control;
} ConsoleKeyInfo;

/**
 * Write formatted text to the console (or, if standard output is
 * redirected to a file/pipe, as UTF-8 bytes to that stream instead).
 *
 * @param format  wprintf-style format string; borrowed.
 * @param ...     Arguments matching the format string's conversion specifiers.
 */
CNET_API void consoleWrite(const wchar_t* format, ...);
/**
 * Write formatted text to the console followed by a newline. Same
 * redirect-aware behavior as consoleWrite().
 *
 * @param format  wprintf-style format string; borrowed.
 * @param ...     Arguments matching the format string's conversion specifiers.
 */
CNET_API void consoleWriteLine(const wchar_t* format, ...);
/**
 * Clear the console screen and move the cursor to the top-left. Has no
 * effect if standard output is redirected.
 */
CNET_API void consoleClear(void);

/**
 * Read one line of input, waiting for a newline or end of input. Reads
 * from a real console a code unit at a time; falls back to decoding
 * redirected input as UTF-8 when standard input is not a console.
 *
 * @param gc  Arena the returned string is allocated from.
 * @return New wide string owned by gc (without the trailing newline), or
 *         NULL at end of input.
 */
CNET_API wchar_t* consoleReadLine(Gc* gc);
/**
 * Read a single character of input.
 *
 * @return The character read, or -1 at end of input.
 */
CNET_API int consoleReadChar(void);
/**
 * Read a single key press from a real console, without waiting for Enter.
 *
 * @param intercept  If false, the character (if any) is also echoed to the
 *                    console, matching a normal keystroke; if true, nothing
 *                    is echoed.
 * @return Info describing the key pressed. If standard input is not a real
 *         console (e.g. redirected from a file/pipe), returns a
 *         zero-initialized ConsoleKeyInfo and records ErrorInvalidState
 *         (check getLastError()/getLastErrorMessage()).
 */
CNET_API ConsoleKeyInfo consoleReadKey(bool intercept);
/**
 * Check whether a key press is waiting to be read, without blocking.
 *
 * @return true if consoleReadKey() would return immediately, false otherwise.
 */
CNET_API bool consoleKeyAvailable(void);

/**
 * Get the color used for text written from now on.
 *
 * @return The current foreground color.
 */
CNET_API ConsoleColor consoleGetForegroundColor(void);
/**
 * Set the color used for text written from now on. Has no effect if
 * standard output is redirected.
 *
 * @param color  New foreground color.
 */
CNET_API void consoleSetForegroundColor(ConsoleColor color);
/**
 * Get the color used behind text written from now on.
 *
 * @return The current background color.
 */
CNET_API ConsoleColor consoleGetBackgroundColor(void);
/**
 * Set the color used behind text written from now on. Has no effect if
 * standard output is redirected.
 *
 * @param color  New background color.
 */
CNET_API void consoleSetBackgroundColor(ConsoleColor color);
/**
 * Restore the foreground and background colors to what they were the
 * first time this process touched the console's colors (or the console's
 * own defaults if colors were never changed).
 */
CNET_API void consoleResetColor(void);

/**
 * Get the cursor's column position.
 *
 * @return 0-based column, or 0 if it could not be determined (e.g. output redirected).
 */
CNET_API int consoleGetCursorLeft(void);
/**
 * Get the cursor's row position.
 *
 * @return 0-based row, or 0 if it could not be determined (e.g. output redirected).
 */
CNET_API int consoleGetCursorTop(void);
/**
 * Move the cursor. Has no effect if standard output is redirected.
 *
 * @param left  0-based column.
 * @param top   0-based row.
 */
CNET_API void consoleSetCursorPosition(int left, int top);
/**
 * Get whether the cursor is currently drawn.
 *
 * @return true if visible, false if hidden or it could not be determined.
 */
CNET_API bool consoleGetCursorVisible(void);
/**
 * Show or hide the cursor. Has no effect if standard output is redirected.
 *
 * @param visible  true to show the cursor, false to hide it.
 */
CNET_API void consoleSetCursorVisible(bool visible);
/**
 * Get the visible console window's width.
 *
 * @return Width in character columns, or 0 if it could not be determined.
 */
CNET_API int consoleGetWindowWidth(void);
/**
 * Get the visible console window's height.
 *
 * @return Height in character rows, or 0 if it could not be determined.
 */
CNET_API int consoleGetWindowHeight(void);
/**
 * Get the console screen buffer's total width (may exceed the visible window).
 *
 * @return Width in character columns, or 0 if it could not be determined.
 */
CNET_API int consoleGetBufferWidth(void);
/**
 * Get the console screen buffer's total height (may exceed the visible window).
 *
 * @return Height in character rows, or 0 if it could not be determined.
 */
CNET_API int consoleGetBufferHeight(void);

/**
 * Get the console window's title.
 *
 * @param gc  Arena the returned string is allocated from.
 * @return New wide string owned by gc; empty string if it could not be determined.
 */
CNET_API wchar_t* consoleGetTitle(Gc* gc);
/**
 * Set the console window's title.
 *
 * @param title  New title text; borrowed.
 */
CNET_API void consoleSetTitle(const wchar_t* title);

/**
 * Sound the console beep at its default pitch/duration (800Hz for 200ms).
 */
CNET_API void consoleBeep(void);
/**
 * Sound the console beep at a specific pitch/duration.
 *
 * @param frequency      Tone frequency in hertz (37-32767).
 * @param durationMillis  How long to sound the tone, in milliseconds.
 */
CNET_API void consoleBeepEx(int frequency, int durationMillis);

/**
 * Check whether standard output has been redirected to a file or pipe
 * rather than a real console.
 *
 * @return true if redirected, false if it is a real console.
 */
CNET_API bool consoleIsOutputRedirected(void);
/**
 * Check whether standard input has been redirected from a file or pipe
 * rather than a real console.
 *
 * @return true if redirected, false if it is a real console.
 */
CNET_API bool consoleIsInputRedirected(void);

#endif
