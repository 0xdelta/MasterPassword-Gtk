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
#include "mpw_user.h"
#include "mpw_window.h"

class mpw_password_window : public mpw_window {
private:
    mpw_user *user;
    Gtk::Entry *serviceEntry, *passwordOutput;
    Gtk::CheckButton *passwordVisibility;
    Gtk::ComboBox *passwordTypeSelect, *mpwVersionSelect;
    Gtk::SpinButton *counterSpinButton;

public:
    mpw_password_window(mpw_user *_user);
    ~mpw_password_window();

private:
    void logout();
    void compute_and_show_password();
    void update_password_visibility();
    void copy_password();

public:
    void setUser(mpw_user *user) {
        mpw_password_window::user = user;
    }
};

#endif //MPW_GTK_MPW_PASSWORD_WINDOW_H