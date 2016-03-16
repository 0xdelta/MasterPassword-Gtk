//
// Created by lukas on 09.03.16.
//

#include "mpw_create_account_window.h"

#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include "mpw_login_window.h"
#include "mpw_password_window.h"

mpw_login_window::mpw_login_window() {
    auto builder = Gtk::Builder::create_from_file("ui/login.ui");

    // Global widgets
    builder->get_widget("window", window);
    builder->get_widget("create-account", createAccountButton);
    builder->get_widget("incognito-password-entry", incognitoPasswordEntry);
    builder->get_widget("account-password-entry", accountPasswordEntry);
    builder->get_widget("incognito-user-entry", incognitoUserEntry);
    builder->get_widget("account-user-entry", accountUserEntry);
    builder->get_widget("incognito-login", incognitoLoginButton);
    builder->get_widget("account-login", accountLoginButton);

    createAccountButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::create_account));

    incognitoLoginButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::incognito_login));
    incognitoPasswordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::incognito_login));
    incognitoPasswordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::update_incognito_login_button));
    incognitoUserEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::incognito_login));
    incognitoUserEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::update_incognito_login_button));

    accountLoginButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::account_login));
    accountPasswordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::account_login));
    accountPasswordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::update_account_login_button));
    accountUserEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::account_login));
    accountUserEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::update_account_login_button));
}


void mpw_login_window::incognito_login() {
    // Create the user from the input form
    std::string userName = std::string(incognitoUserEntry->get_text());
    std::string masterPassword = std::string(incognitoPasswordEntry->get_text());

    // Check whether the input is ok
    if (userName.size() == 0 || masterPassword.size() == 0) {
        return;
    }

    // Create the user
    mpw_user *user = new mpw_user(userName);
    user->loadDefaults();
    user->unlockMasterKey(masterPassword);

    // Create the window
    mpw_password_window *passwordWindow = new mpw_password_window{user};

    // Add the password window to the gtk application
    window->get_application()->add_window(*passwordWindow->getWindow());

    // Hide and delete this window
    window->hide();
    delete this;
}

void mpw_login_window::update_incognito_login_button() {
    Glib::ustring userName = incognitoUserEntry->get_text();
    Glib::ustring masterPassword = incognitoPasswordEntry->get_text();

    incognitoLoginButton->set_sensitive(userName.size() > 0 && masterPassword.size() > 0);
}

void mpw_login_window::account_login() {
    // Create the user from the input form
    std::string userName = std::string(accountUserEntry->get_text());
    std::string masterPassword = std::string(accountPasswordEntry->get_text());

    // Check whether the input is ok
    if (userName.size() == 0 || masterPassword.size() == 0) {
        return;
    }

    // Create the user
    mpw_user *user = new mpw_user(userName);
    if (!user->hasConfigFile()) {
        accountPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*window, "Authentication failure", false, Gtk::MESSAGE_INFO);
        dialog.set_secondary_text("No account found for \"" + userName + "\". Please create an account using the \"Create Account\" button.");
        dialog.run();

        delete user;
        return;
    }

    user->readFromConfig();
    if (!user->unlockMasterKey(masterPassword)) {
        accountPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*window, "Authentication failure", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Incorrect master password for \"" + userName + "\"");
        dialog.run();

        delete user;
        return;
    }
    user->writeToConfig();

    // Create the window
    mpw_password_window *passwordWindow = new mpw_password_window{user};

    // Add the password window to the gtk application
    window->get_application()->add_window(*passwordWindow->getWindow());

    // Hide and delete this window
    window->hide();
    delete this;
}

void mpw_login_window::update_account_login_button() {
    Glib::ustring userName = accountUserEntry->get_text();
    Glib::ustring masterPassword = accountPasswordEntry->get_text();

    accountLoginButton->set_sensitive(userName.size() > 0 && masterPassword.size() > 0);
}

void mpw_login_window::create_account() {
    new mpw_create_account_window;
}
