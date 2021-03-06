//
//  mpw-util.h
//  MasterPassword
//
//  Created by Maarten Billemont on 2014-12-20.
//  Copyright (c) 2014 Lyndir. All rights reserved.
//

#ifndef MPW_UTIL_H
#define MPW_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MP_idLen 32
#define MP_macLen 32

#include <stdio.h>
#include <stdint.h>

/** Push a buffer onto a buffer.  reallocs the given buffer and appends the given buffer. */
void mpw_pushBuf(
        uint8_t **const buffer, size_t *const bufferSize, const void *pushBuffer, const size_t pushSize);
/** Push a string onto a buffer.  reallocs the given buffer and appends the given string. */
void mpw_pushString(
        uint8_t **buffer, size_t *const bufferSize, const char *pushString);
/** Push an integer onto a buffer.  reallocs the given buffer and appends the given integer. */
void mpw_pushInt(
        uint8_t **const buffer, size_t *const bufferSize, const uint32_t pushInt);
/** Free a buffer after zero'ing its contents. */
void mpw_free(
        const void *buffer, const size_t bufferSize);
/** Free a string after zero'ing its contents. */
void mpw_freeString(
        const char *string);

//// Cryptographic functions.

/** Perform a scrypt-based key derivation on the given key using the given salt and scrypt parameters.
  * @return A new keySize-size allocated buffer. */
uint8_t const *mpw_scrypt(
        const size_t keySize, const char *secret, const uint8_t *salt, const size_t saltSize,
        uint64_t N, uint32_t r, uint32_t p);
/** Calculate a SHA256-based HMAC by encrypting the given salt with the given key.
  * @return A new 32-byte allocated buffer. */
uint8_t const *mpw_hmac_sha256(
        const uint8_t *key, const size_t keySize, const uint8_t *salt, const size_t saltSize);

//// Visualizers.

/** Encode a buffer as a string of hexadecimal characters.
  * @return A C-string in a reused buffer, do not free or store it. */
const char *mpw_hex(const void *buf, size_t length);
const uint8_t *mpw_hex_reverse(const char *buf, size_t length);
const char *mpw_hex_l(uint32_t number);
/**
 * Encode a fingerprint for a buffer.
 * @return a 32-byte array containing the fingerprint
 */
const uint8_t *mpw_idBytesForBuf(const void *buf, size_t length);
/** Encode a fingerprint for a buffer.
  * @return A C-string in a reused buffer, do not free or store it. */
const char *mpw_idForBuf(const void *buf, size_t length);
/** Encode a visual fingerprint for a user.
  * @return A newly allocated string. */
const char *mpw_identicon(const char *fullName, const char *masterPassword);

//// String utilities.

/** @return The amount of display characters in the given UTF-8 string. */
const size_t mpw_charlen(const char *utf8String);

#ifdef __cplusplus
}
#endif

#endif // MPW_UTIL_H