#ifndef ACCOUNTS_WINDOW_H
#define ACCOUNTS_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Scroll.H>
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
        Fl_Scroll* scroll_container;

        SqlitoSeguro::accountManager & accountManager;
        int uid;
        std::string username;

    public:
        accountsWindow(SqlitoSeguro::accountManager& am, int uid, std::string_view username);
        void show();
    };
    
}

#endif
