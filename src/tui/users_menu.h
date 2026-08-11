#pragma once
#include "i_menu.h"
#include "menu_result.h"
#include "session.h"
#include "users_service.h"

class UsersMenu : public IMenu {
public:
    UsersMenu(UsersService& users_service, Session& session);
    MenuResult run() override;

private:
    MenuResult list_users();
    MenuResult show_user();
    MenuResult add_user();
    MenuResult delete_user();
    MenuResult update_role();
    MenuResult update_username();

    UsersService& users_service_;
    Session& session_;
};
    