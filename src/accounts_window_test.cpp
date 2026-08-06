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
        browser->add(val.c_str());
    }

    window->show();
}
