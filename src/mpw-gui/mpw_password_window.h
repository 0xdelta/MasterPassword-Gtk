//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_MPW_PASSWORD_WINDOW_H
#define MPW_GTK_MPW_PASSWORD_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/checkbutton.h>
#include "user.h"
#include "mpw_window.h"

class mpw_password_window : public mpw_window {
private:
    UserManager *userManager;
    User *user;

    Gtk::Entry *serviceEntry, *passwordOutput;
    Gtk::CheckButton *passwordVisibility;
    Gtk::ComboBox *passwordTypeSelect, *mpwVersionSelect;
    Gtk::SpinButton *counterSpinButton;

public:
    mpw_password_window(UserManager *userManager, User *_user);
    virtual ~mpw_password_window();

private:
    void logout();
    void serviceSettingsChanged();
    void computeAndShowPassword();
    void updatePasswordVisibility();
    void copyPassword();
};

#endif //MPW_GTK_MPW_PASSWORD_WINDOW_H
