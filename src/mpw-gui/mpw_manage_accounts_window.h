//
// Created by lukas on 21.03.16.
//

#ifndef MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H
#define MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H

#include <gtkmm/treeview.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>

#include "mpw_window.h"
#include "user_manager.h"

/**
 * Within this window, the user can import, edit and delete users.
 */
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
public:
    static mpw_manage_accounts_window *create(UserManager *userManager);

private:
    UserManager *userManager;

    AccountsColumns accountsColumns;
    Gtk::TreeView *accountsTreeView;
    Gtk::Button *importButton, *finishButton;

    // The popup menu that is shown, when a user right-clicks
    // a user in the tree view
    Gtk::Menu popupMenu;

public:
    mpw_manage_accounts_window(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

private:
    /**
     * Called after the object is instantiated. The method is used
     * to pass the user manager to this object.
     */
    void postInit(UserManager *userManager);

    /**
     * Update the users shown in the tree view.
     */
    void updateAccountsTreeView();
    /**
     * Called when a button is clicked within the
     * tree view.
     */
    void onButtonPress(GdkEventButton* event);

    /**
     * Called when edit is clicked.
     */
    void editUser();
    /**
     * Called when delete is clicked.
     */
    void deleteUser();

    /**
     * Called when the import button is clicked.
     */
    void import();
    /**
     * Called when the finish button is clicked.
     */
    void finish();
};

#endif //MPW_GTK_MPW_MANAGE_ACCOUNTS_WINDOW_H
