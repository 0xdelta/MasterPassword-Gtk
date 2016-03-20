//
// Created by lukas on 16.03.16.
//

#include <mpw-util.h>
#include "incognito_user.h"

IncognitoUser::IncognitoUser(std::string userName) : User(userName) { }

bool IncognitoUser::isIncognito() {
    return true;
}

bool IncognitoUser::unlockMasterKey(std::string &masterPassword) {
    masterKey = mpw_masterKeyForUser(userName.c_str(), masterPassword.c_str(), algorithmVersion);
    masterKeyId = mpw_idBytesForBuf(masterKey, MP_dkLen);
    return true;
}

const std::unordered_map<std::string, Service> *IncognitoUser::getServices() const {
    // Incognito users can't store sites, so return a null pointer
    return NULL;
}

void IncognitoUser::addService(Service &service) { }

void IncognitoUser::removeService(std::string serviceName) { }