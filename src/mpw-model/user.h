//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_USER_H
#define MPW_GTK_USER_H

#include <string>
#include <unordered_map>
#include "mpw-algorithm.h"
#include "service.h"

class User {
protected:
    const std::string userName;
    const uint8_t *masterKey = nullptr;
    const uint8_t *masterKeyId = nullptr;
    MPAlgorithmVersion algorithmVersion;

protected:
    User(std::string userName);
    User(std::string userName, const uint8_t *masterKeyId, MPAlgorithmVersion algorithmVersion);

public:
    virtual ~User();

public:
    std::string passwordForService(std::string &siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter);
    std::string passwordForService(Service &service);

    virtual bool isIncognito() = 0;
    virtual bool unlockMasterKey(std::string &masterPassword) = 0;
    virtual const std::unordered_map<std::string, Service> &getServices() const = 0;
    virtual void addService(Service &service) = 0;
    virtual void removeService(std::string serviceName) = 0;

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
