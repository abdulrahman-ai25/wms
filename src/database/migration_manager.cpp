#include "migration_manager.h"
#include "database.h"
#include <vector>
#include <string>


const std::vector<std::string> MigrationManager::migrations_ = {
    // Migration 1: users
    R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            role TEXT NOT NULL DEFAULT 'employee'
                CHECK(role IN ('admin','employee')),
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        );
    )",

    // Migration 2: categories
    R"(
        CREATE TABLE IF NOT EXISTS categories (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            description TEXT
        );
    )",

    // Migration 3: products
    R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            category_id INTEGER NOT NULL,
            quantity INTEGER NOT NULL DEFAULT 0 CHECK(quantity >= 0),
            minimum_quantity INTEGER NOT NULL DEFAULT 10 CHECK(minimum_quantity >= 0),
            default_selling_price REAL NOT NULL DEFAULT 0 CHECK(default_selling_price >= 0),
            description TEXT,
            FOREIGN KEY (category_id) REFERENCES categories(id)
        );
    )",
    

};

MigrationManager::MigrationManager(Database& db) : db_(db) {}

void MigrationManager::run_migrations()
{
    for (const auto& migration : migrations_)
    {
        db_.execute(migration);
    }
}