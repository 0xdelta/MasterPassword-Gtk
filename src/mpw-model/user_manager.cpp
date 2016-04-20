//
// Created by lukas on 16.03.16.
//

#include "user_manager.h"
#include "incognito_user.h"

#include <sys/stat.h>
#include <libconfig.h++>
#include <iostream>
#include <mpw-util.h>

UserManager::UserManager() {
    // Ensure the config directory exists
    mkdir(getConfigDir().c_str(), S_IRWXU | S_IRGRP | S_IXGRP); // Folder with permission 750 (rwx r-x ---)
}

std::string UserManager::getConfigDir() {
    return std::string{getenv("HOME")} + "/.mpw";
}

std::string UserManager::getConfigFileName() {
    return getConfigDir() + "/users.cfg";
}

std::string UserManager::getUserConfigFileName(std::string &userName) {
    if (availableUsers.find(userName) == availableUsers.end()) {
        // Element does not exist
        return getConfigDir() + "/" + userName + ".user";
    } else {
        // User has a file
        return availableUsers.at(userName);
    }
}

bool UserManager::readFromConfig() {
    using namespace libconfig;
    FILE *configFile = fopen(getConfigFileName().c_str(), "r");

    if (!configFile) {
        // No config file found. We can ignore this case, as a
        // new file will be created when writeToConfig is called.
        return true;
    }

    Config config;

    try {
        config.read(configFile);
        fclose(configFile);
    } catch (ParseException &e) {
        std::cerr << "Could not read main config: " << e.what() << "(" << e.getError() << ")" << std::endl;
        fclose(configFile);
        return false;
    }

    int configVersion;
    try {
        configVersion = config.lookup("configVersion");
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read main config version: " << e.what() << std::endl;
        return false;
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
            return true;
        } else {
            std::cerr << "Unknown main config version: " << configVersion << std::endl;
        }
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read main config: " << e.what() << " (" << e.getPath() << ")" << std::endl;
    }
    return false;
}

void UserManager::writeToConfig() {
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

bool UserManager::existsUser(std::string &userName) {
    return availableUsers.find(userName) != availableUsers.end();
}

AccountUser *UserManager::readUserFromConfigDirect(std::string &fileName) {
    using namespace libconfig;
    FILE *configFile = fopen(fileName.c_str(), "r");

    if (!configFile) {
        std::cerr << "User config file does not exist: " << fileName << std::endl;
        return nullptr;
    }

    Config config;

    try {
        config.read(configFile);
        fclose(configFile);
    } catch (ParseException &e) {
        std::cerr << "Could not read user config: " << e.what() << "(" << e.getError() << ")" << std::endl;
        fclose(configFile);
        return nullptr;
    }

    int configVersion;
    try {
        configVersion = config.lookup("configVersion");
    } catch (SettingNotFoundException &e) {
        std::cerr << "Could not read user config version: " << e.what() << std::endl;
        return nullptr;
    }

    try {
        if (configVersion == 1) {
            std::string userName = config.lookup("userName");
            std::string keyIdString = config.lookup("keyId");
            const uint8_t *masterKeyId = mpw_hex_reverse(keyIdString.c_str(), keyIdString.size());
            MPAlgorithmVersion masterKeyAlgorithmVersion = (MPAlgorithmVersion) ((int) config.lookup("algorithmVersion"));

            AccountUser *user = new AccountUser{userName, masterKeyId, masterKeyAlgorithmVersion};

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

                Service service = Service{name, (MPSiteType) type, (MPAlgorithmVersion) serviceAlgorithmVersion, counter};
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

    return nullptr;
}

AccountUser *UserManager::readUserFromConfig(std::string &userName) {
    if (!existsUser(userName)) {
        std::cerr << "User " + userName + " not registered" << std::endl;
        return nullptr;
    }

    std::string fileName = getUserConfigFileName(userName);
    return readUserFromConfigDirect(fileName);
}

void UserManager::writeUserToConfig(User &user) {
    using namespace libconfig;

    // Create a config object and insert the values
    Config config;
    config.getRoot().add("userName", Setting::Type::TypeString) = user.getUserName();
    config.getRoot().add("configVersion", Setting::Type::TypeInt) = 1;
    config.getRoot().add("keyId", Setting::Type::TypeString) = user.getMasterKeyId() ? mpw_hex(user.getMasterKeyId(), 32) : "";
    config.getRoot().add("algorithmVersion", Setting::Type::TypeInt) = (int) user.getAlgorithmVersion();

    Setting &servicesSetting = config.getRoot().add("services", Setting::Type::TypeList);

    if (!user.isIncognito()) {
        for (auto &pair : *user.getServices()) {
            Service service = pair.second;
            Setting &serviceSetting = servicesSetting.add(Setting::Type::TypeGroup);
            serviceSetting.add("name", Setting::Type::TypeString) = service.getName();
            serviceSetting.add("type", Setting::Type::TypeInt) = (int) service.getType();
            serviceSetting.add("algorithmVersion", Setting::Type::TypeInt) = (int) service.getAlgorithmVersion();
            serviceSetting.add("counter", Setting::Type::TypeInt) = service.getCounter();
        }
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

bool UserManager::createUser(std::string &userName, std::string &masterPassword) {
    if (existsUser(userName)) {
        std::cerr << "A user with the name " << userName << " already exists." << std::endl;
        return false;
    }

    IncognitoUser user = IncognitoUser{userName};
    user.unlockMasterKey(masterPassword);

    writeUserToConfig(user);
    setLastUser(userName);
    return true;
}

bool UserManager::deleteUser(std::string &userName) {
    if (!existsUser(userName)) {
        return false;
    }

    availableUsers.erase(userName);
    if (lastUser == userName) {
        setLastUser("");
    }

    std::cout << "User " << userName << " deleted" << std::endl;
    return true;
}

bool UserManager::setUserFile(std::string &userName, std::string &file) {
    if (!existsUser(userName)) {
        return false;
    }

    availableUsers.at(userName) = file;
    return true;
}

bool UserManager::importUser(std::string &fileName) {
    AccountUser *user = readUserFromConfigDirect(fileName);

    if(user) {
        availableUsers.emplace(user->getUserName(), fileName);
        setLastUser(user->getUserName());

        delete user;
        return true;
    }

    return false;
}
