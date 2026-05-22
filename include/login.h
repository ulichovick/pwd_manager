#ifndef LOGIN_H
#define LOGIN_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "users.h"

namespace SqlitoSeguro
{
    class login
    {
        Fl_Window* window;

        Fl_Input* usernameInput;
        Fl_Secret_Input* passwordInput;
        Fl_Button* loginButton;
        Fl_Box* statusLabel;

        SqlitoSeguro::userManager& userManager;
        static void onLogin(Fl_Widget*, void* data);
        void handleLogin();

    public:
        login(SqlitoSeguro::userManager& um);
        void show();
        ~login();
        
    };
    
}

#endif
