#include "accounts_window.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Hold_Browser.H>
#include "accounts.h"

SqlitoSeguro::accountsWindow::accountsWindow(SqlitoSeguro::accountManager& am, int uid, std::string_view username): 
accountManager(am), 
uid(uid),
username(username)
{
    std::string usu {"Cuentas de "};
    usu = usu + std::string(username);
    window = new Fl_Window(600, 900);
    window->copy_label(usu.c_str());

    scroll_container = new Fl_Scroll(25, 30, 250, 150);
    scroll_container->box(FL_DOWN_BOX);
    scroll_container->type(Fl_Scroll::VERTICAL);

}

void SqlitoSeguro::accountsWindow::show()
{
    window->end();

    //actualizar a Fl_Hold_Browser
    
    scroll_container->clear();
    scroll_container->begin();
    int start_x = scroll_container->x() + 10;
    int start_y = scroll_container->y() + 10;
    int label_w = scroll_container->w() - 40;
    int label_h = 25;
    int spacing = 5;
    
    int current_y = start_y;

    auto accounts = accountManager.listAccounts(uid);
    Fl_Box* box = new Fl_Box(20, 20, 360, 20, "Cuentas");
    box->box(FL_UP_BOX);
    Fl_Hold_Browser* label_box = new Fl_Hold_Browser(start_x, current_y, label_w, label_h+50);
    for (const auto& [key, values] : accounts)
    {
        std::cout << key << "\t" << values[0] << "\n";
        std::string val = values[0];
        
        label_box->add(val.c_str());
        current_y += label_h + spacing;
    }
    label_box->callback(browser_callback, this);
    scroll_container->end();
    scroll_container->redraw();
    window->show();
    
}

void SqlitoSeguro::accountsWindow::browser_callback(Fl_Widget* widget, void* data) {
    Fl_Hold_Browser* browser = (Fl_Hold_Browser*)widget;
    std::cout << "Cuenta seleccionada: " << browser->text(browser->value());
}
