//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_ACCOUNT_USER_H
#define MPW_GTK_ACCOUNT_USER_H

#include "user.h"

#include <string>

class account_user : public user {
private:
    std::list<mpw_service> services;

public:
    account_user(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion);

    // Methods from user class
    virtual bool isIncognito();
    virtual bool unlockMasterKey(std::string &masterPassword);
    virtual const std::list<mpw_service> &getServices() const;
    virtual void addService(mpw_service &service);
    virtual void removeService(mpw_service &service);
};


#endif //MPW_GTK_ACCOUNT_USER_H
