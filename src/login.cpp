#include "login.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "users.h"

SqlitoSeguro::login::login(SqlitoSeguro::userManager& um)
{
    window = new Fl_Window(300, 200, "Login");

    usernameInput = new Fl_Input(100, 30, 150, 25, "Username:");
    passwordInput = new Fl_Secret_Input(100, 30, 150, 25, "Username:");
    loginButton = new Fl_Button(100, 110, 80, 30, "Login");
    statusLabel = new Fl_Box(50, 150, 200, 25, "");

    loginButton->callback(onLogin, this);

}

void SqlitoSeguro::login::onLogin(Fl_Widget*, void* data)
{
    auto* self = static_cast<login*>(data);
    self->handleLogin();
}

void SqlitoSeguro::login::handleLogin()
{
    std::string username = usernameInput->value();
    std::string password = passwordInput->value();

    try
    {
        auto userId = userManager.Authenticate()
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}
