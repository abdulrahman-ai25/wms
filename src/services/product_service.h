#pragma once

#include "product_repository.h"
#include "category_repository.h"
#include "service_status.h"
#include <vector>
#include <optional>
#include <string>

class ProductService
{
private:
    ProductRepository& product_repository_;
    CategoryRepository& category_repository_;

public:
    ProductService(ProductRepository& product_repository, CategoryRepository& category_repository);

    // --- Queries (Getters) ---
    std::vector<Product> get_all_products();
    std::optional<Product> get_product_by_id(int id);
    std::vector<Product> get_products_by_name(const std::string& name);
    std::vector<Product> get_products_by_category(int category_id);
    std::vector<Product> get_low_stock_products();
    std::vector<Product> get_out_of_stock_products();

    // --- Statistics (Counts) ---
    int get_total_products_count();
    int get_products_count_by_category(int category_id);
    int get_low_stock_count();
    int get_out_of_stock_count();

    // --- Core Operations (CRUD) ---
    ServiceStatus add_product(const std::string& name, int category_id, int quantity, int minimum_quantity, double default_selling_price, const std::string& description = "");

    ServiceStatus update_product(
        int id, 
        const std::string& name, 
        std::optional<int> category_id, 
        std::optional<int> minimum_quantity, 
        std::optional<double> default_selling_price, 
        const std::string& description
    );

    ServiceStatus delete_product(int id);

    // --- Price Update ---
    ServiceStatus update_price(int id, double new_price);
};
