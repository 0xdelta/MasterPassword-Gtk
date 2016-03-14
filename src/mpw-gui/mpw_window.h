//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_MPW_WINDOW_H
#define MPW_GTK_MPW_WINDOW_H


#include <gtkmm/window.h>

class mpw_window {
protected:
    Gtk::Window *window;

public:
    ~mpw_window();

    Gtk::Window *getWindow();
};


#endif //MPW_GTK_MPW_WINDOW_H
