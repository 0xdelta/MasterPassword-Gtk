//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_INCOGNITO_USER_H
#define MPW_GTK_INCOGNITO_USER_H

#include "user.h"

class IncognitoUser : public User {
public:
    IncognitoUser(std::string userName);

    virtual bool isIncognito();
    virtual bool unlockMasterKey(std::string &masterPassword);
    virtual const std::list<Service> &getServices() const;
    virtual void addService(Service &service);
    virtual void removeService(Service &service);
};


#endif //MPW_GTK_INCOGNITO_USER_H
