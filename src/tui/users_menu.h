#pragma once
#include "i_menu.h"
#include "menu_result.h"
#include "session.h"
#include "users_service.h"

class UsersMenu : public IMenu {
public:
    UsersMenu(UsersService& users_service, Session& session);
    MenuResult run() override;
    std::string get_title() const override { return "Users"; }

private:
    void list_users();
    void show_user();
    void add_user();
    void delete_user();
    void update_role();
    void update_username();
    UsersService& users_service_;
    Session& session_;
};
    