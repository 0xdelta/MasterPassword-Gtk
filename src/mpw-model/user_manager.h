//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_USER_MANAGER_H
#define MPW_GTK_USER_MANAGER_H

#include <unordered_map>
#include <string>
#include "account_user.h"

class user_manager {
private:
    // Maps the user name to the config file location
    std::unordered_map<std::string, std::string> availableUsers;
    std::string lastUser;

public:
    user_manager();

    std::string getConfigDir();
    std::string getConfigFileName();
    std::string getUserConfigFileName(std::string &userName);

    void readFromConfig();
    void writeToConfig();

    bool existsUser(std::string &userName);
    account_user *readUserFromConfig(std::string &userName);
    void writeUserToConfig(user &user);
    bool createUser(std::string &userName, std::string &masterPassword);

    const std::unordered_map<std::string, std::string> &getAvailableUsers() const {
        return availableUsers;
    }

    const std::string &getLastUser() const {
        return lastUser;
    }

    void setLastUser(const std::string &lastUser) {
        user_manager::lastUser = lastUser;
    }
};


#endif //MPW_GTK_USER_MANAGER_H
