//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_INCOGNITO_USER_H
#define MPW_GTK_INCOGNITO_USER_H

#include "user.h"

class incognito_user : public user {
public:
    incognito_user(std::string userName);

    virtual bool isIncognito();
    virtual bool unlockMasterKey(std::string &masterPassword);
    virtual const std::list<mpw_service> &getServices() const;
    virtual void addService(mpw_service &service);
    virtual void removeService(mpw_service &service);
};


#endif //MPW_GTK_INCOGNITO_USER_H
