#include "accounts_window.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include "accounts.h"

SqlitoSeguro::accountsWindow::accountsWindow(SqlitoSeguro::accountManager& am, int uid): accountManager(am), uid(uid)
{
    window = new Fl_Window(300, 200, "Accounts");

    statusLabel = new Fl_Box(50, 30, 200, 25, "Cuentas");

}
void SqlitoSeguro::accountsWindow::show()
{
    window->end();
    
    auto accounts = accountManager.listAccounts(uid);
    
    for (const auto& [key, values] : accounts)
    {
        std::cout << key << "\t" << values[0] << "\n";
        std::string val = values[0];
        statusLabel->label(val.c_str());
        statusLabel->redraw();
    }
    window->show();
    
}
