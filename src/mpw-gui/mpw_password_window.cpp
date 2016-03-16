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

static simple_row_data passwordTypes[] = {
        {0x0, "Maximum", MPSiteTypeGeneratedMaximum},
        {0x1, "Long",    MPSiteTypeGeneratedLong},
        {0x2, "Medium",  MPSiteTypeGeneratedMedium},
        {0x3, "Basic",   MPSiteTypeGeneratedBasic},
        {0x4, "Short",   MPSiteTypeGeneratedShort},
        {0x5, "PIN",     MPSiteTypeGeneratedPIN},
        {0xE, "Name",    MPSiteTypeGeneratedName},
        {0xF, "Phrase",  MPSiteTypeGeneratedPhrase}
};
static simple_row_data defaultPasswordType = passwordTypes[1]; // Generated long

static simple_row_data mpwVersions[] = {
        {1, "V1", MPAlgorithmVersion1},
        {2, "V2", MPAlgorithmVersion2},
        {3, "V3", MPAlgorithmVersion3},
};
static simple_row_data defaultMpwVersion = mpwVersions[2]; // V3

mpw_password_window::mpw_password_window(mpw_user *_user) :
        user(_user) {
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
    serviceEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::compute_and_show_password));
    serviceEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_password_window::copy_password));
    passwordVisibility->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::update_password_visibility));
    passwordTypeSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::compute_and_show_password));
    mpwVersionSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::compute_and_show_password));
    counterSpinButton->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::compute_and_show_password));
    logoutButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::logout));
    copyButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::copy_password));

    // Create the models for ComboBoxes
    Glib::RefPtr<Gtk::ListStore> passwordTypesModel = Gtk::ListStore::create(simple_columns_instance);
    Glib::RefPtr<Gtk::ListStore> mpwVersionsModel = Gtk::ListStore::create(simple_columns_instance);

    // Use the models for our ComboBoxes
    passwordTypeSelect->set_model(passwordTypesModel);
    mpwVersionSelect->set_model(mpwVersionsModel);

    // Configure which elements are shown within the ComboBoxes
    passwordTypeSelect->pack_start(simple_columns_instance.col_name);
    mpwVersionSelect->pack_start(simple_columns_instance.col_name);

    // Temporary variable
    Gtk::TreeModel::Row row;

    // Fill the password types model
    for (auto type : passwordTypes) {
        row = *(passwordTypesModel->append());
        simple_columns_instance.apply(row, type);

        if (type.id == defaultPasswordType.id) {
            passwordTypeSelect->set_active(row);
        }
    }

    //Fill the ComboBox's Tree Model:
    for (auto type : mpwVersions) {
        row = *(mpwVersionsModel->append());
        simple_columns_instance.apply(row, type);

        if (type.id == defaultMpwVersion.id) {
            mpwVersionSelect->set_active(row);
        }
    }

    // Completion for the service entry
    auto completion = Gtk::EntryCompletion::create();
    auto autoCompleteModel = Gtk::ListStore::create(simple_columns_instance);

    serviceEntry->set_completion(completion);
    completion->set_model(autoCompleteModel);
    completion->set_text_column(simple_columns_instance.col_name);

    for (auto &site : user->getServices()) {
        row = *(autoCompleteModel->append());
        simple_columns_instance.apply(row, {0, site.getName(), 0});
    }
}

mpw_password_window::~mpw_password_window() {
    delete user;
}

void mpw_password_window::logout() {
    // Create the login window
    mpw_login_window *loginWindow = new mpw_login_window{};

    // Add the password window to the gtk application
    window->get_application()->add_window(*loginWindow->getWindow());

    // Hide and delete this window
    window->hide();
    delete this;
}

void mpw_password_window::compute_and_show_password() {
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
    Glib::ustring service = serviceEntry->get_text();
    MPSiteType type = (MPSiteType) siteTypeRow[simple_columns_instance.col_data];
    MPAlgorithmVersion version = (MPAlgorithmVersion) mpwVersionRow[simple_columns_instance.col_data];
    uint32_t counter = (uint32_t) std::stoi(counterSpinButton->get_text());

    // Set the output password
    if (service.size() == 0) {
        passwordOutput->set_text("");
    } else {
        try {
            passwordOutput->set_text(user->passwordForService(service, type, version, counter));
        } catch (password_generate_exception &e) {
            passwordOutput->set_text("");

            Gtk::MessageDialog dialog(*window, "Error", false, Gtk::MESSAGE_ERROR);
            dialog.set_secondary_text("Could not generate password: " + std::string{e.what()});
            dialog.run();
        }
    }
}

void mpw_password_window::update_password_visibility() {
    passwordOutput->set_visibility(!passwordVisibility->get_active());
}

void mpw_password_window::copy_password() {
    // We have to make the output visible, to be able to copy it
    passwordOutput->set_visibility(true);

    // Select the whole password and copy it to clipboard
    passwordOutput->select_region(0, -1);
    passwordOutput->copy_clipboard();

    // Revert the visibility to the old state
    update_password_visibility();
}
