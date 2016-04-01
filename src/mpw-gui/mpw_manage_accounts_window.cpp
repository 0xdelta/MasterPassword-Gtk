//
// Created by lukas on 21.03.16.
//

#include "mpw_manage_accounts_window.h"

#include <gtkmm/liststore.h>
#include <gtkmm/dialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <iostream>

mpw_manage_accounts_window *mpw_manage_accounts_window::create(UserManager *userManager) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui/manage-accounts.ui");
    mpw_manage_accounts_window *window = nullptr;
    builder->get_widget_derived("window", window);
    window->postInit(userManager);
    return window;
}

mpw_manage_accounts_window::mpw_manage_accounts_window(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
        mpw_window(cobject, builder) {
    // Widgets
    builder->get_widget("accounts-tree-view", accountsTreeView);
    builder->get_widget("import", importButton);
    builder->get_widget("finish", finishButton);

    // Signals
    accountsTreeView->signal_button_press_event().connect_notify(sigc::mem_fun(this, &mpw_manage_accounts_window::onButtonPress));
    importButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_manage_accounts_window::import));
    finishButton->signal_clicked().connect(sigc::mem_fun(this, &mpw_manage_accounts_window::finish));

    // Configure popup menu
    popupMenu.accelerate(*this);
    auto item = Gtk::manage(new Gtk::MenuItem("Edit", true));
    item->signal_activate().connect(sigc::mem_fun(*this, &mpw_manage_accounts_window::editUser));
    popupMenu.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("Delete", true));
    item->signal_activate().connect(sigc::mem_fun(*this, &mpw_manage_accounts_window::deleteUser));
    popupMenu.append(*item);
    popupMenu.show_all();

    // Configure the tree view
    accountsTreeView->append_column("Name", accountsColumns.col_name);
    accountsTreeView->append_column("File", accountsColumns.col_file);
}

void mpw_manage_accounts_window::postInit(UserManager *_userManager) {
    userManager = _userManager;

    updateAccountsTreeView();
}

void mpw_manage_accounts_window::updateAccountsTreeView() {
    //Create the Tree model:
    Glib::RefPtr<Gtk::ListStore> accountsModel = Gtk::ListStore::create(accountsColumns);
    accountsTreeView->set_model(accountsModel);

    //Fill the TreeView's model
    Gtk::TreeModel::Row row;
    for (auto pair : userManager->getAvailableUsers()) {
        Glib::ustring name = pair.first;
        Glib::ustring file = pair.second;

        row = *(accountsModel->append());
        accountsColumns.apply(row, name, file);
    }
}

void mpw_manage_accounts_window::onButtonPress(GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        popupMenu.popup(event->button, event->time);
    }
}

void mpw_manage_accounts_window::editUser() {
    auto refSelection = accountsTreeView->get_selection();
    if (!refSelection)
        return;
    Gtk::TreeModel::iterator itr = refSelection->get_selected();
    if (!itr)
        return;

    Gtk::TreeModel::Row row = *itr;
    std::string userName = row.get_value(accountsColumns.col_name);

    // Open a file chooser
    Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.set_current_folder(getenv("HOME"));

    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();
    if (result == Gtk::RESPONSE_OK) {
        std::string fileName = dialog.get_filename();
        userManager->setUserFile(userName, fileName);

        updateAccountsTreeView();
    }
}

void mpw_manage_accounts_window::deleteUser() {
    auto refSelection = accountsTreeView->get_selection();
    if (!refSelection)
        return;
    Gtk::TreeModel::iterator itr = refSelection->get_selected();
    if (!itr)
        return;

    Gtk::TreeModel::Row row = *itr;
    std::string userName = row.get_value(accountsColumns.col_name);
    userManager->deleteUser(userName);

    updateAccountsTreeView();
}

void mpw_manage_accounts_window::finish() {
    userManager->writeToConfig();

    delete this;
}

void mpw_manage_accounts_window::import() {
    // Open a file chooser
    Gtk::FileChooserDialog fileChooser("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    fileChooser.set_transient_for(*this);
    fileChooser.set_current_folder(getenv("HOME"));

    fileChooser.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    fileChooser.add_button("Select", Gtk::RESPONSE_OK);

    int result = fileChooser.run();
    if (result == Gtk::RESPONSE_OK) {
        std::string fileName = fileChooser.get_filename();
        if (!userManager->importUser(fileName)) {
            Gtk::MessageDialog dialog(*this, "Error", false, Gtk::MESSAGE_ERROR);
            dialog.set_secondary_text("Could not import " + fileName + ".\n\nSee log for details.");
            dialog.run();
        }

        updateAccountsTreeView();
    }
}
