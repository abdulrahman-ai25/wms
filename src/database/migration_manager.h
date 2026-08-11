#pragma once
#include "database.h"
#include <vector>
#include <string>


class MigrationManager {
public:
    MigrationManager(Database& db);
    void run_migrations();
private:
    Database& db_;
    static const std::vector<std::string> migrations_;
};
