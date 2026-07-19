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
    window = new Fl_Window(800, 250);
    window->copy_label(usu.c_str());

    Fl_Box* dets = new Fl_Box(150, 30, 100, 20, "Make a selection");
    dets->box(FL_UP_BOX); // Give it a visible box border
    dets->align(FL_ALIGN_CENTER);

    scroll_container = new Fl_Scroll(25, 30, 100, 200);
    scroll_container->box(FL_DOWN_BOX);
    scroll_container->type(Fl_Scroll::VERTICAL);

    

    window->end();

    //actualizar a Fl_Hold_Browser

    scroll_container->clear();
    scroll_container->begin();
    int start_x = scroll_container->x();
    int start_y = scroll_container->y() + 20;
    int label_w = scroll_container->w();
    int label_h = 25;
    int spacing = 5;
    
    int current_y = start_y;

    auto accounts = accountManager.listAccounts(uid);
    Fl_Box* box = new Fl_Box(20, 32, 110, 20, "Cuentas");
    box->box(FL_UP_BOX);

    Fl_Hold_Browser* label_box = new Fl_Hold_Browser(start_x, current_y, label_w, label_h+50);
    label_box->color(FL_GRAY);
    for (const auto& [key, values] : accounts)
    {
        std::string val = values[0];
        std::string cen_itm = "@c" + val;
        label_box->add(cen_itm.c_str());
        current_y += label_h + spacing;
    }
    scroll_container->end();
    scroll_container->redraw();
    label_box->callback(browser_callback, dets);
    label_box->when(FL_WHEN_CHANGED); 
    
    window->show();


}

void SqlitoSeguro::accountsWindow::browser_callback(Fl_Widget* widget, void* data) {
    Fl_Hold_Browser* browser = (Fl_Hold_Browser*)widget;
    Fl_Box* target_box = (Fl_Box*)data;
    std::string cuenta = browser->text(browser->value());
    cuenta.erase(0,2);
    browser->copy_label(cuenta.c_str());

    target_box->copy_label("test");
    target_box->redraw(); 
}
