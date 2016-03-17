//
// Created by lukas on 09.03.16.
//

#include "mpw_create_account_window.h"

#include <gtkmm/builder.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include <incognito_user.h>
#include <gtkmm/liststore.h>
#include "mpw_login_window.h"
#include "mpw_password_window.h"
#include "simple_columns.h"

mpw_login_window::mpw_login_window(user_manager *_userManager) :
        userManager(_userManager) {
    auto builder = Gtk::Builder::create_from_file("ui/login.ui");

    // Global widgets
    builder->get_widget("window", window);
    builder->get_widget("create-account", createAccountButton);
    builder->get_widget("incognito-password-entry", incognitoPasswordEntry);
    builder->get_widget("account-password-entry", accountPasswordEntry);
    builder->get_widget("incognito-user-entry", incognitoUserEntry);
    builder->get_widget("account-user-select", accountUserSelect);
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
    accountUserSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::update_account_login_button));

    // Create the model for the user select in the account tab
    Glib::RefPtr<Gtk::ListStore> usersModel = Gtk::ListStore::create(simple_columns_instance);
    accountUserSelect->set_model(usersModel);
    accountUserSelect->pack_start(simple_columns_instance.col_name);

    // Fill the password types model
    Gtk::TreeModel::Row row;
    for (auto pair : userManager->getAvailableUsers()) {
        row = *(usersModel->append());
        simple_columns_instance.apply(row, {0, pair.first, 0});
    }
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
    incognito_user *user = new incognito_user{userName};
    user->unlockMasterKey(masterPassword);

    // Create the window
    mpw_password_window *passwordWindow = new mpw_password_window{userManager, user};

    // Add the password window to the gtk application
    window->get_application()->add_window(*passwordWindow->getWindow());

    std::cout << "delete me2!" << std::endl;

    // Hide and delete this window
    window->hide();
    delete this;
}

void mpw_login_window::update_incognito_login_button() {
    incognitoLoginButton->set_sensitive(
            incognitoUserEntry->get_text().size() > 0 && incognitoPasswordEntry->get_text().size() > 0);
}

void mpw_login_window::account_login() {
    Gtk::TreeModel::iterator userItr = accountUserSelect->get_active();
    if (!userItr) {
        return;
    }

    Gtk::TreeModel::Row userRow = *userItr;
    if (!userRow) {
        return;
    }

    // Create the user from the input form
    std::string userName = (std::string) userRow.get_value(simple_columns_instance.col_name);
    std::string masterPassword = std::string(accountPasswordEntry->get_text());

    // Check whether the input is ok
    if (userName.size() == 0 || masterPassword.size() == 0) {
        return;
    }

    if (!userManager->existsUser(userName)) {
        accountPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*window, "Authentication failure", false, Gtk::MESSAGE_INFO);
        dialog.set_secondary_text("No account found for \"" + userName + "\". Please create an account using the \"Create Account\" button.");
        dialog.run();

        return;
    }

    account_user *user = userManager->readUserFromConfig(userName);

    if (!user->unlockMasterKey(masterPassword)) {
        accountPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*window, "Authentication failure", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Incorrect master password for \"" + userName + "\"");
        dialog.run();

        delete user;
        return;
    }

    // Create the window
    mpw_password_window *passwordWindow = new mpw_password_window{userManager, user};

    // Add the password window to the gtk application
    window->get_application()->add_window(*passwordWindow->getWindow());

    std::cout << "delete me!" << std::endl;

    // Hide and delete this window
    window->hide();
    delete this;
}

void mpw_login_window::update_account_login_button() {
    accountLoginButton->set_sensitive(
            accountUserSelect->get_active() && accountPasswordEntry->get_text().size() > 0);
}

void mpw_login_window::create_account() {
    new mpw_create_account_window{userManager};
}
