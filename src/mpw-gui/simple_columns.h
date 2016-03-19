//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_SIMPLE_COLUMN_RECORD_H
#define MPW_GTK_SIMPLE_COLUMN_RECORD_H

#include <gtkmm/treemodel.h>
#include <glibmm/ustring.h>
#include <gtkmm/treemodelcolumn.h>


struct SimpleRowData {
    Glib::ustring name;
    int data;
};

class SimpleColumns : public Gtk::TreeModel::ColumnRecord {
public:
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<int> col_data;

    SimpleColumns();

    void apply(Gtk::TreeModel::Row row, SimpleRowData data);
};

static SimpleColumns simpleColumnsInstance;

#endif //MPW_GTK_SIMPLE_COLUMN_RECORD_H
