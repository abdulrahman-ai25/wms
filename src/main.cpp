#include "app.h"
#include "auth_menu.h"
#include "auth_service.h"
#include "core/users_service.h"
#include "database.h"
#include "i_menu.h"
#include "migration_manager.h"
#include "session.h"
#include "tui/users_menu.h"
#include "user_repository.h"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    Database db;
    if (!db.open("wms.db"))
    {
        std::cerr << "Failed to open database!" << std::endl;
        return 1;
    }
    std::cout << "Database opened successfully." << std::endl;

    MigrationManager manager(db);
    manager.run_migrations();

    // std::cout << "enter sql statements (terminate each query with ';' or CTRL+C to exit):" << std::endl;
    // std::string sql;
    // char c;
    // while (std::cin.get(c)) {
    //     if (c == ';') {
    //         if (!sql.empty()) {
    //             db.execute(sql);
    //             sql.clear();
    //         }
    //     } else {
    //         sql += c;
    //     }
    // }

#pragma region repos

    UserRepository usersrepo(db);

#pragma endregion

#pragma region services

    Session session;
    AuthService auth_service(usersrepo, session);
    UsersService users_service(usersrepo,auth_service);
    if(usersrepo.is_empty())
        auth_service.register_user("admin", "123", Role::ADMIN);
    // // auth.register_user("admin", "123", Role::ADMIN);
    // auth.login("admin", "1234");

    // std::cout << session.get_current_user().to_string();

#pragma endregion

#pragma region tui

    AuthMenu auth_menu(auth_service);
    UsersMenu users_menu(users_service, session);
    std::vector<IMenu*> menus = {&auth_menu, &users_menu};

    App app(session, menus);

#pragma endregion

    app.run();
    db.close();
    return 0;
}
