#include "users_menu.h"
#include <iostream>
UsersMenu::UsersMenu(UsersService& users_service, Session& session)
    : users_service_(users_service), session_(session) {}

MenuResult UsersMenu::run()
{
    while (true)
    {
        std::cout << "Users Menu" << std::endl;
        std::cout << "1. List all users" << std::endl;
        std::cout << "2. Show user" << std::endl;
        std::cout << "3. Add user" << std::endl;
        std::cout << "4. Delete user" << std::endl;
        std::cout << "5. Update role" << std::endl;
        std::cout << "6. Update username" << std::endl;
        std::cout << "7. Back" << std::endl;
        std::cout << "0. Exit" << std::endl;
        int choice;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            list_users();
            break;
        case 2:
            show_user();
            break;
        case 3:
            add_user();
            break;
        case 4:
            delete_user();
            break;
        case 5:
            update_role();
            break;
        case 6:
            update_username();
            break;
        case 7:
            return MenuResult::BACK;
        case 0:
            return MenuResult::EXIT;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }
}

MenuResult UsersMenu::list_users()
{
    Role role = session_.get_current_user().get_role();
    std::vector<User> users = users_service_.get_all_users(role);
    if (users.empty())
    {
        std::cout << "No users found or permission denied." << std::endl;
        return MenuResult::BACK;
    }
    for (const auto& user : users)
        std::cout << user.to_string() << std::endl;
    return MenuResult::SUBMIT;
}

MenuResult UsersMenu::show_user()
{
    int id;
    std::cout << "Enter user id: ";
    std::cin >> id;
    Role role = session_.get_current_user().get_role();
    std::optional<User> user = users_service_.get_user_by_id(role, id);
    if (user.has_value())
    {
        std::cout << user.value().to_string() << std::endl;
    }
    else
    {
        std::cout << "User not found" << std::endl;
    }
    return MenuResult::SUBMIT;
}

MenuResult UsersMenu::add_user()
{
    std::string username, password;
    int role_choice;
    Role user_role;

    std::cout << "Enter new username: ";
    std::cin >> username;
    std::cout << "Enter new password: ";
    std::cin >> password;
    std::cout << "Enter role (0: Admin, 1: Employee): ";
    std::cin >> role_choice;

    if (role_choice == 0)
        user_role = Role::ADMIN;
    else
        user_role = Role::EMPLOYEE;

    Role caller_role = session_.get_current_user().get_role();
    ServiceStatus status = users_service_.add_user(caller_role, username, password, user_role);

    if (status == ServiceStatus::SUCCESS)
        std::cout << "User added successfully" << std::endl;
    else if (status == ServiceStatus::USER_ALREADY_EXISTS)
        std::cout << "Username already exists" << std::endl;
    else if (status == ServiceStatus::PERMISSION_DENIED)
        std::cout << "Permission denied" << std::endl;
    else
        std::cout << "Failed to add user" << std::endl;

    return MenuResult::SUBMIT;
}

MenuResult UsersMenu::delete_user()
{
    int id;
    std::cout << "Enter user id: ";
    std::cin >> id;
    Role role = session_.get_current_user().get_role();
    ServiceStatus status = users_service_.delete_user(role, id);
    if (status == ServiceStatus::SUCCESS)
        std::cout << "User deleted successfully" << std::endl;
    else if (status == ServiceStatus::PERMISSION_DENIED)
        std::cout << "Permission denied" << std::endl;
    else
        std::cout << "User not found" << std::endl;
    return MenuResult::SUBMIT;
}

MenuResult UsersMenu::update_role()
{
    int id;
    Role new_role;
    std::cout << "Enter user id: ";
    std::cin >> id;
    std::cout << "Enter new role: ";
    std::cout << "0. Admin " << std::endl;
    std::cout << "1. Employee" << std::endl;
    int choice;
    std::cin >> choice;
    switch (choice)
    {
    case 0:
        new_role = Role::ADMIN;
        break;
    case 1:
        new_role = Role::EMPLOYEE;
        break;
    default:
        std::cout << "Invalid choice" << std::endl;
        return MenuResult::BACK;
    }

    Role role = session_.get_current_user().get_role();
    ServiceStatus status = users_service_.update_role(role, id, new_role);
    if (status == ServiceStatus::SUCCESS)
        std::cout << "Role updated successfully" << std::endl;
    else if (status == ServiceStatus::PERMISSION_DENIED)
        std::cout << "Permission denied" << std::endl;
    else
        std::cout << "User not found" << std::endl;
    return MenuResult::SUBMIT;
}

MenuResult UsersMenu::update_username()
{
    int id;
    std::string new_username;
    std::cout << "Enter user id: ";
    std::cin >> id;
    std::cout << "Enter new username: ";
    std::cin >> new_username;
    Role role = session_.get_current_user().get_role();
    ServiceStatus status = users_service_.update_username(role, id, new_username);
    if (status == ServiceStatus::SUCCESS)
        std::cout << "Username updated successfully" << std::endl;
    else if (status == ServiceStatus::PERMISSION_DENIED)
        std::cout << "Permission denied" << std::endl;
    else
        std::cout << "User not found or username taken" << std::endl;
    return MenuResult::SUBMIT;
}
