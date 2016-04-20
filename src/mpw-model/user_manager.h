//
// Created by lukas on 16.03.16.
//

#ifndef MPW_GTK_USER_MANAGER_H
#define MPW_GTK_USER_MANAGER_H

#include <unordered_map>
#include <string>
#include "account_user.h"

/**
 * This class manages the account user instances.
 */
class UserManager {
private:
    // Maps the user name to the config file location
    std::unordered_map<std::string, std::string> availableUsers;
    // The last user that logged in
    std::string lastUser;

public:
    /**
     * Creates a new instance and ensures, that the config
     * directory exists.
     */
    UserManager();

    /**
     * Get the config dir where the config file of the user manager is stored.
     * The config dir is: {HOME}/.mpw
     */
    std::string getConfigDir();
    /**
     * Get the config file name. That is: {CONFIG_DIR}/users.cfg
     */
    std::string getConfigFileName();
    /**
     * Get the config file name for a given user name. If the user is
     * registered, the stored path will be returned. Otherwise, the
     * default path ({CONFIG_DIR}/{USER_NAME}.user) will be returned.
     */
    std::string getUserConfigFileName(std::string &userName);

    /**
     * Reads the config file.
     * @retval true if succeed.
     */
    bool readFromConfig();
    /**
     * Writes the current state to the config.
     */
    bool writeToConfig();

    /**
     * Checks if a given user name is registered by this manager.
     */
    bool existsUser(std::string &userName);
    /**
     * Reads an account user from its config file.
     */
    AccountUser *readUserFromConfigDirect(std::string &fileName);
    /**
     * Reads an account user from its config file. This calls
     * the previous function with the return value of getUserConfigFileName(string)
     */
    AccountUser *readUserFromConfig(std::string &userName);
    /**
     * Writes the given user to the file, that is returned from
     * getUserConfigFileName(string)
     */
    bool writeUserToConfig(User &user);
    /**
     * Creates a new user with the given user name and master password.
     * @retval true if succeed.
     */
    bool createUser(std::string &userName, std::string &masterPassword);
    /**
     * Unregister a user from this manager. This will not delete
     * the user file.
     * @retval true if succeed.
     */
    bool deleteUser(std::string &userName);
    /**
     * Changes the user file for a given user name.
     */
    bool setUserFile(std::string &userName, std::string &file);
    /**
     * Imports a user from a given file.
     * @retval true if succeed.
     */
    bool importUser(std::string &fileName);

    /* Getters & Setters */

    const std::unordered_map<std::string, std::string> &getAvailableUsers() const {
        return availableUsers;
    }

    const std::string &getLastUser() const {
        return lastUser;
    }

    void setLastUser(const std::string &lastUser) {
        UserManager::lastUser = lastUser;
    }
};


#endif //MPW_GTK_USER_MANAGER_H
