//
// Created by lukas on 16.03.16.
//

#include "user.h"

#include <iostream>
#include <string.h>

user::user(std::string _userName) :
        userName(_userName), algorithmVersion(MPAlgorithmVersionCurrent) { }

user::user(std::string _userName, const uint8_t *_masterKeyId, MPAlgorithmVersion _algorithmVersion) :
        userName(_userName), masterKeyId(_masterKeyId), algorithmVersion(_algorithmVersion) { }

user::~user() {
    if (masterKey) {
        // Override the master key for security reasons
        memset((void *) masterKey, 0, MP_dkLen);
        delete masterKey;
    }
    if (masterKeyId) {
        delete masterKeyId;
    }
}

std::string user::passwordForService(std::string &siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter) {
    const char *res = mpw_passwordForSite(masterKey, siteName.c_str(), siteType, counter, MPSiteVariantPassword, NULL, version);
    if (res == NULL) {
        std::cerr << "mpw_passwordForSite returned NULL" << std::endl;
        //throw password_generate_exception{};
    }
    return std::string{res};
}
