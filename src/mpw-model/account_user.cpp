//
// Created by lukas on 16.03.16.
//

#include <mpw-util.h>
#include <string.h>
#include "account_user.h"

AccountUser::AccountUser(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion) :
        User(userName, masterKeyId, algorithmVersion) {
}

bool AccountUser::isIncognito() {
    return false;
}

bool AccountUser::unlockMasterKey(std::string &masterPassword) {
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

const std::list<Service> &AccountUser::getServices() const {
    return services;
}

void AccountUser::addService(Service &service) {
    services.push_back(Service{service});
}

void AccountUser::removeService(Service &service) {
    services.remove(service);
}
