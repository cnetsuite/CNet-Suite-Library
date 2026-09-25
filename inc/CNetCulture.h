#pragma once

#ifndef CNET_CULTURE_H
#define CNET_CULTURE_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sCulture Culture;

/* localeName is a BCP-47 tag such as L"en-US"; NULL or L"" resolves to the
   user's current default locale via GetUserDefaultLocaleName. A non-empty
   name is validated with IsValidLocaleName and asserts on failure. */
/**
 * Create a Culture describing a locale's identity and formatting
 * conventions (decimal/thousand separators, currency symbol, date/time
 * patterns), read from the Windows locale database. On Linux/macOS, read
 * from ICU (the project's cross-platform substitute for Windows NLS) instead
 * — see Culture.posix.c's header comment for the handful of documented
 * NLS-vs-ICU/CLDR formatting differences this maps around.
 *
 * @param gc          Arena the returned Culture is allocated from.
 * @param localeName  BCP-47 locale tag (e.g. L"de-DE"); NULL or L"" resolves
 *                     to the user's current default locale.
 * @return New Culture owned by gc, or NULL if localeName is a non-empty
 *         string that is not a recognized locale name (check
 *         getLastError()/getLastErrorMessage()).
 */
CNET_API Culture* cultureCreate(Gc* gc, const wchar_t* localeName);
/**
 * Get a Culture for the user's current default locale.
 * Equivalent to cultureCreate(gc, NULL).
 *
 * @param gc  Arena the returned Culture is allocated from.
 * @return New Culture owned by gc.
 */
CNET_API Culture* cultureGetCurrent(Gc* gc); /* = cultureCreate(gc, NULL) */

/**
 * Get the culture's BCP-47 locale name.
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture; valid as long as
 *         culture is (do not free separately).
 */
CNET_API const wchar_t* cultureGetName(const Culture* culture);

/**
 * Get the culture's two-letter ISO 639 language code (e.g. "en").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetIsoLanguageCode(const Culture* culture);

/**
 * Get the culture's ISO 3166 country/region code (e.g. "US").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetIsoCountryCode(const Culture* culture);

/**
 * Get the culture's display name in its own language (e.g. "Deutsch (Deutschland)").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetNativeDisplayName(const Culture* culture);

/**
 * Get the culture's display name in English (e.g. "German (Germany)").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetEnglishDisplayName(const Culture* culture);

/**
 * Get the culture's decimal separator (e.g. "." or ",").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetDecimalSeparator(const Culture* culture);

/**
 * Get the culture's digit grouping (thousands) separator.
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetThousandSeparator(const Culture* culture);

/**
 * Get the culture's currency symbol (e.g. "$", "€").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetCurrencySymbol(const Culture* culture);

/**
 * Get the culture's short date format pattern (e.g. "M/d/yyyy").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetShortDatePattern(const Culture* culture);

/**
 * Get the culture's long date format pattern (e.g. "dddd, MMMM d, yyyy").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetLongDatePattern(const Culture* culture);

/**
 * Get the culture's time format pattern (e.g. "h:mm:ss tt").
 *
 * @param culture  Culture to read; borrowed, not modified.
 * @return NUL-terminated string owned by culture.
 */
CNET_API const wchar_t* cultureGetTimePattern(const Culture* culture);

/* decimalDigits < 0 uses the locale's own default digit count/rules
   (lpFormat = NULL); decimalDigits >= 0 forces that many fraction digits
   via a NUMBERFMTW override, keeping separators/grouping from the culture. */
/**
 * Format a number using the culture's decimal/grouping conventions.
 *
 * @param gc             Arena the returned string is allocated from.
 * @param culture        Culture whose formatting conventions to use; borrowed.
 * @param value          Number to format.
 * @param decimalDigits  Fraction digits to force; -1 uses the locale's
 *                        own default digit count/rules instead.
 * @return New NUL-terminated string owned by gc, or NULL on formatting
 *         failure (check getLastError()/getLastErrorMessage()).
 */
CNET_API wchar_t* cultureFormatNumber(Gc* gc, const Culture* culture, double value, int decimalDigits);
/**
 * Format an amount using the culture's currency conventions (symbol,
 * placement, decimal digits, grouping).
 *
 * @param gc       Arena the returned string is allocated from.
 * @param culture  Culture whose currency conventions to use; borrowed.
 * @param value    Amount to format.
 * @return New NUL-terminated string owned by gc, or NULL on formatting
 *         failure (check getLastError()/getLastErrorMessage()).
 */
CNET_API wchar_t* cultureFormatCurrency(Gc* gc, const Culture* culture, double value);

#endif
