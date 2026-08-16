#pragma once
#include "i_menu.h"
#include "auth_service.h"

class AuthMenu:public IMenu{
public:
    AuthMenu( AuthService& auth_service);
    MenuResult run() override;
    std::string get_title() const override { return "Auth"; }
private:

    MenuResult login_menu() ;

    AuthService& auth_service_;
};
    