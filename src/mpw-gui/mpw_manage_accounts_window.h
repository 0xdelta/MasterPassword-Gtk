//
// Created by lukas on 21.03.16.
//

#ifndef MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H
#define MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H

#include <gtkmm/treeview.h>
#include <user_manager.h>
#include "mpw_window.h"

class AccountsColumns : public Gtk::TreeModel::ColumnRecord {
public:
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<Glib::ustring> col_file;

    AccountsColumns() {
        add(col_name);
        add(col_file);
    }

    void apply(Gtk::TreeModel::Row row, Glib::ustring name, Glib::ustring file) {
        row.set_value(col_name, name);
        row.set_value(col_file, file);
    }
};

class mpw_manage_accounts_window : public mpw_window {
private:
    UserManager *userManager;

    AccountsColumns accountsColumns;
    Gtk::TreeView *accountsTreeView;

public:
    mpw_manage_accounts_window(UserManager *userManager);

    void updateAccountsTreeView();
};

#endif //MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H
