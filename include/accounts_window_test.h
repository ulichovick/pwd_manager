#ifndef ACCOUNTS_WINDOW_TEST_H
#define ACCOUNTS_WINDOW_TEST_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Hold_Browser.H>
#include "accounts.h"


namespace SqlitoSeguro
{
    class accountsWindowtest
    {
        Fl_Window* window;
        Fl_Hold_Browser* browser;
        Fl_Output* nameOutput;
        Fl_Output* usernameOutput;
        Fl_Output* passwordOutput;
        Fl_Output* urlOutput;

        Fl_Button* addButton;
        Fl_Button* editButton;
        Fl_Button* deleteButton;
        Fl_Button* copyUserButton;
        Fl_Button* copyPassButton;
        Fl_Button* logoutButton;

        struct outputs{
        Fl_Output* nameOutput;
        Fl_Output* usernameOutput;
        Fl_Output* passwordOutput;
        Fl_Output* urlOutput;
        int uid;
        };

        SqlitoSeguro::accountManager & accountManager;
        int uid;
        std::string username;
        static void browser_callback(Fl_Widget* widget, void* data);

    public:
        accountsWindowtest(SqlitoSeguro::accountManager& am, int uid, std::string_view username);
        
        void show();
    };
    
}

#endif
