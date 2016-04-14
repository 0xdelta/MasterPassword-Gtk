#include <gtkmm/application.h>
#include <user_manager.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include "mpw_login_window.h"
#include "mpw_password_window.h"

std::string getResourceDir() {
    return "/usr/share/mpw-gtk";
}

int main(int argc, char *argv[]) {
    // Init gtk (this has to be done before any window is shown)
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    UserManager userManager;
    if (!userManager.readFromConfig()) {
        Gtk::MessageDialog dialog("Error", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Could not read main config. \n\nSee log for more details.");
        dialog.run();
        return 1;
    }

    // Init the login window
    mpw_login_window *loginWindow = mpw_login_window::create(&userManager);

    // Launch the application
    return app->run(*loginWindow);
}