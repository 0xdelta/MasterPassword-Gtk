//
// Created by lukas on 06.03.16.
//

#ifndef MPW_GTK_MPW_USER_H
#define MPW_GTK_MPW_USER_H

#include <stdint.h>
#include <vector>
#include <libconfig.h++>
#include "mpw_service.h"

extern "C" {
#include "mpw-algorithm.h"
};

class mpw_user {
private:
    const std::string userName;
    const uint8_t *masterKey = nullptr, *keyId = nullptr;
    MPAlgorithmVersion algorithmVersion;

    std::vector<mpw_service> services;

public:
    mpw_user(std::string &userName);
    ~mpw_user();

    std::string getConfigFileName();
    bool hasConfigFile();
    void readFromConfig();
    void loadDefaults();
    void writeToConfig();
    bool unlockMasterKey(std::string &masterPassword);

    std::string passwordForService(std::string siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter);
    std::string passwordForService(mpw_service &service);

    const std::string &getUserName() const {
        return userName;
    }

    const std::vector<mpw_service> &getServices() const {
        return services;
    }
};


#endif //MPW_GTK_MPW_USER_H
