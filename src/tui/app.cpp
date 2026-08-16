
#include "app.h"
#include "menu_result.h"
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
            MenuResult result = menus_[0]->run();
            if (result == MenuResult::EXIT)
            {
                std::cout << "goodbye.." << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "you are logged in as " << session_.get_current_user().to_string() << std::endl;

            for (size_t i = 1; i < menus_.size(); ++i)
                std::cout << i << ". " << menus_[i]->get_title() << std::endl;
            std::cout << "L. Logout" << std::endl;
            std::cout << "0. Exit" << std::endl;

            std::string input;
            std::cin >> input;

            if (input == "0")
            {
                std::cout << "goodbye.." << std::endl;
                break;
            }
            else if (input == "L" || input == "l")
            {
                session_.logout();
            }
            else
            {
                try
                {
                    size_t choice = std::stoul(input);
                    if (choice >= 1 && choice < menus_.size())
                    {
                        MenuResult result = menus_[choice]->run();
                        if (result == MenuResult::BACK)
                        {
                            continue;
                        }
                        else if (result == MenuResult::EXIT)
                        {
                            std::cout << "goodbye.." << std::endl;
                            break;
                        }
                    }
                    else
                        std::cout << "Invalid choice" << std::endl;
                }
                catch (...)
                {
                    std::cout << "Invalid choice" << std::endl;
                }
            }
        }
    }
    return 0;
}

