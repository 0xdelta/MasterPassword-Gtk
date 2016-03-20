//
// Created by lukas on 09.03.16.
//

#include "mpw_login_window.h"

#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>
#include <gtkmm/listviewtext.h>
#include <iostream>
#include <gtkmm/messagedialog.h>
#include "mpw_password_window.h"
#include "simple_columns.h"

#include "password_type.h"
#include "algorithm_version.h"

mpw_password_window::mpw_password_window(UserManager *_userManager, User *_usr) :
        userManager(_userManager), user(_usr) {
    auto builder = Gtk::Builder::create_from_file("ui/password.ui");

    Gtk::Button *logoutButton, *copyButton;

    // Widgets
    builder->get_widget("window", window);
    builder->get_widget("service-entry", serviceEntry);
    builder->get_widget("password-output", passwordOutput);
    builder->get_widget("password-visibility", passwordVisibility);
    builder->get_widget("password-type", passwordTypeSelect);
    builder->get_widget("mpw-version", mpwVersionSelect);
    builder->get_widget("counter", counterSpinButton);
    builder->get_widget("logout", logoutButton);
    builder->get_widget("copy-password", copyButton);

    // Signals
    serviceEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceNameChanged));
    serviceEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_password_window::copyPassword));
    passwordVisibility->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::updatePasswordVisibility));
    passwordTypeSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceSettingsChanged));
    mpwVersionSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceSettingsChanged));
    counterSpinButton->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceSettingsChanged));
    logoutButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::logout));
    copyButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::copyPassword));

    // Create the models for ComboBoxes
    Glib::RefPtr<Gtk::ListStore> passwordTypesModel = Gtk::ListStore::create(simpleColumnsInstance);
    Glib::RefPtr<Gtk::ListStore> mpwVersionsModel = Gtk::ListStore::create(simpleColumnsInstance);

    // Use the models for our ComboBoxes
    passwordTypeSelect->set_model(passwordTypesModel);
    mpwVersionSelect->set_model(mpwVersionsModel);

    // Configure which elements are shown within the ComboBoxes
    passwordTypeSelect->pack_start(simpleColumnsInstance.col_name);
    mpwVersionSelect->pack_start(simpleColumnsInstance.col_name);

    // Temporary variable
    Gtk::TreeModel::Row row;

    // Fill the password types model
    for (auto &type : passwordTypes) {
        row = *(passwordTypesModel->append());
        simpleColumnsInstance.apply(row, {type.getName(), (int) type.getMpSiteType()});

        if (type.getMpSiteType() == defaultPasswordType.getMpSiteType()) {
            passwordTypeSelect->set_active(row);
        }
    }

    //Fill the ComboBox's Tree Model:
    for (auto &version : algorithmVersions) {
        row = *(mpwVersionsModel->append());
        simpleColumnsInstance.apply(row, {version.getDisplayName(), (int) version.getMpAlgorithmVersion()});

        if (version.getMpAlgorithmVersion() == defaultAlgorithmVersion.getMpAlgorithmVersion()) {
            mpwVersionSelect->set_active(row);
        }
    }

    if (!user->isIncognito()) {
        // Auto Completion for the service entry
        auto completion = Gtk::EntryCompletion::create();
        auto autoCompleteModel = Gtk::ListStore::create(simpleColumnsInstance);

        serviceEntry->set_completion(completion);
        completion->set_model(autoCompleteModel);
        completion->set_text_column(simpleColumnsInstance.col_name);

        for (auto &pair : *user->getServices()) {
            Service service = pair.second;
            row = *(autoCompleteModel->append());
            simpleColumnsInstance.apply(row, {service.getName(), 0});
        }
    }
}

mpw_password_window::~mpw_password_window() {
    delete user;
}

void mpw_password_window::setServiceSettings(MPSiteType siteType, MPAlgorithmVersion algorithmVersion, int counter) {
    // Site type
    Gtk::TreeModel::Children types = passwordTypeSelect->get_model()->children();
    for (Gtk::TreeModel::Children::iterator iter = types.begin(); iter != types.end(); ++iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row.get_value(simpleColumnsInstance.col_data) == siteType) {
            passwordTypeSelect->set_active(row);
            break;
        }
    }

    // Version
    Gtk::TreeModel::Children versions = mpwVersionSelect->get_model()->children();
    for (Gtk::TreeModel::Children::iterator iter = versions.begin(); iter != versions.end(); ++iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row.get_value(simpleColumnsInstance.col_data) == algorithmVersion) {
            mpwVersionSelect->set_active(row);
            break;
        }
    }

    // Counter
    counterSpinButton->set_text(std::to_string(counter));
    counterSpinButton->update();
}

void mpw_password_window::logout() {
    // Create the login window
    mpw_login_window *loginWindow = new mpw_login_window{userManager};

    // Add the password window to the gtk application
    window->get_application()->add_window(*loginWindow->getWindow());

    // Hide and delete this window
    window->hide();
    delete this;
}

void mpw_password_window::serviceNameChanged() {
    std::string serviceName = serviceEntry->get_text();

    if (!user->isIncognito() && user->getServices()->find(serviceName) != user->getServices()->end()) {
        // Display stored info
        Service &service = (Service &) user->getServices()->at(serviceName);

        setServiceSettings(service.getType(), service.getAlgorithmVersion(), service.getCounter());
    } else {
        // Display defaults
        setServiceSettings(defaultPasswordType.getMpSiteType(), defaultAlgorithmVersion.getMpAlgorithmVersion(), 1);
    }

    computeAndShowPassword();
}

void mpw_password_window::serviceSettingsChanged() {
    std::string serviceName = serviceEntry->get_text();

    if (!user->isIncognito() && user->getServices()->find(serviceName) != user->getServices()->end()) {
        // The user changed the settings for a service, that is stored
        // in his config file. So we have to update the service, in order
        // to hold the config up to date.

        Service &service = (Service &) user->getServices()->at(serviceName);

        Gtk::TreeModel::iterator siteTypeItr = passwordTypeSelect->get_active(); // Get pointer to active site type
        Gtk::TreeModel::iterator mpwVersionItr = mpwVersionSelect->get_active(); // Get pointer to active version
        if (!siteTypeItr || !mpwVersionItr) {
            goto show;
        }

        Gtk::TreeModel::Row siteTypeRow = *siteTypeItr;
        Gtk::TreeModel::Row mpwVersionRow = *mpwVersionItr;
        if (!siteTypeRow || !mpwVersionRow) {
            goto show;
        }

        // Collect the needed data
        MPSiteType type = (MPSiteType) siteTypeRow[simpleColumnsInstance.col_data];
        MPAlgorithmVersion version = (MPAlgorithmVersion) mpwVersionRow[simpleColumnsInstance.col_data];
        uint32_t counter = (uint32_t) std::stoi(counterSpinButton->get_text());

        service.setType(type);
        service.setAlgorithmVersion(version);
        service.setCounter(counter);

        userManager->writeUserToConfig(*user);
    }

    show:
    computeAndShowPassword();
}

void mpw_password_window::computeAndShowPassword() {
    std::string serviceName = serviceEntry->get_text();
    if (serviceName.size() == 0) {
        passwordOutput->set_text("");
        return;
    }

    if (user->isIncognito() || user->getServices()->find(serviceName) == user->getServices()->end()) {
        // No Service found

        Gtk::TreeModel::iterator siteTypeItr = passwordTypeSelect->get_active(); // Get pointer to active site type
        Gtk::TreeModel::iterator mpwVersionItr = mpwVersionSelect->get_active(); // Get pointer to active version
        if (!siteTypeItr || !mpwVersionItr) {
            passwordOutput->set_text("");
            return;
        }

        Gtk::TreeModel::Row siteTypeRow = *siteTypeItr;
        Gtk::TreeModel::Row mpwVersionRow = *mpwVersionItr;
        if (!siteTypeRow || !mpwVersionRow) {
            passwordOutput->set_text("");
            return;
        }

        // Collect the needed data
        MPSiteType type = (MPSiteType) siteTypeRow[simpleColumnsInstance.col_data];
        MPAlgorithmVersion version = (MPAlgorithmVersion) mpwVersionRow[simpleColumnsInstance.col_data];
        uint32_t counter = (uint32_t) std::stoi(counterSpinButton->get_text());

        // Set the output password
        passwordOutput->set_text(user->passwordForService(serviceName, type, version, counter));
    } else {
        Service &service = (Service &) user->getServices()->at(serviceName);
        passwordOutput->set_text(user->passwordForService(service));
    }
}

void mpw_password_window::updatePasswordVisibility() {
    passwordOutput->set_visibility(!passwordVisibility->get_active());
}

void mpw_password_window::copyPassword() {
    // We have to make the output visible, to be able to copy it
    passwordOutput->set_visibility(true);

    // Select the whole password and copy it to clipboard
    passwordOutput->select_region(0, -1);
    passwordOutput->copy_clipboard();

    // Revert the visibility to the old state
    updatePasswordVisibility();
}
