//
// Created by lukas on 16.03.16.
//

#include <mpw-util.h>
#include <string.h>
#include "account_user.h"

account_user::account_user(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion) :
        user(userName, masterKeyId, algorithmVersion) {
}

bool account_user::isIncognito() {
    return false;
}

bool account_user::unlockMasterKey(std::string &masterPassword) {
    // Check if the master key id exists. If not, it is not possible
    // to unlock the master key.
    if (!masterKeyId) {
        return false;
    }

    const uint8_t *tmpKey = mpw_masterKeyForUser(userName.c_str(), masterPassword.c_str(), algorithmVersion);
    const uint8_t *tmpId = mpw_idBytesForBuf(tmpKey, MP_dkLen);

    // Compare the ids
    int cmp = memcmp(masterKeyId, tmpId, 32);

    delete tmpId;

    if (cmp == 0) {
        masterKey = tmpKey;
        return true;
    }

    // Error: Key ids not equal
    return false;
}

const std::list<mpw_service> &account_user::getServices() const {
    return services;
}

void account_user::addService(mpw_service &service) {
    services.push_back(mpw_service{service});
}

void account_user::removeService(mpw_service &service) {
    services.remove(service);
}
