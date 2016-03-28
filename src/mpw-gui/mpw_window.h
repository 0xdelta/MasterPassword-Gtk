//
// Created by lukas on 09.03.16.
//

#ifndef MPW_GTK_MPW_WINDOW_H
#define MPW_GTK_MPW_WINDOW_H


#include <gtkmm/window.h>

/**
 * The base class of any window. This class just holds
 * a Gtk Window, that will be deleted in the dtor.
 */
class mpw_window {
protected:
    Gtk::Window *window;

public:
    virtual ~mpw_window();

    Gtk::Window *getWindow();
};


#endif //MPW_GTK_MPW_WINDOW_H
