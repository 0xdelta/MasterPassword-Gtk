//
// Created by lukas on 09.03.16.
//

#include "mpw_create_account_window.h"

#include <iostream>
#include <gtkmm/messagedialog.h>
#include <gtkmm/liststore.h>

#include "mpw_login_window.h"
#include "incognito_user.h"
#include "mpw_password_window.h"
#include "simple_columns.h"
#include "mpw_manage_accounts_window.h"

extern std::string getResourceDir();

mpw_login_window *mpw_login_window::create(UserManager *userManager) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(getResourceDir() + "/ui/login.ui");
    mpw_login_window *window = nullptr;
    builder->get_widget_derived("window", window);
    window->postInit(userManager);
    return window;
}

mpw_login_window::mpw_login_window(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        mpw_window(cobject, builder) {
    // User Manager
    userManager = new UserManager;
    userManager->readFromConfig();

    // Global widgets
    builder->get_widget("create-account", createAccountButton);
    builder->get_widget("manage-accounts", manageAccountsButton);
    builder->get_widget("incognito-password-entry", incognitoPasswordEntry);
    builder->get_widget("account-password-entry", accountPasswordEntry);
    builder->get_widget("incognito-user-entry", incognitoUserEntry);
    builder->get_widget("account-user-select", accountUserSelect);
    builder->get_widget("incognito-login", incognitoLoginButton);
    builder->get_widget("account-login", accountLoginButton);

    createAccountButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::createAccount));
    manageAccountsButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::manageAccounts));

    incognitoLoginButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::incognitoLogin));
    incognitoPasswordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::incognitoLogin));
    incognitoPasswordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::updateIncognitoLoginButton));
    incognitoUserEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::incognitoLogin));
    incognitoUserEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::updateIncognitoLoginButton));

    accountLoginButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_login_window::accountLogin));
    accountPasswordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_login_window::accountLogin));
    accountPasswordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::updateAccountLoginButton));
    accountUserSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_login_window::updateAccountLoginButton));

    accountUserSelect->pack_start(simpleColumnsInstance.col_name);
}

void mpw_login_window::postInit(UserManager *_userManager) {
    userManager = _userManager;

    updateAvailableUsers();
}

void mpw_login_window::updateAvailableUsers() {
    // Create the model for the user select in the account tab
    Glib::RefPtr<Gtk::ListStore> usersModel = Gtk::ListStore::create(simpleColumnsInstance);
    accountUserSelect->set_model(usersModel);

    // Fill the password types model
    Gtk::TreeModel::Row row;
    for (auto pair : userManager->getAvailableUsers()) {
        std::string userName = pair.first;
        row = *(usersModel->append());
        simpleColumnsInstance.apply(row, {userName, 0});

        if (userManager->getLastUser() == userName) {
            accountUserSelect->set_active(row);
        }
    }
}

void mpw_login_window::accountLogin() {
    Gtk::TreeModel::iterator userItr = accountUserSelect->get_active();
    if (!userItr) {
        return;
    }

    Gtk::TreeModel::Row userRow = *userItr;
    if (!userRow) {
        return;
    }

    // Create the user from the input form
    std::string userName = (std::string) userRow.get_value(simpleColumnsInstance.col_name);
    std::string masterPassword = std::string(accountPasswordEntry->get_text());

    // Check whether the input is ok
    if (userName.size() == 0 || masterPassword.size() == 0) {
        return;
    }

    if (!userManager->existsUser(userName)) {
        accountPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*this, "Authentication failure", false, Gtk::MESSAGE_INFO);
        dialog.set_secondary_text("No account found for \"" + userName + "\". Please create an account using the \"Create Account\" button.");
        dialog.run();

        return;
    }

    AccountUser *user = userManager->readUserFromConfig(userName);

    if (!user) {
        Gtk::MessageDialog dialog(*this, "Error", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Could not read user \"" + userName + "\" from config.\n\nSee log for details.");
        dialog.run();
        return;
    }

    if (!user->unlockMasterKey(masterPassword)) {
        accountPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*this, "Authentication failure", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Incorrect master password for \"" + userName + "\"");
        dialog.run();

        delete user;
        return;
    }

    userManager->setLastUser(userName);
    userManager->writeToConfig();

    // Create the window
    mpw_password_window *passwordWindow = mpw_password_window::create(userManager, user);

    // Add the password window to the gtk application
    get_application()->add_window(*passwordWindow);

    // Delete this window
    delete this;
}

void mpw_login_window::updateAccountLoginButton() {
    accountLoginButton->set_sensitive(
            accountUserSelect->get_active() && accountPasswordEntry->get_text().size() > 0);
}

void mpw_login_window::incognitoLogin() {
    // Create the user from the input form
    std::string userName = std::string(incognitoUserEntry->get_text());
    std::string masterPassword = std::string(incognitoPasswordEntry->get_text());

    // Check whether the input is ok
    if (userName.size() == 0 || masterPassword.size() == 0) {
        return;
    }

    // Create the user
    IncognitoUser *user = new IncognitoUser{userName};
    user->unlockMasterKey(masterPassword);

    // Create the window
    mpw_password_window *passwordWindow = mpw_password_window::create(userManager, user);

    // Add the password window to the gtk application
    get_application()->add_window(*passwordWindow);

    // Delete this window
    delete this;
}

void mpw_login_window::updateIncognitoLoginButton() {
    incognitoLoginButton->set_sensitive(
            incognitoUserEntry->get_text().size() > 0 && incognitoPasswordEntry->get_text().size() > 0);
}

void mpw_login_window::createAccount() {
    mpw_create_account_window *createAccountWindow = mpw_create_account_window::create(userManager);
    createAccountWindow->signal_hide().connect(sigc::mem_fun(this, &mpw_login_window::updateAvailableUsers));
}

void mpw_login_window::manageAccounts() {
    mpw_manage_accounts_window *manageAccountsWindow = mpw_manage_accounts_window::create(userManager);
    manageAccountsWindow->signal_hide().connect(sigc::mem_fun(this, &mpw_login_window::updateAvailableUsers)); //TODO Create own signal
}
