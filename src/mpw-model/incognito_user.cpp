//
// Created by lukas on 16.03.16.
//

#include <mpw-util.h>
#include "incognito_user.h"

incognito_user::incognito_user(std::string userName) : user(userName) { }

bool user::unlockMasterKey(std::string &masterPassword) {
    masterKey = mpw_masterKeyForUser(userName.c_str(), masterPassword.c_str(), algorithmVersion);
    masterKeyId = mpw_idBytesForBuf(masterKey, MP_dkLen);
    return true;
}

bool user::isIncognito() {
    return true;
}

const std::list<mpw_service> &user::getServices() const {
    // Incognito users can't store sites, so return an empty list
    return std::list<mpw_service>{};
}

void user::addService(mpw_service &service) { }

void user::removeService(mpw_service &service) { }