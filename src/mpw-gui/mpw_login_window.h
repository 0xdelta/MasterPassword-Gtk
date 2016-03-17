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

class mpw_login_window : public mpw_window {
private:
    UserManager *userManager;
    Gtk::Entry *incognitoPasswordEntry, *incognitoUserEntry,
            *accountPasswordEntry;
    Gtk::ComboBox *accountUserSelect;
    Gtk::Button *createAccountButton, *incognitoLoginButton, *accountLoginButton;
public:
    mpw_login_window(UserManager *userManager);

private:
    void updateAvailableUsers();
    void createAccount();

    void accountLogin();
    void updateAccountLoginButton();

    void incognitoLogin();
    void updateIncognitoLoginButton();
};


#endif //MPW_GTK_MPW_LOGIN_WINDOW_H
