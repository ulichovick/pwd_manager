#include "login.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "users.h"
#include "new_user.h"
#include "accounts_window.h"

SqlitoSeguro::loginWindow::loginWindow(SqlitoSeguro::userManager& um, SqlitoSeguro::accountManager& am, SqlitoSeguro::session& cus ): userManager(um), accountManager(am), currentSession(cus)
{
    window = new Fl_Window(360, 240, "Login");

    titleLabel = new Fl_Box(80, 15, 200, 25, "Password Manager");
    usernameInput = new Fl_Input(120, 55, 190, 25, "Username:");
    passwordInput = new Fl_Secret_Input(120, 95, 190, 25, "Password:");
    loginButton = new Fl_Button(75, 140, 90, 30, "Login");
    createUserButton = new Fl_Button(180, 140, 105, 30, "New User");
    statusLabel = new Fl_Box(40, 185, 280, 25, "");

    titleLabel->labelfont(FL_BOLD);
    titleLabel->labelsize(18);
    statusLabel->align(FL_ALIGN_CENTER);

    loginButton->callback(onLogin, this);
    createUserButton->callback(onCreateUser, this);
    loginButton->shortcut(FL_Enter);

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
            currentSession.userId = *userId;
            window->hide();
            auto* accWin = new SqlitoSeguro::accountsWindow(accountManager, username, currentSession);
            accWin->refreshAccounts();
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

void SqlitoSeguro::loginWindow::onCreateUser(Fl_Widget*, void* data)
{
    auto* self = static_cast<loginWindow*>(data);
    self->newUserWindow = std::make_unique<SqlitoSeguro::createUserWindow>(self->userManager);
    self->newUserWindow->show();
}

void SqlitoSeguro::loginWindow::show()
{
    window->end();
    window->show();
}
