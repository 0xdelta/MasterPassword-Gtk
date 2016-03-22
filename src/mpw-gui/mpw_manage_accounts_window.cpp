//
// Created by lukas on 21.03.16.
//

#include "mpw_manage_accounts_window.h"

#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>

mpw_manage_accounts_window::mpw_manage_accounts_window(UserManager *_userManager) :
        userManager(_userManager) {
    auto builder = Gtk::Builder::create_from_file("ui/manage-accounts.ui");

    // Widgets
    builder->get_widget("window", window);
    builder->get_widget("accounts-tree-view", accountsTreeView);

    // Signals

    // Configure the tree view
    accountsTreeView->append_column("Name", accountsColumns.col_name);
    accountsTreeView->append_column("File", accountsColumns.col_file);

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

