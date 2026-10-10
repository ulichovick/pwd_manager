#include "new_user.h"

SqlitoSeguro::createUserWindow::createUserWindow(SqlitoSeguro::userManager& um):
    userManager(um)
{
    window = new Fl_Window(320, 240, "Create User");

    usernameInput =
        new Fl_Input(110, 30, 170, 25, "Username:");

    passwordInput =
        new Fl_Secret_Input(110, 70, 170, 25, "Password:");

    confirmPasswordInput =
        new Fl_Secret_Input(110, 110, 170, 25, "Confirm:");

    createButton =
        new Fl_Button(110, 150, 110, 30, "Create User");

    statusLabel =
        new Fl_Box(20, 195, 280, 25, "");

    createButton->callback(onCreateUser, this);

    window->end();
}

void SqlitoSeguro::createUserWindow::onCreateUser(
    Fl_Widget*,
    void* data)
{
    auto* self =
        static_cast<createUserWindow*>(data);

    self->handleCreateUser();
}

void SqlitoSeguro::createUserWindow::handleCreateUser()
{
    std::string username = usernameInput->value();
    std::string password = passwordInput->value();
    std::string confirmation = confirmPasswordInput->value();

    if (username.empty())
    {
        statusLabel->label("Username cannot be empty.");
        return;
    }

    if (password.empty())
    {
        statusLabel->label("Password cannot be empty.");
        return;
    }

    if (password != confirmation)
    {
        statusLabel->label("Passwords do not match.");
        return;
    }

    try
    {
        userManager.createUser(username, password);

        statusLabel->label("User created successfully!");

        usernameInput->value("");
        passwordInput->value("");
        confirmPasswordInput->value("");
    }
    catch (const std::exception& e)
    {
        statusLabel->label(e.what());
    }
}

void SqlitoSeguro::createUserWindow::show()
{
    window->show();
}
