
#include "app.h"

#include "session.h"

#include <iostream>
#include <utility>


App::App(Session& session, std::vector<IMenu*> menus) : session_(session), menus_(std::move(menus)) {}

void App::clear_screen()
{
    std::system("cls");
}

int App::run()
{
    while (true)
    {
        std::cout << "Welcome to the Warehouse Management System" << std::endl;
        if (!session_.is_logged_in())
        {
            MenuResult menu_result = menus_[0]->run();  // 0 AuthMenu
            if (menu_result == MenuResult::EXIT)
            {
                std::cout << "goodbye.." << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "you are logged in as " << session_.get_current_user().to_string() << std::endl;
            std::cout << "1. users" << std::endl;
            std::cout << "2. logout" << std::endl;
            std::cout << "0. exit" << std::endl;

            int choice;
            std::cin >> choice;
            switch (choice)
            {
                case 1:
                    menus_[1]->run();
                    break;
                case 2:
                    session_.logout();
                    break;
                case 0:
                    std::cout << "goodbye.." << std::endl;
                    return 0;
                default:
                    std::cout << "Invalid choice" << std::endl;
                    break;
            }
        }
    }
    return 0;
}
