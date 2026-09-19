#include "account_form_window.h"
#include "accounts.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <utility>

SqlitoSeguro::AccountFormWindow::AccountFormWindow(SqlitoSeguro::accountManager& am, SqlitoSeguro::session& cus, std::function<void()> onSaved, std::optional<int> accountId):
    accountManager(am),
    currentSession(cus),
    accountId(accountId),
    onAccountSaved(std::move(onSaved))
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

    /*notesInput =
        new Fl_Multiline_Input(
            140,
            210,
            320,
            100,
            "Notes:"
        );*/

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
        if (accountId.has_value())
        {
            int accId = accountId.value_or(0);
            std::cout << "acc id: " << accId << "\n";
            std::vector<std::string> currAccount = accountManager.detailAccount(currentSession.userId.value(), accId);

            nameInput->value(currAccount[1].c_str());
            usernameInput->value(currAccount[2].c_str());
            passwordInput->value(currAccount[3].c_str());
            urlInput->value(currAccount[4].c_str());

        }
        else
        {
            std::cout << "no se ha seleccionado cuenta \n";
        }
        
        

    saveButton->callback(onSave, this);
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

void SqlitoSeguro::AccountFormWindow::onSave(
    Fl_Widget*,
    void* data)
{
    auto* self = static_cast<AccountFormWindow*>(data);
    int userId = *self->currentSession.userId;
    std::string name = self->nameInput->value();
    std::string username = self->usernameInput->value();
    std::string password = self->passwordInput->value();
    std::string url = self->urlInput->value();
    //std::string notes = self->notesInput->value();

    if (self->accountId.has_value())
    {
        self->accountManager.editAccount(name, username, password, userId, self->accountId.value());  
    }
    else
    {
        self->accountManager.addAccount(name, username, password, userId);  
    }
    
    if (self->onAccountSaved)
    {
    self->onAccountSaved();
    }


    self->window->hide();
    
}
