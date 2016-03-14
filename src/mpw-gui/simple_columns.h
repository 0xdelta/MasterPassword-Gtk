//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_SIMPLE_COLUMN_RECORD_H
#define MPW_GTK_SIMPLE_COLUMN_RECORD_H

#include <gtkmm/treemodel.h>
#include <glibmm/ustring.h>
#include <gtkmm/treemodelcolumn.h>


struct simple_row_data {
    int id;
    Glib::ustring name;
    int data;
};

class simple_columns : public Gtk::TreeModel::ColumnRecord {
public:
    Gtk::TreeModelColumn<int> col_id;
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<int> col_data;

    simple_columns();

    void apply(Gtk::TreeModel::Row row, simple_row_data data);
};

static simple_columns simple_columns_instance;

#endif //MPW_GTK_SIMPLE_COLUMN_RECORD_H
