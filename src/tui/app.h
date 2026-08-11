#pragma once

#include "i_menu.h"
#include "session.h"
#include <vector>

class App{
public:
    App(Session& session,std::vector<IMenu*> menus);
    int run();
    void clear_screen();

private:
    Session& session_;
    std::vector<IMenu*> menus_;

};