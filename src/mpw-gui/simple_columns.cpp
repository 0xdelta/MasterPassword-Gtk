//
// Created by lukas on 10.03.16.
//

#include "simple_columns.h"

SimpleColumns::SimpleColumns() {
    add(col_id);
    add(col_name);
    add(col_data);
}

void SimpleColumns::apply(Gtk::TreeModel::Row row, SimpleRowData data){
    row.set_value(col_id, data.id);
    row.set_value(col_name, data.name);
    row.set_value(col_data, data.data);
}
