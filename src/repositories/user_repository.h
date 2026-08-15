#pragma once

#include "database.h"
#include "user.h"

#include <optional>
#include <string>
#include <vector>

class UserRepository
{
private:
    Database& database_;

    static User map_row(sqlite3_stmt* stmt);

public:
    explicit UserRepository(Database& database);

    std::optional<User> find_by_id(int id);

    std::optional<User> find_by_username(const std::string& username);

    std::vector<User> get_all();

    bool create(const User& user);

    bool update(const User& user);

    bool remove(int id);

    bool exists_by_username(const std::string& username);
    bool is_empty();
};
