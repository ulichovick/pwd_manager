#include "login.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "users.h"

#include "accounts_window.h"

SqlitoSeguro::loginWindow::loginWindow(SqlitoSeguro::userManager& um, SqlitoSeguro::accountManager& am): userManager(um), accountManager(am)
{
    window = new Fl_Window(300, 200, "Login");

    usernameInput = new Fl_Input(100, 30, 150, 25, "Username:");
    passwordInput = new Fl_Secret_Input(100, 70, 150, 25, "Password:");
    loginButton = new Fl_Button(100, 110, 80, 30, "Login");
    statusLabel = new Fl_Box(50, 150, 200, 25, "");

    loginButton->callback(onLogin, this);

}

void SqlitoSeguro::loginWindow::onLogin(Fl_Widget*, void* data)
{
    auto* self = static_cast<loginWindow*>(data);
    self->handleLogin();
}

void SqlitoSeguro::loginWindow::handleLogin()
{
    std::string username = usernameInput->value();
    std::string password = passwordInput->value();

    try
    {
        auto userId = userManager.Authenticate(username, password);
        if (userId)
        {
            window->hide();
            auto* accWin = new SqlitoSeguro::accountsWindow(accountManager, *userId, username);
        }
        else
        {
            statusLabel->label("Invalid Credentials!");
        }
    }
    catch(const std::exception& e)
    {
        
        statusLabel->label(e.what());
        
    }
    

}
void SqlitoSeguro::loginWindow::show()
{
    window->end();
    window->show();
}
