//
// Created by lukas on 21.03.16.
//

#ifndef MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H
#define MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H

#include <gtkmm/treeview.h>
#include <user_manager.h>
#include "mpw_window.h"

class mpw_manage_accounts_window : public mpw_window {
private:
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


private:
    UserManager *userManager;

    AccountsColumns accountsColumns;
    Gtk::TreeView *accountsTreeView;
    Gtk::Button *importButton, *finishButton;

    Gtk::Menu popupMenu;

public:
    mpw_manage_accounts_window(UserManager *userManager);

private:
    void updateAccountsTreeView();
    void onButtonPress(GdkEventButton* event);

    void editUser();
    void deleteUser();

    void import();
    void finish();
};

#endif //MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H
