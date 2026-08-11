#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <variant>
#include <functional>

using DbValue = std::variant<int, double, std::string>;

class Database {
private:
    sqlite3* db_ = nullptr;

    void bind_parameters(sqlite3_stmt* stmt, const std::vector<DbValue>& params);

public:
    Database();
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool open(const std::string& db_path);
    void close();
    bool is_open() const { return db_ != nullptr; }
    sqlite3* connection() const { return db_; }

    bool query(const std::string& sql,
               const std::vector<DbValue>& params,
               const std::function<void(sqlite3_stmt*)>& row_processor);

    bool execute(const std::string& sql, const std::vector<DbValue>& params= {});
};