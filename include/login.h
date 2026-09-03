#ifndef LOGIN_H
#define LOGIN_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "users.h"
#include "accounts.h"
#include "session.h"


namespace SqlitoSeguro
{
    class loginWindow
    {
        Fl_Window* window;

        Fl_Input* usernameInput;
        Fl_Secret_Input* passwordInput;
        Fl_Button* loginButton;
        Fl_Box* statusLabel;

        SqlitoSeguro::userManager& userManager;
        SqlitoSeguro::accountManager& accountManager;
        SqlitoSeguro::session& currentSession;
        static void onLogin(Fl_Widget*, void* data);
        void handleLogin();

    public:
        loginWindow(SqlitoSeguro::userManager& um, SqlitoSeguro::accountManager& am, SqlitoSeguro::session& cus);
        void show();
    };
    
}

#endif
