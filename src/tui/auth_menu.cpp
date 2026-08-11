#include "auth_menu.h"
#include <iostream>


AuthMenu::AuthMenu(AuthService& auth_service) : auth_service_(auth_service) {}

MenuResult AuthMenu::run(){
    int choice;
    while (true)
    {
        std::cout << "1. Login" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            if (login_menu() == MenuResult::SUBMIT)
                {return MenuResult::SUBMIT;}
            break;
        case 0:
            return MenuResult::EXIT;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
}

MenuResult AuthMenu::login_menu(){
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    
    ServiceStatus status = auth_service_.login(username, password);
    switch (status)
    {
    case ServiceStatus::SUCCESS:
        return MenuResult::SUBMIT;
    case ServiceStatus::USER_NOT_FOUND:
        std::cout << "User not found" << std::endl;
        return MenuResult::BACK;
    case ServiceStatus::INVALID_PASSWORD:
        std::cout << "Invalid password" << std::endl;
        return MenuResult::BACK;
    case ServiceStatus::USER_ALREADY_LOGGED_IN:
        std::cout << "User already logged in" << std::endl;
        return MenuResult::BACK;
    default:
        std::cout << "Invalid choice" << std::endl;
        return MenuResult::BACK;
    }
}
