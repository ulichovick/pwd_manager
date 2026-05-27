#ifndef ACCOUNTS_WINDOW_H
#define ACCOUNTS_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "accounts.h"


namespace SqlitoSeguro
{
    class accountsWindow
    {
        Fl_Window* window;

        Fl_Input* usernameInput;
        Fl_Secret_Input* passwordInput;
        Fl_Button* loginButton;
        Fl_Box* statusLabel;

        SqlitoSeguro::accountManager & accountManager;
        int uid;

    public:
        accountsWindow(SqlitoSeguro::accountManager& am, int uid);
        void show();
    };
    
}

#endif
