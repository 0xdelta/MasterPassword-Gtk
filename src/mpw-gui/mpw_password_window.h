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

#include "mpw_window.h"
#include "user.h"

/**
 * This is the window, that is shown, after the user entered
 * his credentials in the login window.
 */
class mpw_password_window : public mpw_window {
public:
    static mpw_password_window *create(UserManager *userManager, User *user);

private:
    UserManager *userManager;
    User *user;

    Gtk::Entry *serviceEntry, *passwordOutput;
    Gtk::CheckButton *passwordVisibility;
    Gtk::ComboBox *passwordTypeSelect, *mpwVersionSelect;
    Gtk::SpinButton *counterSpinButton;
    Gtk::Button *modifySiteButton;

public:
    mpw_password_window(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~mpw_password_window();

private:
    /**
     * Called after the object is instantiated. The method
     * is used to pass the user manager and the user object to
     * this object.
     */
    void postInit(UserManager *userManager, User *user);

    /**
     * Create an new Service instance that represents
     * the current input values.
     */
    Service getSelectedService();
    /**
     * Update the auto completion for services.
     */
    void updateAutoCompletion();
    /**
     * Updates the input settings for a service to the given values.
     */
    void setServiceSettings(MPSiteType siteType, MPAlgorithmVersion algorithmVersion, int counter);
    /**
     * Logout button clicked.
     */
    void logout();
    /**
     * Called when the service name changed.
     */
    void serviceNameChanged();
    /**
     * Called when any service setting is changed.
     * That includes password type, version and counter.
     */
    void serviceSettingsChanged();
    /**
     * Computes and displays the generated password in
     * the password field.
     */
    void computeAndShowPassword();
    /**
     * Updates the password visibility relative to the password visibility
     * button.
     */
    void updatePasswordVisibility();
    /**
     * Copies the current password to clipboard.
     */
    void copyPassword();
    /**
     * Update the button that is on the right of the service entry.
     * It changes the displayed text to "Add"/"Remove"
     */
    void updateModifySiteButton();
    /**
     * The "Add"/"Remove" button was clicked.
     */
    void modifySiteButtonClicked();
};

#endif //MPW_GTK_MPW_PASSWORD_WINDOW_H
