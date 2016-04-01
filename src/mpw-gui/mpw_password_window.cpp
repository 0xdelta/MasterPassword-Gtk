//
// Created by lukas on 09.03.16.
//

#include "mpw_login_window.h"

#include <gtkmm/liststore.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/messagedialog.h>
#include <iostream>

#include "mpw_password_window.h"
#include "simple_columns.h"
#include "password_type.h"
#include "algorithm_version.h"

mpw_password_window *mpw_password_window::create(UserManager *userManager, User *user) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui/password.ui");
    mpw_password_window *window = nullptr;
    builder->get_widget_derived("window", window);
    window->postInit(userManager, user);
    return window;
}

mpw_password_window::mpw_password_window(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        mpw_window(cobject, builder) {
    Gtk::Button *logoutButton, *copyButton;

    // Widgets
    builder->get_widget("service-entry", serviceEntry);
    builder->get_widget("password-output", passwordOutput);
    builder->get_widget("password-visibility", passwordVisibility);
    builder->get_widget("password-type", passwordTypeSelect);
    builder->get_widget("mpw-version", mpwVersionSelect);
    builder->get_widget("counter", counterSpinButton);
    builder->get_widget("modify-site-button", modifySiteButton);
    builder->get_widget("logout", logoutButton);
    builder->get_widget("copy-password", copyButton);

    // Signals
    serviceEntry->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceNameChanged));
    serviceEntry->signal_activate().connect(sigc::mem_fun(this, &mpw_password_window::copyPassword));
    passwordVisibility->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::updatePasswordVisibility));
    passwordTypeSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceSettingsChanged));
    mpwVersionSelect->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceSettingsChanged));
    counterSpinButton->signal_changed().connect(sigc::mem_fun(this, &mpw_password_window::serviceSettingsChanged));
    modifySiteButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::modifySiteButtonClicked));
    logoutButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::logout));
    copyButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_password_window::copyPassword));
}

mpw_password_window::~mpw_password_window() {
    delete user;
}

void mpw_password_window::postInit(UserManager *_userManager, User *_user) {
    userManager = _userManager;
    user = _user;

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
        simpleColumnsInstance.apply(row, {type.name, (int) type.mpSiteType});

        if (type.mpSiteType == defaultPasswordType.mpSiteType) {
            passwordTypeSelect->set_active(row);
        }
    }

    //Fill the ComboBox's Tree Model:
    for (auto &version : algorithmVersions) {
        row = *(mpwVersionsModel->append());
        simpleColumnsInstance.apply(row, {version.displayName, (int) version.mpAlgorithmVersion});

        if (version.mpAlgorithmVersion == defaultAlgorithmVersion.mpAlgorithmVersion) {
            mpwVersionSelect->set_active(row);
        }
    }

    if (user->isIncognito()) {
        // Hide the modify site button
        modifySiteButton->hide();
    } else {
        updateAutoCompletion();
    }
}

Service mpw_password_window::getSelectedService() {
    std::string serviceName = serviceEntry->get_text();

    Gtk::TreeModel::iterator siteTypeItr = passwordTypeSelect->get_active(); // Get pointer to active site type
    Gtk::TreeModel::iterator mpwVersionItr = mpwVersionSelect->get_active(); // Get pointer to active version
    if (!siteTypeItr || !mpwVersionItr) {
        return Service{serviceName};
    }

    Gtk::TreeModel::Row siteTypeRow = *siteTypeItr;
    Gtk::TreeModel::Row mpwVersionRow = *mpwVersionItr;
    if (!siteTypeRow || !mpwVersionRow) {
        return Service{serviceName};
    }

    // Collect the needed data
    MPSiteType type = (MPSiteType) siteTypeRow[simpleColumnsInstance.col_data];
    MPAlgorithmVersion version = (MPAlgorithmVersion) mpwVersionRow[simpleColumnsInstance.col_data];
    uint32_t counter = (uint32_t) std::stoi(counterSpinButton->get_text());

    return Service{serviceName, type, version, (int) counter};
}

void mpw_password_window::updateAutoCompletion() {
// Temporary variable
    Gtk::TreeModel::Row row;

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

void mpw_password_window::setServiceSettings(MPSiteType siteType, MPAlgorithmVersion algorithmVersion, int counter) {
    // Site type
    Gtk::TreeModel::Children types = passwordTypeSelect->get_model()->children();
    for (Gtk::TreeModel::Children::iterator iter = types.begin(); iter != types.end(); ++iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row.get_value(simpleColumnsInstance.col_data) == (int) siteType) {
            passwordTypeSelect->set_active(row);
            break;
        }
    }

    // Version
    Gtk::TreeModel::Children versions = mpwVersionSelect->get_model()->children();
    for (Gtk::TreeModel::Children::iterator iter = versions.begin(); iter != versions.end(); ++iter) {
        Gtk::TreeModel::Row row = *iter;
        if (row.get_value(simpleColumnsInstance.col_data) == (int) algorithmVersion) {
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
    mpw_login_window *loginWindow = mpw_login_window::create(userManager);

    // Add the password window to the gtk application
    get_application()->add_window(*loginWindow);

    // Delete this window
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
        setServiceSettings(defaultPasswordType.mpSiteType, defaultAlgorithmVersion.mpAlgorithmVersion, 1);
    }
    // Show correct button (Remove service/Add service)
    updateModifySiteButton();

    // Display password
    computeAndShowPassword();
}

void mpw_password_window::serviceSettingsChanged() {
    std::string serviceName = serviceEntry->get_text();

    if (!user->isIncognito() && user->getServices()->find(serviceName) != user->getServices()->end()) {
        // The user changed the settings for a service, that is stored
        // in his config file. So we have to update the service, in order
        // to hold the config up to date.

        Service &storedService = (Service &) user->getServices()->at(serviceName);
        Service selectedService = getSelectedService();

        storedService.setType(selectedService.getType());
        storedService.setAlgorithmVersion(selectedService.getAlgorithmVersion());
        storedService.setCounter(selectedService.getCounter());

        userManager->writeUserToConfig(*user);
    }

    computeAndShowPassword();
}

void mpw_password_window::computeAndShowPassword() {
    std::string serviceName = serviceEntry->get_text();
    if (serviceName.size() == 0) {
        passwordOutput->set_text("");
        return;
    }

    if (user->isIncognito() || user->getServices()->find(serviceName) == user->getServices()->end()) {
        // No stored service found.
        Service selectedService = getSelectedService();
        passwordOutput->set_text(user->passwordForService(selectedService));
    } else {
        // Show the stored service
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

void mpw_password_window::updateModifySiteButton() {
    if (!user->isIncognito()) {
        std::string serviceName = serviceEntry->get_text();

        if (user->getServices()->find(serviceName) != user->getServices()->end()) {
            modifySiteButton->set_label("Remove");
        } else {
            modifySiteButton->set_label("Add");
        }
    }
}

void mpw_password_window::modifySiteButtonClicked() {
    if (!user->isIncognito()) {
        std::string serviceName = serviceEntry->get_text();

        if (user->getServices()->find(serviceName) != user->getServices()->end()) {
            // Remove the service
            user->removeService(serviceName);
        } else {
            // Add the service
            Service service = getSelectedService();
            user->addService(service);
        }
        userManager->writeUserToConfig(*user);

        updateModifySiteButton();
        updateAutoCompletion();
    }
}
