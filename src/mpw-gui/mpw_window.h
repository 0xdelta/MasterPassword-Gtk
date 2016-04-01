//
// Created by lukas on 01.04.16.
//

#ifndef MPW_GTK_MPW_WINDOW_H
#define MPW_GTK_MPW_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/builder.h>

class mpw_window : public Gtk::Window {
public:
    mpw_window(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);

private:
    /**
     * The delete event is triggered, when a user requests a top level
     * container (in this case a window) to be closed (via the X button).
     *
     * By default, the implementation of the handler in this class
     * deletes (now in c++ way) this instance (i.e. delete this)
     */
    bool mpw_delete_event_handler(GdkEventAny *any_event);
};


#endif //MPW_GTK_MPW_WINDOW_H
