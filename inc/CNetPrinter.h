#pragma once

#ifndef CNET_PRINTER_H
#define CNET_PRINTER_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sPrinter Printer;

/**
 * List the names of all installed local and connected printers. On
 * Linux/macOS, backed by CUPS's cupsGetDests() rather than the Windows
 * print spooler; everything past printerOpen() that would need real page
 * rendering is out of scope there and returns a documented
 * ErrorUnsupported failure instead — see Printer.posix.c's header comment
 * for the agreed minimal-parity scope.
 *
 * @param outCount Set to the number of names returned (0 on failure).
 * @return Newly malloc'd array of malloc'd wide name strings the caller
 *         must free (each string, then the array), or NULL on failure.
 */
CNET_API wchar_t** printerListNames(int* outCount);
/**
 * Get the name of the system's default printer.
 *
 * @return Newly malloc'd wide string the caller must free() , or NULL if
 *         there is no default printer.
 */
CNET_API wchar_t* printerGetDefaultName(void);

/**
 * Open a device context for a printer, reading its page size and DPI.
 *
 * @param printerName Name of an installed printer (as returned by
 *                    printerListNames()/printerGetDefaultName()); borrowed.
 * @return New Printer the caller must release with printerFree(), or NULL
 *         if the Printer license feature is not enabled or the printer
 *         could not be opened.
 */
CNET_API Printer* printerOpen(const wchar_t* printerName);
/**
 * Release a printer's device context, font, and resources.
 *
 * @param printer Printer to free; ownership is taken, do not use it afterward.
 */
CNET_API void printerFree(Printer* printer);

/**
 * Get the printer name it was opened with.
 *
 * @param printer Printer to query; borrowed.
 * @return UTF-8 name owned by printer; valid until printerFree().
 */
CNET_API const wchar_t* printerGetName(const Printer* printer);
/**
 * Get the printable page width.
 *
 * @param printer Printer to query; borrowed.
 * @return Page width in device pixels (per the printer's current DPI/paper settings).
 */
CNET_API int printerGetPageWidth(const Printer* printer);
/**
 * Get the printable page height.
 *
 * @param printer Printer to query; borrowed.
 * @return Page height in device pixels (per the printer's current DPI/paper settings).
 */
CNET_API int printerGetPageHeight(const Printer* printer);

/**
 * Begin a new print job.
 *
 * @param printer      Printer to start a document on; borrowed.
 * @param documentName Name shown for the job in the print queue; borrowed.
 * @param outputPath   Path to redirect output to a file instead of the
 *                      physical printer (e.g. for a "Print to PDF"-style
 *                      driver), or NULL to print normally; borrowed.
 * @return true on success, false on failure.
 */
CNET_API bool printerStartDocument(Printer* printer, const wchar_t* documentName, const wchar_t* outputPath);
/**
 * Begin a new page within the current document.
 *
 * @param printer Printer to start a page on; borrowed.
 * @return true on success, false on failure.
 */
CNET_API bool printerStartPage(Printer* printer);
/**
 * Create and select a font to use for subsequent printerDrawText() calls,
 * replacing any previously selected font.
 *
 * @param printer      Printer to set the font on; borrowed.
 * @param faceName     Font face name (e.g. "Arial"); borrowed.
 * @param heightPoints Font size in points; converted to device pixels
 *                      using the printer's vertical DPI.
 * @param bold         Nonzero for bold weight, 0 for normal weight.
 * @return true on success, false if the font could not be created.
 */
CNET_API bool printerSetFont(Printer* printer, const wchar_t* faceName, int heightPoints, int bold);
/**
 * Draw a line of text at a position on the current page, using the
 * currently selected font.
 *
 * @param printer Printer to draw on; borrowed.
 * @param x       X coordinate in device pixels.
 * @param y       Y coordinate in device pixels.
 * @param text    Text to draw; borrowed.
 * @return true on success, false on failure.
 */
CNET_API bool printerDrawText(Printer* printer, int x, int y, const wchar_t* text);
/**
 * End the current page.
 *
 * @param printer Printer to end the page on; borrowed.
 * @return true on success, false on failure.
 */
CNET_API bool printerEndPage(Printer* printer);
/**
 * End the current document, submitting it to the printer/spooler.
 *
 * @param printer Printer to end the document on; borrowed.
 * @return true on success, false on failure.
 */
CNET_API bool printerEndDocument(Printer* printer);
/**
 * Abort the current document, discarding any pages printed so far.
 *
 * @param printer Printer whose document should be cancelled; borrowed.
 */
CNET_API void printerCancelDocument(Printer* printer);

#endif
