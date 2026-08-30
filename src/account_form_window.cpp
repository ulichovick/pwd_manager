#include "account_form_window.h"
#include "accounts.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>

SqlitoSeguro::AccountFormWindow::AccountFormWindow(SqlitoSeguro::accountManager& am, int uid):
    accountManager(am),
    userId(uid)
{
    window = new Fl_Window(
        500,
        400,
        "Add Account"
    );

    nameInput =
        new Fl_Input(140, 30, 320, 30, "Name:");

    usernameInput =
        new Fl_Input(140, 75, 320, 30, "Username:");

    passwordInput =
        new Fl_Input(140, 120, 320, 30, "Password:");

    urlInput =
        new Fl_Input(140, 165, 320, 30, "URL:");

    notesInput =
        new Fl_Multiline_Input(
            140,
            210,
            320,
            100,
            "Notes:"
        );

    saveButton =
        new Fl_Button(
            270,
            340,
            90,
            30,
            "Save"
        );

    cancelButton =
        new Fl_Button(
            370,
            340,
            90,
            30,
            "Cancel"
        );

    //saveButton->callback(onSave, this);
    cancelButton->callback(onCancel, this);

    window->end();
}

void SqlitoSeguro::AccountFormWindow::show()
{
    window->set_modal();
    window->show();
}

void SqlitoSeguro::AccountFormWindow::onCancel(
    Fl_Widget*,
    void* data)
{
    auto* self =
        static_cast<AccountFormWindow*>(data);

    self->window->hide();
}
