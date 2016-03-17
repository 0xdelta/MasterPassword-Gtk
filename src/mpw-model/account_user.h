//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_ACCOUNT_USER_H
#define MPW_GTK_ACCOUNT_USER_H

#include "user.h"

#include <string>

class AccountUser : public User {
private:
    std::list<Service> services;

public:
    AccountUser(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion);

    // Methods from user class
    virtual bool isIncognito();
    virtual bool unlockMasterKey(std::string &masterPassword);
    virtual const std::list<Service> &getServices() const;
    virtual void addService(Service &service);
    virtual void removeService(Service &service);
};


#endif //MPW_GTK_ACCOUNT_USER_H
