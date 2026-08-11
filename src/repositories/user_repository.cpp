#include "user_repository.h"

UserRepository::UserRepository(Database& database)
    : database_(database) {}

User UserRepository::map_row(sqlite3_stmt* stmt)
{
    int id = sqlite3_column_int(stmt, 0);

    std::string username =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

    std::string password_hash =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

    Role role = role_from_string(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

    std::string created_at =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

    return User(id, username, password_hash, role, created_at);
}

bool UserRepository::create(const User& user)
{
    return database_.execute(
        "INSERT INTO users (username, password_hash, role) VALUES (?, ?, ?);",
        {user.get_username(), user.get_password_hash(), role_to_string(user.get_role())}
    );
}

std::optional<User> UserRepository::find_by_id(int id)
{
    std::optional<User> result;

    database_.query(
        "SELECT id, username, password_hash, role, created_at "
        "FROM users WHERE id = ?;",
        {id},
        [&result](sqlite3_stmt* stmt) {
            result = UserRepository::map_row(stmt);
        }
    );

    return result;
}

std::optional<User> UserRepository::find_by_username(const std::string& username)
{
    std::optional<User> result;

    database_.query(
        "SELECT id, username, password_hash, role, created_at "
        "FROM users WHERE username = ?;",
        {username},
        [&result](sqlite3_stmt* stmt) {
            result = UserRepository::map_row(stmt);
        }
    );

    return result;
}

std::vector<User> UserRepository::get_all()
{
    std::vector<User> users;

    database_.query(
        "SELECT id, username, password_hash, role, created_at FROM users;",
        {},
        [&users](sqlite3_stmt* stmt) {
            users.push_back(UserRepository::map_row(stmt));
        }
    );

    return users;
}

bool UserRepository::update(const User& user)
{
    if (!user.get_id().has_value())
    {
        return false;
    }

    return database_.execute(
        "UPDATE users SET username = ?, password_hash = ?, role = ? WHERE id = ?;",
        {user.get_username(), user.get_password_hash(),
         role_to_string(user.get_role()), user.get_id().value()}
    );
}

bool UserRepository::remove(int id)
{
    return database_.execute(
        "DELETE FROM users WHERE id = ?;",
        {id}
    );
}

bool UserRepository::exists_by_username(const std::string& username)
{
    bool found = false;

    database_.query(
        "SELECT 1 FROM users WHERE username = ? LIMIT 1;",
        {username},
        [&found](sqlite3_stmt* /*stmt*/) {
            found = true;
        }
    );

    return found;
}

bool UserRepository::is_empty()
{
    int count=0;
    database_.query("select count(*) from users;",{},[&count](sqlite3_stmt* stmt){
        count=sqlite3_column_int(stmt,0);
    });
    return count==0;
}
