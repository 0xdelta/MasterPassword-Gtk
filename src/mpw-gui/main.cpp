#include <gtkmm/application.h>
#include <user_manager.h>
#include <gtkmm/messagedialog.h>
#include "mpw_login_window.h"
#include "mpw_password_window.h"

int main(int argc, char *argv[]) {
    UserManager *userManager = new UserManager;
    if (!userManager->readFromConfig()) {
        Gtk::MessageDialog dialog("Error", false, Gtk::MESSAGE_ERROR);
        dialog.set_secondary_text("Could not read main config. \n\nSee log for more details.");
        dialog.run();

        return 1;
    }

    // Init gtk
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    // Init the login window
    mpw_login_window *loginWindow = new mpw_login_window{userManager};

    // Launch the application
    return app->run(*loginWindow->getWindow());
}