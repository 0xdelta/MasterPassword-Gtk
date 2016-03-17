//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_USER_H
#define MPW_GTK_USER_H

#include <string>
#include <list>
#include "mpw-algorithm.h"
#include "mpw_service.h"

class user {
protected:
    const std::string userName;
    const uint8_t *masterKey = nullptr;
    const uint8_t *masterKeyId = nullptr;
    MPAlgorithmVersion algorithmVersion;

protected:
    user(std::string userName);
    user(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion);

public:
    virtual ~user();

public:
    std::string passwordForService(std::string &siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter);

    virtual bool isIncognito() = 0;
    virtual bool unlockMasterKey(std::string &masterPassword) = 0;
    virtual const std::list<mpw_service> &getServices() const = 0;
    virtual void addService(mpw_service &service) = 0;
    virtual void removeService(mpw_service &service) = 0;

    const std::string &getUserName() const {
        return userName;
    }

    const uint8_t *getMasterKeyId() const {
        return masterKeyId;
    }

    MPAlgorithmVersion getAlgorithmVersion() const {
        return algorithmVersion;
    }
};


#endif //MPW_GTK_USER_H