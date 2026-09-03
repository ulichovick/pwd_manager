#ifndef ACCOUNT_FORM_WINDOW_H
#define ACCOUNT_FORM_WINDOW_H


#include <string>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include "accounts.h"
#include "session.h"

namespace SqlitoSeguro
{

    class AccountFormWindow
    {
    private:
        SqlitoSeguro::accountManager & accountManager;
        SqlitoSeguro::session& currentSession;

        Fl_Window* window;

        Fl_Input* nameInput;
        Fl_Input* usernameInput;
        Fl_Input* passwordInput;
        Fl_Input* urlInput;
        Fl_Multiline_Input* notesInput;

        Fl_Button* saveButton;
        Fl_Button* cancelButton;

        static void onSave(Fl_Widget*, void* data);
        static void onCancel(Fl_Widget*, void* data);
        void handleSave();
        void handleCancel();

    public:
        AccountFormWindow(SqlitoSeguro::accountManager& am, SqlitoSeguro::session& cus);
        void show();
    };
}

#endif
