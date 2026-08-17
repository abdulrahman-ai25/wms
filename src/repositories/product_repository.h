#pragma once
#include "product.h"
#include "database.h"

#include <optional>
#include <vector>


class ProductRepository
{
private:
    Database& database_;

    static Product map_row(sqlite3_stmt* stmt);

public:
    ProductRepository(Database& db);

    std::optional<Product> find_by_id(int id);
    std::optional<Product> find_by_name(const std::string& name);
    std::vector<Product> get_all();
    std::vector<Product> get_low_stock_products();
    std::vector<Product> get_out_of_stock_products();
    std::vector<Product> get_products_by_category(int category_id);
    bool create(const Product& product);
    bool update(const Product& product);
    bool remove(int id);
    bool exists_by_name(const std::string& name);

    int count();
    int count_by_category(int category_id);
    int count_low_stock_products();
    int count_out_of_stock_products();
    bool update_quantity(int id, int quantity);
    bool update_default_selling_price(int id, double price);
};
