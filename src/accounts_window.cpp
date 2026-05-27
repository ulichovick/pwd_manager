#include "accounts_window.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "accounts.h"

SqlitoSeguro::accountsWindow::accountsWindow(SqlitoSeguro::accountManager& am, int uid): accountManager(am), uid(uid)
{
    window = new Fl_Window(300, 200, "Accounts");

    usernameInput = new Fl_Input(100, 30, 150, 25, "Username:");
    passwordInput = new Fl_Secret_Input(100, 70, 150, 25, "Password:");
    loginButton = new Fl_Button(100, 110, 80, 30, "Login");
    statusLabel = new Fl_Box(50, 150, 200, 25, "");

}
void SqlitoSeguro::accountsWindow::show()
{
    window->end();
    window->show();
}
