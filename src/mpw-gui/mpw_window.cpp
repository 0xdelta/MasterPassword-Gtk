//
// Created by lukas on 09.03.16.
//

#include "mpw_window.h"

Gtk::Window *mpw_window::getWindow() {
    return window;
}

mpw_window::~mpw_window() {
    delete window;
}
