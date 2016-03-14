//
// Created by lukas on 10.03.16.
//

#include "simple_columns.h"

simple_columns::simple_columns() {
    add(col_id);
    add(col_name);
    add(col_data);
}

void simple_columns::apply(Gtk::TreeModel::Row row, simple_row_data data){
    row.set_value(col_id, data.id);
    row.set_value(col_name, data.name);
    row.set_value(col_data, data.data);
}
