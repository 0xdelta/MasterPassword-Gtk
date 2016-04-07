//
// Created by lukas on 13.03.16.
//

#include "mpw_create_account_window.h"

#include <gtkmm/messagedialog.h>

extern std::string getResourceDir();

mpw_create_account_window *mpw_create_account_window::create(UserManager *userManager) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(getResourceDir() + "/ui/create-account.ui");
    mpw_create_account_window *window = nullptr;
    builder->get_widget_derived("window", window);
    window->userManager = userManager;
    return window;
}

mpw_create_account_window::mpw_create_account_window(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        mpw_window(cobject, builder) {
    // Widgets
    builder->get_widget("password-strength", passwordStrength);
    builder->get_widget("user-entry", userEntry);
    builder->get_widget("password-entry", passwordEntry);
    builder->get_widget("repeat-password-entry", repeatPasswordEntry);
    builder->get_widget("cancel-button", cancelButton);
    builder->get_widget("create-button", createButton);

    // Signals
    passwordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::updatePasswordStrength));
    userEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::updateCreateButton));
    passwordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::updateCreateButton));
    repeatPasswordEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_create_account_window::updateCreateButton));
    cancelButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_create_account_window::cancelButtonClicked));
    createButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_create_account_window::createButtonClicked));
    userEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_create_account_window::createButtonClicked));
    passwordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_create_account_window::createButtonClicked));
    repeatPasswordEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_create_account_window::createButtonClicked));
}

void mpw_create_account_window::updatePasswordStrength() {
    passwordStrength->set_fraction(std::min(1.0, (double) passwordEntry->get_text().size() / 14));
}

void mpw_create_account_window::updateCreateButton() {
    createButton->set_sensitive(userEntry->get_text().size() > 0 &&
                                passwordEntry->get_text().size() > 0 &&
                                repeatPasswordEntry->get_text().size() > 0);
}

void mpw_create_account_window::cancelButtonClicked() {
    delete this;
}

void mpw_create_account_window::createButtonClicked() {
    std::string userName = userEntry->get_text();
    std::string password = passwordEntry->get_text();
    std::string passwordRepeat = repeatPasswordEntry->get_text();

    if (userName.size() == 0 || password.size() == 0 || passwordRepeat.size() == 0) {
        return;
    }

    if (password != passwordRepeat) {
        passwordEntry->set_text("");
        repeatPasswordEntry->set_text("");

        Gtk::MessageDialog dialog(*this, "Warning", false, Gtk::MESSAGE_WARNING);
        dialog.set_secondary_text("Passwords are not equal");
        dialog.run();
        return;
    }

    if (userManager->existsUser(userName)) {
        Gtk::MessageDialog dialog(*this, "Error", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("An account with the name \"" + userName + "\" already exists.");
        dialog.run();
        return;
    }

    if (!userManager->createUser(userName, password)) {
        Gtk::MessageDialog dialog(*this, "Error", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Could not create a user with the name \"" + userName + "\".\n\nSee log for details");
        dialog.run();
        return;
    }

    userManager->writeToConfig();

    Gtk::MessageDialog dialog(*this, "Success", false, Gtk::MESSAGE_INFO);
    dialog.set_secondary_text("Account created! You can now login using your username and password.");
    dialog.run();

    delete this;
}
