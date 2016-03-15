//
// Created by lukas on 06.03.16.
//

#ifndef MPW_GTK_MPW_USER_H
#define MPW_GTK_MPW_USER_H

#include <exception>
#include <stdint.h>
#include <vector>
#include <libconfig.h++>
#include "mpw_service.h"

extern "C" {
#include "mpw-algorithm.h"
};

class password_generate_exception : public std::exception {
public:
    virtual const char *what() const throw() {
        return "Exception while generating password";
    }
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

    std::string passwordForService(std::string siteName, MPSiteType siteType, MPAlgorithmVersion version, uint32_t counter) throw(password_generate_exception);
    std::string passwordForService(mpw_service &service) throw(password_generate_exception);

    const std::string &getUserName() const {
        return userName;
    }

    const std::vector<mpw_service> &getServices() const {
        return services;
    }
};


#endif //MPW_GTK_MPW_USER_H
