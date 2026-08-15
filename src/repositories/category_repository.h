#pragma once
#include "category.h"
#include "database.h"

#include <optional>
#include <vector>


class CategoryRepository
{
private:
    Database& database_;

    static Category map_row(sqlite3_stmt* stmt);

public:
    CategoryRepository(Database& db);

    std::optional<Category> find_by_id(int id);
    std::optional<Category> find_by_name(const std::string& name);
    std::vector<Category> get_all();
    bool create(const Category& category);
    bool update(const Category& category);
    bool remove(int id);
    bool exists_by_name(const std::string& name);
};
