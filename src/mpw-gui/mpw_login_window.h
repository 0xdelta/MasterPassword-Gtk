//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_MPW_LOGIN_WINDOW_H
#define MPW_GTK_MPW_LOGIN_WINDOW_H

#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include "mpw_window.h"

class mpw_login_window : public mpw_window {
private:
    Gtk::Entry *incognitoPasswordEntry, *incognitoUserEntry,
            *accountPasswordEntry, *accountUserEntry;
    Gtk::Button *createAccountButton, *incognitoLoginButton, *accountLoginButton;
public:
    mpw_login_window();

private:
    void create_account();
    void incognito_login();
    void update_incognito_login_button();
    void account_login();
    void update_account_login_button();
};


#endif //MPW_GTK_MPW_LOGIN_WINDOW_H
