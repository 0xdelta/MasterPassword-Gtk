#include <gtkmm/application.h>
#include "mpw_login_window.h"
#include "mpw_password_window.h"

int main(int argc, char *argv[]) {
    // Init gtk
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    // Init the login window
    mpw_login_window *loginWindow = new mpw_login_window{};

    // Launch the application
    return app->run(*loginWindow->getWindow());
}