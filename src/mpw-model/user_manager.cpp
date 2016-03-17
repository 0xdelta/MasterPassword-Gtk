//
// Created by lukas on 16.03.16.
//

#include "user_manager.h"
#include "incognito_user.h"

#include <sys/stat.h>
#include <libconfig.h++>
#include <iostream>
#include <mpw-util.h>

user_manager::user_manager() {
    // Ensure the config directory exists
    mkdir(getConfigDir().c_str(), S_IRWXU | S_IRGRP | S_IXGRP); // Folder with permission 750 (rwx r-x ---)
}

std::string user_manager::getConfigDir() {
    return std::string{getenv("HOME")} + "/.mpw";
}

std::string user_manager::getConfigFileName() {
    return getConfigDir() + "/users.cfg";
}

std::string user_manager::getUserConfigFileName(std::string &userName) {
    if (availableUsers.find(userName) == availableUsers.end()) {
        // Element does not exist
        return getConfigDir() + "/" + userName + ".user";
    } else {
        // User has a file
        return availableUsers.at(userName);
    }
}

void user_manager::readFromConfig() {
    using namespace libconfig;
    FILE *configFile = fopen(getConfigFileName().c_str(), "r");

    if (!configFile) {
        // No config file found. We can ignore this case, as a
        // new file will be created when writeToConfig is called.
        return;
    }

    Config config;

    try {
        config.read(configFile);
        fclose(configFile);
    } catch (ParseException &e) {
        std::cerr << "Could not read main config: " << e.what() << "(" << e.getError() << ")" << std::endl;
        fclose(configFile);
        return;
    }

    int configVersion;
    try {
        configVersion = config.lookup("configVersion");
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read main config version: " << e.what() << std::endl;
        return;
    }

    try {
        if (configVersion == 1) {
            lastUser = config.lookup("lastUser").c_str();

            Setting &usersSetting = config.lookup("users");
            for (int i = 0; i < usersSetting.getLength(); ++i) {
                Setting &userSetting = usersSetting[i];

                std::string name, path;
                if (!(userSetting.lookupValue("name", name) &&
                      userSetting.lookupValue("path", path))) {
                    // Ignore invalid
                    continue;
                }

                availableUsers.emplace(name, path);
            }

            std::cout << "Success reading main config." << std::endl;
        } else {
            std::cerr << "Unknown main config version: " << configVersion << std::endl;
        }
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read main config: " << e.what() << " (" << e.getPath() << ")" << std::endl;
    }
}

void user_manager::writeToConfig() {
    using namespace libconfig;

    // Create a config object and insert the values
    Config config;
    config.getRoot().add("configVersion", Setting::Type::TypeInt) = 1;
    config.getRoot().add("lastUser", Setting::Type::TypeString) = lastUser;

    Setting &usersSetting = config.getRoot().add("users", Setting::Type::TypeList);
    for (auto &pair : availableUsers) {
        Setting &userSetting = usersSetting.add(Setting::Type::TypeGroup);
        userSetting.add("name", Setting::Type::TypeString) = pair.first;
        userSetting.add("path", Setting::Type::TypeString) = pair.second;
    }

    // Create a stream to write the config
    FILE *configFile = fopen(getConfigFileName().c_str(), "w");
    if (!configFile) {
        std::cerr << "Could not create main config file" << std::endl;
        return;
    }

    // Write the config
    config.write(configFile);
    fclose(configFile);

    std::cout << "Success writing main config." << std::endl;
}

bool user_manager::existsUser(std::string &userName) {
    return availableUsers.find(userName) != availableUsers.end();
}

account_user *user_manager::readUserFromConfig(std::string &userName) {
    using namespace libconfig;
    FILE *configFile = fopen(getUserConfigFileName(userName).c_str(), "r");

    if (!configFile) {
        std::cerr << "User config file does not exist: " << getUserConfigFileName(userName) << std::endl;
        return NULL;
    }

    Config config;

    try {
        config.read(configFile);
        fclose(configFile);
    } catch (ParseException &e) {
        std::cerr << "Could not read user config: " << e.what() << "(" << e.getError() << ")" << std::endl;
        fclose(configFile);
        return NULL;
    }

    int configVersion;
    try {
        configVersion = config.lookup("configVersion");
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read user config version: " << e.what() << std::endl;
        return NULL;
    }

    try {
        if (configVersion == 1) {
            std::string keyIdString = config.lookup("keyId");
            const uint8_t *masterKeyId = mpw_hex_reverse(keyIdString.c_str(), keyIdString.size());
            MPAlgorithmVersion masterKeyAlgorithmVersion = (MPAlgorithmVersion) ((int) config.lookup("algorithmVersion"));

            account_user *user = new account_user{userName, masterKeyId, masterKeyAlgorithmVersion};

            Setting &servicesSetting = config.lookup("services");
            for (int i = 0; i < servicesSetting.getLength(); ++i) {
                Setting &serviceSetting = servicesSetting[i];

                std::string name;
                int type, serviceAlgorithmVersion, counter;
                if (!(serviceSetting.lookupValue("name", name) &&
                      serviceSetting.lookupValue("type", type) &&
                      serviceSetting.lookupValue("algorithmVersion", serviceAlgorithmVersion) &&
                      serviceSetting.lookupValue("counter", counter))) {
                    // Ignore invalid
                    continue;
                }

                mpw_service service = mpw_service{name, (MPSiteType) type, (MPAlgorithmVersion) serviceAlgorithmVersion, counter};
                user->addService(service);
            }

            std::cout << "Success reading user config." << std::endl;
            return user;
        } else {
            std::cerr << "Unknown user config version: " << configVersion << std::endl;
        }
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read user config: " << e.what() << " (" << e.getPath() << ")" << std::endl;
    }

    return NULL;
}

void user_manager::writeUserToConfig(user &user) {
    using namespace libconfig;

    // Create a config object and insert the values
    Config config;
    config.getRoot().add("configVersion", Setting::Type::TypeInt) = 1;
    config.getRoot().add("keyId", Setting::Type::TypeString) = user.getMasterKeyId() ? mpw_hex(user.getMasterKeyId(), 32) : "";
    config.getRoot().add("algorithmVersion", Setting::Type::TypeInt) = (int) user.getAlgorithmVersion();

    Setting &servicesSetting = config.getRoot().add("services", Setting::Type::TypeList);
    for (auto &service : user.getServices()) {
        Setting &serviceSetting = servicesSetting.add(Setting::Type::TypeGroup);
        serviceSetting.add("name", Setting::Type::TypeString) = service.getName();
        serviceSetting.add("type", Setting::Type::TypeInt) = (int) service.getType();
        serviceSetting.add("algorithmVersion", Setting::Type::TypeInt) = (int) service.getAlgorithmVersion();
        serviceSetting.add("counter", Setting::Type::TypeInt) = service.getCounter();
    }

    // Create a stream to write the config
    std::string configFileName = getUserConfigFileName((std::string &) user.getUserName());
    FILE *configFile = fopen(configFileName.c_str(), "w");
    if (!configFile) {
        std::cerr << "Could not create user config file" << std::endl;
        return;
    }

    // Write the config
    config.write(configFile);
    fclose(configFile);

    availableUsers.emplace(user.getUserName(), configFileName);
    std::cout << "Success writing user config." << std::endl;
}

bool user_manager::createUser(std::string &userName, std::string &masterPassword) {
    if (existsUser(userName)) {
        return false;
    }

    incognito_user user = incognito_user{userName};
    user.unlockMasterKey(masterPassword);

    writeUserToConfig(user);
    setLastUser(userName);
    return true;
}
