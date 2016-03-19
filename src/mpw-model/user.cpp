//
// Created by lukas on 16.03.16.
//

#include "user.h"

#include <iostream>
#include <string.h>

User::User(std::string _userName) :
        userName(_userName), algorithmVersion(MPAlgorithmVersionCurrent) { }

User::User(std::string _userName, const uint8_t *_masterKeyId, MPAlgorithmVersion _algorithmVersion) :
        userName(_userName), masterKeyId(_masterKeyId), algorithmVersion(_algorithmVersion) { }

User::~User() {
    if (masterKey) {
        // Override the master key for security reasons
        memset((void *) masterKey, 0, MP_dkLen);
        delete masterKey;
    }
    if (masterKeyId) {
        delete masterKeyId;
    }
}

std::string User::passwordForService(std::string &siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter) {
    const char *res = mpw_passwordForSite(masterKey, siteName.c_str(), siteType, counter, MPSiteVariantPassword, NULL, version);
    if (res == NULL) {
        std::cerr << "mpw_passwordForSite returned NULL" << std::endl;
        //throw password_generate_exception{};
    }
    return std::string{res};
}

std::string User::passwordForService(Service &service) {
    return passwordForService((std::string &) service.getName(), service.getType(), service.getAlgorithmVersion(), (uint32_t) service.getCounter());
}


