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

static SimpleRowData passwordTypes[] = {
        {0x0, "Maximum", MPSiteTypeGeneratedMaximum},
        {0x1, "Long",    MPSiteTypeGeneratedLong},
        {0x2, "Medium",  MPSiteTypeGeneratedMedium},
        {0x3, "Basic",   MPSiteTypeGeneratedBasic},
        {0x4, "Short",   MPSiteTypeGeneratedShort},
        {0x5, "PIN",     MPSiteTypeGeneratedPIN},
        {0xE, "Name",    MPSiteTypeGeneratedName},
        {0xF, "Phrase",  MPSiteTypeGeneratedPhrase}
};
static SimpleRowData defaultPasswordType = passwordTypes[1]; // Generated long

static SimpleRowData mpwVersions[] = {
        {1, "V1", MPAlgorithmVersion1},
        {2, "V2", MPAlgorithmVersion2},
        {3, "V3", MPAlgorithmVersion3},
};
static SimpleRowData defaultMpwVersion = mpwVersions[2]; // V3

mpw_password_window::mpw_password_window(UserManager *_userManager, User *_usr) :
        user(_usr), userManager(_userManager) {
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
    serviceEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::computeAndShowPassword));
    serviceEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_password_window::copyPassword));
    passwordVisibility->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::updatePasswordVisibility));
    passwordTypeSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::computeAndShowPassword));
    mpwVersionSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::computeAndShowPassword));
    counterSpinButton->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::computeAndShowPassword));
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
    for (auto type : passwordTypes) {
        row = *(passwordTypesModel->append());
        simpleColumnsInstance.apply(row, type);

        if (type.id == defaultPasswordType.id) {
            passwordTypeSelect->set_active(row);
        }
    }

    //Fill the ComboBox's Tree Model:
    for (auto type : mpwVersions) {
        row = *(mpwVersionsModel->append());
        simpleColumnsInstance.apply(row, type);

        if (type.id == defaultMpwVersion.id) {
            mpwVersionSelect->set_active(row);
        }
    }

    // Completion for the service entry
    auto completion = Gtk::EntryCompletion::create();
    auto autoCompleteModel = Gtk::ListStore::create(simpleColumnsInstance);

    serviceEntry->set_completion(completion);
    completion->set_model(autoCompleteModel);
    completion->set_text_column(simpleColumnsInstance.col_name);

    for (auto &site : user->getServices()) {
        row = *(autoCompleteModel->append());
        simpleColumnsInstance.apply(row, {0, site.getName(), 0});
    }
}

mpw_password_window::~mpw_password_window() {
    delete user;
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

void mpw_password_window::computeAndShowPassword() {
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
    std::string service = serviceEntry->get_text();
    MPSiteType type = (MPSiteType) siteTypeRow[simpleColumnsInstance.col_data];
    MPAlgorithmVersion version = (MPAlgorithmVersion) mpwVersionRow[simpleColumnsInstance.col_data];
    uint32_t counter = (uint32_t) std::stoi(counterSpinButton->get_text());

    // Set the output password
    if (service.size() == 0) {
        passwordOutput->set_text("");
    } else {
        //try {
            passwordOutput->set_text(user->passwordForService(service, type, version, counter));
        /*} catch (password_generate_exception &e) {
            passwordOutput->set_text("");

            Gtk::MessageDialog dialog(*window, "Error", false, Gtk::MESSAGE_ERROR);
            dialog.set_secondary_text("Could not generate password: " + std::string{e.what()});
            dialog.run();
        }*/ //TODO
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
