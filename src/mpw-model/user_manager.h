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
public:
    user_manager();

    std::string getConfigDir();
    std::string getConfigFileName();
    std::string getUserConfigFileName(std::string &userName);

    void readFromConfig();
    void writeToConfig();

    account_user* readUserFromConfig(std::string &userName);
    void writeUserToConfig(account_user &user);

};


#endif //MPW_GTK_USER_MANAGER_H
