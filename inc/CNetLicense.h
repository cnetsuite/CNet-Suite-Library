#pragma once

#ifndef CNET_LICENSE_H
#define CNET_LICENSE_H

#include "CNetGc.h"
#include "CNetRsa.h"
#include "CNetDateTime.h"
#include "CNetApi.h"

typedef enum {
    LICENSE_FEATURE_1   = 1 << 0,
    LICENSE_FEATURE_2   = 1 << 1,
    LICENSE_FEATURE_3   = 1 << 2,
    LICENSE_FEATURE_4   = 1 << 3,
    LICENSE_FEATURE_5   = 1 << 4,
    LICENSE_FEATURE_6   = 1 << 5,
    LICENSE_FEATURE_ALL = 0x7F    /* Bitwise OR of every individual feature flag above. */
} LicenseFeature;

typedef struct sLicense License;

/**
 * Build a signed license document as JSON text: a payload (licensee,
 * expiry, features) plus a base64-encoded RSA-PSS signature over that
 * payload, produced with the issuer's private key.
 *
 * @param gc              Arena the returned text is allocated from;
 *                         borrowed.
 * @param privateKeyPair  Issuer's key pair; its private key is used to
 *                         sign the payload; borrowed.
 * @param licensee        Name of the licensed party; borrowed, copied.
 * @param expiry          Expiry date/time, or NULL for a perpetual
 *                         license that never expires; borrowed.
 * @param features        Bitmask of LicenseFeature values to grant.
 * @return Newly allocated license document text, owned by gc.
 */
CNET_API wchar_t* licenseIssue(Gc* gc, RsaKeyPair* privateKeyPair, const wchar_t* licensee, const DateTime* expiry, unsigned int features);
/**
 * Parse a license document produced by licenseIssue() and check its
 * signature against the issuer's public key. Allocates its own internal
 * arena independent of any caller-supplied Gc; the resulting License must
 * be released with licenseFree(). A malformed/invalid signature does not
 * prevent parsing -- check licenseIsSignatureValid()/licenseIsValid() on
 * the result.
 *
 * @param licenseText     License document text, as produced by
 *                        licenseIssue(); borrowed.
 * @param publicKeyPair   Issuer's key pair (public key is sufficient) used
 *                        to verify the signature; borrowed.
 * @return Newly created License, owning its own arena; free it with
 *         licenseFree(). NULL if licenseText is not well-formed JSON or is
 *         missing required fields.
 */
CNET_API License* licenseParse(const wchar_t* licenseText, RsaKeyPair* publicKeyPair);
/**
 * Read a license document from a file and parse it as licenseParse()
 * does. The file's bytes on disk are UTF-8 encoded, as with File's own
 * text I/O.
 *
 * @param path           Path to the license file; borrowed.
 * @param publicKeyPair  Issuer's key pair (public key is sufficient) used
 *                       to verify the signature; borrowed.
 * @return Newly created License, owning its own arena; free it with
 *         licenseFree(). NULL if the file cannot be opened/read or its
 *         contents are not a well-formed license document.
 */
CNET_API License* licenseLoadFromFile(const wchar_t* path, RsaKeyPair* publicKeyPair);
/**
 * Free a License and its internal arena. If this license is currently the
 * process-wide installed license (see licenseInstall()), it is uninstalled
 * first.
 *
 * @param license  License to free; caller must not use it afterward.
 */
CNET_API void licenseFree(License* license);

/**
 * Get the licensee name recorded in a license.
 *
 * @param license  License to read; borrowed.
 * @return Licensee name, owned by the license; valid until the license is
 *         freed.
 */
CNET_API const wchar_t* licenseGetLicensee(const License* license);
/**
 * Get the expiry date/time recorded in a license.
 *
 * @param license  License to read; borrowed.
 * @return Expiry date/time, owned by the license and valid until it is
 *         freed; or NULL if the license is perpetual (never expires).
 */
CNET_API DateTime* licenseGetExpiry(const License* license);
/**
 * Check whether a license's RSA signature was found valid when it was
 * parsed.
 *
 * @param license  License to check; borrowed.
 * @return Nonzero if the signature is valid; 0 if it is invalid or was
 *         signed/verified with a mismatched key.
 */
CNET_API int licenseIsSignatureValid(const License* license);
/**
 * Check whether a license's expiry date/time is in the past.
 *
 * @param license  License to check; borrowed.
 * @return Nonzero if the license has an expiry date and it is before the
 *         current time; 0 if not expired or if the license is perpetual
 *         (no expiry set).
 */
CNET_API int licenseIsExpired(const License* license);
/**
 * Check whether a license is currently usable: its signature is valid and
 * it has not expired.
 *
 * @param license  License to check; borrowed.
 * @return Nonzero if the license is valid and unexpired; 0 otherwise.
 */
CNET_API int licenseIsValid(const License* license);
/**
 * Check whether a license's feature bitmask grants a given feature.
 *
 * @param license  License to check; borrowed.
 * @param feature  Single LicenseFeature bit to test for.
 * @return Nonzero if license's feature bitmask includes feature; 0
 *         otherwise. Does not check signature validity or expiry.
 */
CNET_API int licenseHasFeature(const License* license, LicenseFeature feature);

/**
 * Install a license as the process-wide active license, used by
 * licenseRequire() (and in turn by feature-gated functions such as
 * aesEncrypt() or socketCreate()) to authorize use of licensed features.
 *
 * @param license  License to install; borrowed by the process-wide
 *                 installed-license slot. Caller retains ownership and
 *                 must keep it alive (and eventually free it with
 *                 licenseFree()) for as long as it stays installed.
 */
CNET_API void licenseInstall(License* license);
/**
 * Get the currently installed process-wide license, if any.
 *
 * @return The license passed to the most recent licenseInstall() call; or
 *         NULL if no license is installed (or it has since been freed).
 */
CNET_API License* licenseGetInstalled(void);
/**
 * Assert that the installed license grants a given feature: it must be
 * installed, valid (signature valid and unexpired), and include the
 * feature in its bitmask. Records a process error describing which check
 * failed when it returns 0.
 *
 * @param feature  Single LicenseFeature bit that must be granted.
 * @return 1 if the installed license grants the feature; 0 otherwise (no
 *         license installed, license invalid/expired, or feature not
 *         granted).
 */
CNET_API int licenseRequire(LicenseFeature feature);

#endif
