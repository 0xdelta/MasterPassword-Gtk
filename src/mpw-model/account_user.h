//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_ACCOUNT_USER_H
#define MPW_GTK_ACCOUNT_USER_H

#include "user.h"

#include <string>

/**
 * The account user class represents a user, that has a local account
 * with stored sites and master password. Hence, to create an object
 * of this class, a master key id is needed, which is stored in the
 * users config file.
 */
class AccountUser : public User {
private:
    std::unordered_map<std::string, Service> services;

public:
    AccountUser(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion);

    virtual bool isIncognito();
    virtual bool unlockMasterKey(std::string &masterPassword);
    virtual const std::unordered_map<std::string, Service> *getServices() const;
    virtual void addService(Service &service);
    virtual void removeService(std::string serviceName);
};


#endif //MPW_GTK_ACCOUNT_USER_H
