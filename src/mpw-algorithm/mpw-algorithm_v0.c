//
//  mpw-algorithm.c
//  MasterPassword
//
//  Created by Maarten Billemont on 2014-12-20.
//  Copyright (c) 2014 Lyndir. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include "mpw-util.h"
#include "mpw-types.h"

#define MP_N                32768
#define MP_r                8
#define MP_p                2

static const char *mpw_templateForType_v0(MPSiteType type, uint16_t seedByte) {

    size_t count = 0;
    const char **templates = mpw_templatesForType( type, &count );
    if (!count)
        return NULL;

    return templates[seedByte % count];
}

static const char mpw_characterFromClass_v0(char characterClass, uint16_t seedByte) {

    const char *classCharacters = mpw_charactersInClass( characterClass );
    return classCharacters[seedByte % strlen( classCharacters )];
}

static const uint8_t *mpw_masterKeyForUser_v0(const char *fullName, const char *masterPassword) {

    const char *mpKeyScope = mpw_scopeForVariant( MPSiteVariantPassword );
    // Calculate the master key salt.
    // masterKeySalt = mpKeyScope . #fullName . fullName
    size_t masterKeySaltSize = 0;
    uint8_t *masterKeySalt = NULL;
    mpw_pushString( &masterKeySalt, &masterKeySaltSize, mpKeyScope );
    mpw_pushInt( &masterKeySalt, &masterKeySaltSize, htonl( mpw_charlen( fullName ) ) );
    mpw_pushString( &masterKeySalt, &masterKeySaltSize, fullName );
    if (!masterKeySalt) {
        return NULL;
    }

    // Calculate the master key.
    // masterKey = scrypt( masterPassword, masterKeySalt )
    const uint8_t *masterKey = mpw_scrypt( MP_dkLen, masterPassword, masterKeySalt, masterKeySaltSize, MP_N, MP_r, MP_p );
    mpw_free( masterKeySalt, masterKeySaltSize );
    if (!masterKey) {
        return NULL;
    }

    return masterKey;
}

static const char *mpw_passwordForSite_v0(const uint8_t *masterKey, const char *siteName, const MPSiteType siteType, const uint32_t siteCounter,
        const MPSiteVariant siteVariant, const char *siteContext) {

    const char *siteScope = mpw_scopeForVariant( siteVariant );

    // Calculate the site seed.
    // sitePasswordSeed = hmac-sha256( masterKey, siteScope . #siteName . siteName . siteCounter . #siteContext . siteContext )
    size_t sitePasswordInfoSize = 0;
    uint8_t *sitePasswordInfo = NULL;
    mpw_pushString( &sitePasswordInfo, &sitePasswordInfoSize, siteScope );
    mpw_pushInt( &sitePasswordInfo, &sitePasswordInfoSize, htonl( mpw_charlen( siteName ) ) );
    mpw_pushString( &sitePasswordInfo, &sitePasswordInfoSize, siteName );
    mpw_pushInt( &sitePasswordInfo, &sitePasswordInfoSize, htonl( siteCounter ) );
    if (siteContext) {
        mpw_pushInt( &sitePasswordInfo, &sitePasswordInfoSize, htonl( mpw_charlen( siteContext ) ) );
        mpw_pushString( &sitePasswordInfo, &sitePasswordInfoSize, siteContext );
    }
    if (!sitePasswordInfo) {
        return NULL;
    }

    const char *sitePasswordSeed = (const char *)mpw_hmac_sha256( masterKey, MP_dkLen, sitePasswordInfo, sitePasswordInfoSize );
    mpw_free( sitePasswordInfo, sitePasswordInfoSize );
    if (!sitePasswordSeed) {
        return NULL;
    }

    // Determine the template.
    const char *template = mpw_templateForType_v0( siteType, htons( sitePasswordSeed[0] ) );
    if (strlen( template ) > 32) {
        mpw_free( sitePasswordSeed, sizeof( sitePasswordSeed ) );
        return NULL;
    }

    // Encode the password from the seed using the template.
    char *const sitePassword = calloc( strlen( template ) + 1, sizeof( char ) );
    for (size_t c = 0; c < strlen( template ); ++c) {
        sitePassword[c] = mpw_characterFromClass_v0( template[c], htons( sitePasswordSeed[c + 1] ) );
    }
    mpw_free( sitePasswordSeed, sizeof( sitePasswordSeed ) );

    return sitePassword;
}
