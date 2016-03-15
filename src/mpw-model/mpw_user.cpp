//
// Created by lukas on 06.03.16.
//

#include <cstring>
#include <iostream>
#include <libconfig.h++>
#include "mpw_user.h"

extern "C" {
#include "mpw-util.h"
};

mpw_user::mpw_user(std::string &_userName) :
        userName(_userName) { }

mpw_user::~mpw_user() {
    if (masterKey) {
        // Override the master key for security reasons
        memset((void *) masterKey, 0, MP_dkLen);
        delete masterKey;
    }
    if (keyId) {
        delete keyId;
    }
}

bool mpw_user::unlockMasterKey(std::string &masterPassword) {
    const uint8_t *tmpKey = mpw_masterKeyForUser(userName.c_str(), masterPassword.c_str(), algorithmVersion);
    if (keyId) {
        // Key id found, check for correct master password
        const uint8_t *tmpId = mpw_idBytesForBuf(tmpKey, MP_dkLen);

        // Compare the ids
        int cmp = memcmp(keyId, tmpId, 32);

        delete tmpId;

        if (cmp == 0) {
            masterKey = tmpKey;
            return true;
        }

        // Error: Key ids not equal
        return false;
    } else {
        // No key id found, use the new master key and generate the key id
        masterKey = tmpKey;
        keyId = mpw_idBytesForBuf(masterKey, MP_dkLen);
        return true;
    }
}

std::string mpw_user::getConfigFileName() {
    return std::string(getenv("HOME")) + "/.mpw/" + userName + ".cfg";
}

bool mpw_user::hasConfigFile() {
    if (FILE *file = fopen(getConfigFileName().c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void mpw_user::readFromConfig() {
    using namespace libconfig;
    FILE *configFile = fopen(getConfigFileName().c_str(), "r");

    if (!configFile) {
        // No configuration for this user found. Load the defaults.
        loadDefaults();
        return;
    }

    Config config;

    try {
        config.read(configFile);
        fclose(configFile);
    } catch (ParseException &e) {
        std::cerr << "Could not read config: " << e.what() << "(" << e.getError() << ")" << std::endl;
        fclose(configFile);
        return;
    }

    int configVersion;
    try {
        configVersion = config.lookup("configVersion");
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read config version: " << e.what() << std::endl;
        return;
    }

    try {
        if (configVersion == 1) {
            std::string keyIdString = config.lookup("keyId");
            if (keyIdString.size() > 0) {
                keyId = mpw_hex_reverse(keyIdString.c_str(), keyIdString.size());
            }
            algorithmVersion = (MPAlgorithmVersion) ((int) config.lookup("algorithmVersion"));

            Setting &servicesSetting = config.lookup("services");
            for (int i = 0; i < servicesSetting.getLength(); ++i) {
                Setting &serviceSetting = servicesSetting[i];

                std::string name;
                int type, algorithmVersion, counter;
                if (!(serviceSetting.lookupValue("name", name) &&
                      serviceSetting.lookupValue("type", type) &&
                      serviceSetting.lookupValue("algorithmVersion", algorithmVersion) &&
                      serviceSetting.lookupValue("counter", counter))) {
                    // Ignore invalid
                    continue;
                }

                mpw_service service = mpw_service{name, (MPSiteType) type, (MPAlgorithmVersion) algorithmVersion, counter};
                services.push_back(service);
            }

            std::cout << "Success reading config." << std::endl;
        } else {
            std::cerr << "Unknown config version: " << configVersion << std::endl;
        }
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read config: " << e.what() << " (" << e.getPath() << ")" << std::endl;
    }
}

void mpw_user::loadDefaults() {
    algorithmVersion = MPAlgorithmVersionCurrent;
}

void mpw_user::writeToConfig() {
    using namespace libconfig;

    // Create a config object and insert the values
    Config config;
    config.getRoot().add("configVersion", Setting::Type::TypeInt) = 1;
    config.getRoot().add("keyId", Setting::Type::TypeString) = keyId ? mpw_hex(keyId, 32) : "";
    config.getRoot().add("algorithmVersion", Setting::Type::TypeInt) = (int) algorithmVersion;

    Setting &servicesSetting = config.getRoot().add("services", Setting::Type::TypeList);
    for (auto &service : services) {
        Setting &serviceSetting = servicesSetting.add(Setting::Type::TypeGroup);
        serviceSetting.add("name", Setting::Type::TypeString) = service.getName();
        serviceSetting.add("type", Setting::Type::TypeInt) = (int) service.getType();
        serviceSetting.add("algorithmVersion", Setting::Type::TypeInt) = (int) service.getAlgorithmVersion();
        serviceSetting.add("counter", Setting::Type::TypeInt) = service.getCounter();
    }

    // Create a stream to write the config
    FILE *configFile = fopen(getConfigFileName().c_str(), "w");
    if (!configFile) {
        std::cerr << "Could not create config file" << std::endl;
        return;
    }

    // Write the config
    config.write(configFile);
    fclose(configFile);

    std::cout << "Success writing config." << std::endl;
}

std::string mpw_user::passwordForService(std::string siteName, MPSiteType siteType,
                                         MPAlgorithmVersion version, uint32_t counter) throw(password_generate_exception) {
    const char *res = mpw_passwordForSite(masterKey, siteName.c_str(), siteType, counter, MPSiteVariantPassword, NULL, version);
    if (res == NULL) {
        std::cerr << "mpw_passwordForSite returned NULL" << std::endl;
        throw password_generate_exception{};
    }
    return std::string{res};
}

std::string mpw_user::passwordForService(mpw_service &service) throw(password_generate_exception){
    return passwordForService(service.getName(), service.getType(), (uint32_t) service.getCounter(), service.getAlgorithmVersion());
}
