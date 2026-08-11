#include "database.h"

#include <iostream>


Database::Database()
    : db_(nullptr) {}

Database::~Database()
{
    close();
}

bool Database::open(const std::string& db_path)
{
    if (db_ != nullptr)
    {
        close();
    }

    int rc = sqlite3_open(db_path.c_str(), &db_);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db_) << std::endl;
        sqlite3_close(db_);
        db_ = nullptr;
        return false;
    }

    return true;
}

void Database::close()
{
    if (db_ != nullptr)
    {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}




void Database::bind_parameters(sqlite3_stmt* stmt, const std::vector<DbValue>& params)
{
    for (size_t i = 0; i < params.size(); ++i)
    {
        int index = static_cast<int>(i + 1);

        if (std::holds_alternative<int>(params[i]))
        {
            sqlite3_bind_int(stmt, index, std::get<int>(params[i]));
        }
        else if (std::holds_alternative<double>(params[i]))
        {
            sqlite3_bind_double(stmt, index, std::get<double>(params[i]));
        }
        else if (std::holds_alternative<std::string>(params[i]))
        {
            const auto& str = std::get<std::string>(params[i]);
            sqlite3_bind_text(stmt, index, str.c_str(), -1, SQLITE_TRANSIENT);
        }
    }
}

bool Database::query(const std::string& sql,
                     const std::vector<DbValue>& params,
                   const std::function<void(sqlite3_stmt*)>& row_processor)
{
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    bind_parameters(stmt, params);

    try
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            row_processor(stmt);
        }
    }
    catch (...)
    {
        sqlite3_finalize(stmt);
        throw;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool Database::execute(const std::string& sql, const std::vector<DbValue>& params)
{
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    bind_parameters(stmt, params);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}
