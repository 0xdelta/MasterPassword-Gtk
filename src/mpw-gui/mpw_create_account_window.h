//
// Created by lukas on 13.03.16.
//

#ifndef MPW_GTK_MPW_CREATE_ACCOUNT_WINDOW_H
#define MPW_GTK_MPW_CREATE_ACCOUNT_WINDOW_H

#include <gtkmm/entry.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>

#include "mpw_window.h"
#include "user_manager.h"

/**
 * Within this window, the user can create a new account.
 */
class mpw_create_account_window : public mpw_window {
public:
    static mpw_create_account_window *create(UserManager *userManager);

private:
    UserManager *userManager;
    Gtk::ProgressBar *passwordStrength;
    Gtk::Entry *userEntry, *passwordEntry, *repeatPasswordEntry;
    Gtk::Button *cancelButton,*createButton;
public:
    mpw_create_account_window(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

private:
    /**
     * Updates the progress bar, that helps to determine,
     * how strong the password is.
     */
    void updatePasswordStrength();
    /**
     * Update the sensitivity of the create button.
     */
    void updateCreateButton();
    /**
     * Cancel button clicked.
     */
    void cancelButtonClicked();
    /**
     * Create button clicked.
     */
    void createButtonClicked();
};


#endif //MPW_GTK_MPW_CREATE_ACCOUNT_WINDOW_H
