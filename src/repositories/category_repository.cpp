#include "category_repository.h"

CategoryRepository::CategoryRepository(Database& db) : database_(db) {}

Category CategoryRepository::map_row(sqlite3_stmt* stmt)
{
    int id = sqlite3_column_int(stmt, 0);

    std::string name =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

    if(sqlite3_column_type(stmt,2)==SQLITE_NULL)
    {
        return Category(id, name);
    }
    std::string description =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    return Category(id, name,description);
}



bool CategoryRepository::create(const Category& category)
{
    return database_.execute(
        "INSERT INTO categories(name, description) VALUES (?, ?);",
        {category.get_name(), category.get_description()}
    );
}

std::optional<Category> CategoryRepository::find_by_id(int id)
{
    std::optional<Category> result;

    database_.query(
        "SELECT id, name, description "
        "FROM categories WHERE id = ?;",
        {id},
        [&result](sqlite3_stmt* stmt) {
            result = CategoryRepository::map_row(stmt);
        }
    );

    return result;
}

std::optional<Category> CategoryRepository::find_by_name(const std::string& name)
{
    std::optional<Category> result;

    database_.query(
        "SELECT id, name, description "
        "FROM categories WHERE name = ?;",
        {name},
        [&result](sqlite3_stmt* stmt) {
            result = CategoryRepository::map_row(stmt);
        }
    );

    return result;
}

std::vector<Category> CategoryRepository::get_all()
{
    std::vector<Category> categories;

    database_.query(
        "SELECT id, name, description FROM categories;",
        {},
        [&categories](sqlite3_stmt* stmt) {
            categories.push_back(CategoryRepository::map_row(stmt));
        }
    );

    return categories;
}

bool CategoryRepository::update(const Category& category)
{
    if (!category.get_id().has_value())
    {
        return false;
    }

    return database_.execute(
        "UPDATE categories SET name = ?, description = ? WHERE id = ?;",
        {category.get_name(), category.get_description(), category.get_id().value()}
    );
}

bool CategoryRepository::remove(int id)
{
    return database_.execute(
        "DELETE FROM categories WHERE id = ?;",
        {id}
    );
}

bool CategoryRepository::exists_by_name(const std::string& name)
{
    bool found = false;

    database_.query(
        "SELECT 1 FROM categories WHERE name = ? LIMIT 1;",
        {name},
        [&found](sqlite3_stmt* /*stmt*/) {
            found = true;
        }
    );

    return found;
}
