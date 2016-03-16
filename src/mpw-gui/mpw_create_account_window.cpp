//
// Created by lukas on 13.03.16.
//

#include "mpw_create_account_window.h"

#include <gtkmm/builder.h>
#include <gtkmm/messagedialog.h>
#include <mpw_user.h>

mpw_create_account_window::mpw_create_account_window() {
    auto builder = Gtk::Builder::create_from_file("ui/create-account.ui");

    // Widgets
    builder->get_widget("window", window);
    builder->get_widget("password-strength", passwordStrength);
    builder->get_widget("user-entry", userEntry);
    builder->get_widget("password-entry", passwordEntry);
    builder->get_widget("repeat-password-entry", repeatPasswordEntry);
    builder->get_widget("cancel-button", cancelButton);
    builder->get_widget("create-button", createButton);

    // Signals
    passwordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::update_password_strength));
    userEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::update_create_button));
    passwordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::update_create_button));
    repeatPasswordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::update_create_button));
    cancelButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_create_account_window::cancel));
    createButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_create_account_window::create));
    userEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_create_account_window::create));
    passwordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_create_account_window::create));
    repeatPasswordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_create_account_window::create));
}

void mpw_create_account_window::update_password_strength() {
    passwordStrength->set_fraction(std::min(1.0, (double) passwordEntry->get_text().size() / 14));
}

void mpw_create_account_window::update_create_button() {
    createButton->set_sensitive(userEntry->get_text().size() > 0 &&
                                passwordEntry->get_text().size() > 0 &&
                                repeatPasswordEntry->get_text().size() > 0);
}

void mpw_create_account_window::cancel() {
    window->hide();
    delete this;
}


void mpw_create_account_window::create() {
    std::string userName = userEntry->get_text();
    std::string password = passwordEntry->get_text();
    std::string passwordRepeat = repeatPasswordEntry->get_text();

    if (userName.size() == 0 || password.size() == 0 || passwordRepeat.size() == 0) {
        return;
    }

    if (password != passwordRepeat) {
        passwordEntry->set_text("");
        repeatPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*window, "Warning", false, Gtk::MESSAGE_WARNING);
        dialog.set_secondary_text("Passwords are not equal");
        dialog.run();
        return;
    }

    mpw_user user{userName};

    if (user.hasConfigFile()) {
        Gtk::MessageDialog dialog(*window, "Error", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("An account with the name \"" + userName + "\" already exists.");
        dialog.run();
        return;
    }

    user.loadDefaults();
    user.unlockMasterKey(password);
    user.writeToConfig();

    Gtk::MessageDialog dialog(*window, "Success", false, Gtk::MESSAGE_INFO);
    dialog.set_secondary_text("Account created! You can now login using your username and password.");
    dialog.run();

    window->hide();
    delete this;
}