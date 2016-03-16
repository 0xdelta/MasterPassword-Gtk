//
// Created by lukas on 16.03.16.
//

#include <mpw-util.h>
#include "incognito_user.h"

incognito_user::incognito_user(std::string userName) : user(userName) { }

bool incognito_user::isIncognito() {
    return true;
}

bool incognito_user::unlockMasterKey(std::string &masterPassword) {
    masterKey = mpw_masterKeyForUser(userName.c_str(), masterPassword.c_str(), algorithmVersion);
    masterKeyId = mpw_idBytesForBuf(masterKey, MP_dkLen);
    return true;
}

const std::list<mpw_service> &incognito_user::getServices() const {
    // Incognito users can't store sites, so return an empty list
    return std::list<mpw_service>{};
}

void incognito_user::addService(mpw_service &service) { }

void incognito_user::removeService(mpw_service &service) { }