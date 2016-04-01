//
// Created by lukas on 01.04.16.
//

#include "mpw_window.h"

mpw_window::mpw_window(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::Window(cobject) {
    signal_delete_event().connect(sigc::mem_fun(this, &mpw_window::mpw_delete_event_handler));
}

bool mpw_window::mpw_delete_event_handler(GdkEventAny *any_event) {
    delete this;
    return true;
}