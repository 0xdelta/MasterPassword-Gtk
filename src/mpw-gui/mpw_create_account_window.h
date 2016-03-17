//
// Created by lukas on 13.03.16.
//

#ifndef MPW_GTK_MPW_CREATE_ACCOUNT_WINDOW_H
#define MPW_GTK_MPW_CREATE_ACCOUNT_WINDOW_H

#include <gtkmm/entry.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
#include <user_manager.h>
#include "mpw_window.h"

class mpw_create_account_window : public mpw_window {
private:
    UserManager *userManager;
    Gtk::ProgressBar *passwordStrength;
    Gtk::Entry *userEntry, *passwordEntry, *repeatPasswordEntry;
    Gtk::Button *cancelButton,*createButton;
public:
    mpw_create_account_window(UserManager *userManager);

    void updatePasswordStrength();
    void updateCreateButton();
    void cancel();
    void create();
};


#endif //MPW_GTK_MPW_CREATE_ACCOUNT_WINDOW_H
