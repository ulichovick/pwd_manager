#include "accounts_window_test.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Hold_Browser.H>
#include "accounts.h"

SqlitoSeguro::accountsWindowtest::accountsWindowtest(SqlitoSeguro::accountManager& am, int uid, std::string_view username):
    accountManager(am),
    uid(uid),
    username(username)
{
    std::string usu {"Cuentas de "};
    usu = usu + std::string(username);
    window = new Fl_Window(900, 600);
    window->copy_label(usu.c_str());

    browser = new Fl_Hold_Browser(
        10, 40,
        250, 500,
        "Cuentas");
    browser->align(FL_ALIGN_TOP);

    nameOutput =
        new Fl_Output(350,40,500,25,"Servicio:");

    usernameOutput =
        new Fl_Output(350,80,500,25,"Usuario:");

    passwordOutput =
        new Fl_Output(350,120,500,25,"Contraseña:");

    urlOutput =
        new Fl_Output(350,160,500,25,"URL:");

    addButton =
        new Fl_Button(10,550,90,30,"Agregar");

    editButton =
        new Fl_Button(110,550,90,30,"Editar");

    deleteButton =
        new Fl_Button(210,550,90,30,"Borrar");

    copyUserButton =
        new Fl_Button(430,550,110,30,"Copiar Usu");

    copyPassButton =
        new Fl_Button(550,550,110,30,"Copiar Contra");

    logoutButton =
        new Fl_Button(760,550,120,30,"Salir");

    auto accounts = accountManager.listAccounts(uid);

    window->end();

    for (const auto& [key, values] : accounts)
    {
        std::string val = values[0];
        browser->add(val.c_str(), (void*)(intptr_t)key);
    }

    browser->callback(browser_callback, this);
    addButton->callback(addAccountWind, this);

    browser->when(FL_WHEN_CHANGED); 

    window->show();
}

//arreglar el detallar cuenta, el return

void SqlitoSeguro::accountsWindowtest::browser_callback(Fl_Widget* widget, void* data) {
    Fl_Hold_Browser* browser = (Fl_Hold_Browser*)widget;
    std::string cuenta = browser->text(browser->value());


    int line = browser->value();
    int id;
    if (line > 0) 
    {
        id = (int)(intptr_t)browser->data(line);
    }

    auto* self = static_cast<accountsWindowtest*>(data);
    std::vector<std::string> currAccount = self->detailCurrAccount(self->uid, id);
    if (currAccount.size() < 4)
        return;
    self->nameOutput->value(currAccount[1].c_str());
    self->usernameOutput->value(currAccount[2].c_str());
    self->passwordOutput->value(currAccount[3].c_str());
    self->urlOutput->value(currAccount[4].c_str());
}

std::vector<std::string> SqlitoSeguro::accountsWindowtest::detailCurrAccount(int usid,int id)
{
    
    std::vector<std::string> currAccount {accountManager.detailAccount(usid, id)};

    return currAccount;
}

void SqlitoSeguro::accountsWindowtest::addAccountWind(Fl_Widget* widget, void* data)
{
    Fl_Window* win = (Fl_Window*)data;
    win->begin();
    Fl_Window* dlg = new Fl_Window(300, 150, "Add/Edit Account");
    Fl_Button* close_btn = new Fl_Button(60, 60, 80, 25, "Close");
    close_btn->callback(close_dialog_cb, dlg);

    dlg->end();
    win->end();

    dlg->set_modal();
    dlg->show();
    while (dlg->shown()) 
    {
        Fl::wait();
    }
    delete dlg;
}

void SqlitoSeguro::accountsWindowtest::close_dialog_cb(Fl_Widget* w, void* data) {
    Fl_Window* dialog = (Fl_Window*)data;
    dialog->hide(); 
}
