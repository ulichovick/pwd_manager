#ifndef NEW_USER_H
#define NEW_USER_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

#include "users.h"

namespace SqlitoSeguro
{
    class createUserWindow
    {
    private:
        Fl_Window* window{nullptr};

        Fl_Input* usernameInput{nullptr};
        Fl_Secret_Input* passwordInput{nullptr};
        Fl_Secret_Input* confirmPasswordInput{nullptr};

        Fl_Button* createButton{nullptr};
        Fl_Box* statusLabel{nullptr};

        SqlitoSeguro::userManager & userManager;

        static void onCreateUser(Fl_Widget*, void* data);
        void handleCreateUser();

    public:
        createUserWindow(SqlitoSeguro::userManager& um);

        void show();
    };
}

#endif
