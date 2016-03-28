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
    /**
     * Generates the password for the given
     * - site name
     * - site type
     * - algorithm version
     * - counter
     */
    std::string passwordForService(std::string &siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter);
    /**
     * Calls the previous function with the encapsulated values
     * in the Service class.
     */
    std::string passwordForService(Service &service);

    /**
     * To be implemented function, that determines, whether this
     * user instance is an incognito instance.
     */
    virtual bool isIncognito() = 0;
    /**
     * To be implemented function, that unlocks the master key.
     * Returns true on success.
     */
    virtual bool unlockMasterKey(std::string &masterPassword) = 0;
    /**
     * To be implemented function, that returns a pointer to the services,
     * this user has stored. Can return null, if this user is incognito.
     */
    virtual const std::unordered_map<std::string, Service> *getServices() const = 0;
    /**
     * Adds a service to the stored services.
     * @param service the service instance to add.
     */
    virtual void addService(Service &service) = 0;
    /**
     * Removes a service from the stored services.
     * @param name the service name to remove.
     */
    virtual void removeService(std::string serviceName) = 0;

    /* Getters */

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
