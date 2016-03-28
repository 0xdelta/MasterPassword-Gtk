//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_MPW_LOGIN_WINDOW_H
#define MPW_GTK_MPW_LOGIN_WINDOW_H

#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <user_manager.h>
#include <gtkmm/combobox.h>
#include "mpw_window.h"

/**
 * The login window is the entry point of the application.
 */
class mpw_login_window : public mpw_window {
private:
    UserManager *userManager;
    Gtk::Entry *incognitoPasswordEntry, *incognitoUserEntry,
            *accountPasswordEntry;
    Gtk::ComboBox *accountUserSelect;
    Gtk::Button *manageAccountsButton, *createAccountButton, *incognitoLoginButton, *accountLoginButton;
public:
    mpw_login_window(UserManager *userManager);

private:
    /**
     * Updates the combobox that shows all registered users
     * from the user manager.
     */
    void updateAvailableUsers();
    /**
     * Open the create account window.
     */
    void createAccount();
    /**
     * Open the manage accounts window.
     */
    void manageAccounts();

    /**
     * Called when the account login button is clicked.
     */
    void accountLogin();
    /**
     * Updates the sensitivity of the account login button.
     */
    void updateAccountLoginButton();

    /**
     * Called when the incognito login button is clicked.
     */
    void incognitoLogin();
    /**
     * Updates the sensitivity of the incognito login button.
     */
    void updateIncognitoLoginButton();
};


#endif //MPW_GTK_MPW_LOGIN_WINDOW_H
