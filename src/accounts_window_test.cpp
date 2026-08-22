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
        "Accounts");
    browser->align(FL_ALIGN_TOP);

    nameOutput =
        new Fl_Output(350,40,500,25,"Name:");

    usernameOutput =
        new Fl_Output(350,80,500,25,"Username:");

    passwordOutput =
        new Fl_Output(350,120,500,25,"Password:");

    urlOutput =
        new Fl_Output(350,160,500,25,"URL:");

    static outputs outs;
    outs.nameOutput = nameOutput;
    outs.usernameOutput = usernameOutput;
    outs.passwordOutput = passwordOutput;
    outs.urlOutput = urlOutput;
    outs.uid = uid;

    addButton =
        new Fl_Button(10,550,90,30,"Add");

    editButton =
        new Fl_Button(110,550,90,30,"Edit");

    deleteButton =
        new Fl_Button(210,550,90,30,"Delete");

    copyUserButton =
        new Fl_Button(430,550,110,30,"Copy User");

    copyPassButton =
        new Fl_Button(550,550,110,30,"Copy Pass");

    logoutButton =
        new Fl_Button(760,550,120,30,"Logout");

    auto accounts = accountManager.listAccounts(uid);

    window->end();

    for (const auto& [key, values] : accounts)
    {
        std::string val = values[0];
        browser->add(val.c_str(), (void*)(intptr_t)key);
    }
    browser->callback(browser_callback, &outs);
    browser->when(FL_WHEN_CHANGED); 

    window->show();
}

//arreglar el detallar cuenta, el return

void SqlitoSeguro::accountsWindowtest::browser_callback(Fl_Widget* widget, void* data) {
    Fl_Hold_Browser* browser = (Fl_Hold_Browser*)widget;
    std::string cuenta = browser->text(browser->value());
    outputs *outs = (outputs *)data;
    SqlitoSeguro::accountManager *accDetails = (SqlitoSeguro::accountManager *)data;

    int line = browser->value();
    int id;
    if (line > 0) 
    {
        id = (int)(intptr_t)browser->data(line);
    }
    
    std::vector<std::string> currAccount {accDetails->detailAccount(outs->uid, id)};
    outs->nameOutput->value(currAccount[0].c_str());
    outs->usernameOutput->value(currAccount[1].c_str());
    outs->passwordOutput->value(currAccount[2].c_str());
    outs->urlOutput->value(currAccount[3].c_str());
}
