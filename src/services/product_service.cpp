#include "product_service.h"
#include "product.h"
#include <optional>

ProductService::ProductService(ProductRepository& product_repository, CategoryRepository& category_repository)
    : product_repository_(product_repository), category_repository_(category_repository) {}



// --- Queries (Getters) ---
std::vector<Product> ProductService::get_all_products() {
    return product_repository_.get_all();
}

std::optional<Product> ProductService::get_product_by_id(int id) {
    return product_repository_.find_by_id(id);
}

std::vector<Product> ProductService::get_products_by_name(const std::string& name) {
    return product_repository_.find_by_name(name);
}

std::vector<Product> ProductService::get_products_by_category(int category_id) {
    return product_repository_.get_products_by_category(category_id);
}

std::vector<Product> ProductService::get_low_stock_products() {
    return product_repository_.get_low_stock_products();
}

std::vector<Product> ProductService::get_out_of_stock_products() {
    return product_repository_.get_out_of_stock_products();
}


// --- Statistics (Counts) ---
int ProductService::get_total_products_count() {
    return product_repository_.count();
}

int ProductService::get_products_count_by_category(int category_id) {
    return product_repository_.count_by_category(category_id);
}

int ProductService::get_low_stock_count() {
    return product_repository_.count_low_stock_products();
}

int ProductService::get_out_of_stock_count() {
    return product_repository_.count_out_of_stock_products();
}

// --- Core Operations (CRUD) ---
ServiceStatus ProductService::add_product(const std::string& name, int category_id, int quantity, int minimum_quantity, double default_selling_price, const std::string& description) {
    if (name.empty()) {
        return ServiceStatus::INVALID_INPUT;
    }
    if (quantity < 0 || minimum_quantity < 0 || default_selling_price < 0) {
        return ServiceStatus::INVALID_INPUT;
    }

    if (!category_repository_.find_by_id(category_id).has_value()) {
        return ServiceStatus::NOT_FOUND;
    }

    Product new_product(name, category_id, quantity, minimum_quantity, default_selling_price, description);
    if (product_repository_.create(new_product)) {
        return ServiceStatus::SUCCESS;
    }

    return ServiceStatus::OPERATION_FAILED;
}

ServiceStatus ProductService::update_product(
    int id, 
    const std::string& name, 
    std::optional<int> category_id, 
    std::optional<int> minimum_quantity, 
    std::optional<double> default_selling_price, 
    const std::string& description) 
{
    std::optional<Product> existing_product = product_repository_.find_by_id(id);
    if (!existing_product.has_value()) {
        return ServiceStatus::NOT_FOUND;
    }

    if (!name.empty()) {
        existing_product->set_name(name);
    }

    if (category_id.has_value()) {
        if (!category_repository_.find_by_id(category_id.value()).has_value()) {
            return ServiceStatus::NOT_FOUND;
        }
        existing_product->set_category_id(category_id.value());
    }

    if (minimum_quantity.has_value()) {
        if (minimum_quantity.value() < 0) {
            return ServiceStatus::INVALID_INPUT;
        }
        existing_product->set_minimum_quantity(minimum_quantity.value());
    }

    if (default_selling_price.has_value()) {
        if (default_selling_price.value() < 0) {
            return ServiceStatus::INVALID_INPUT;
        }
        existing_product->set_default_selling_price(default_selling_price.value());
    }

    if (!description.empty()) {
        existing_product->set_description(description);
    }

    if (product_repository_.update(existing_product.value())) {
        return ServiceStatus::SUCCESS;
    }

    return ServiceStatus::OPERATION_FAILED;
}

ServiceStatus ProductService::delete_product(int id) {
    if (!product_repository_.find_by_id(id).has_value()) {
        return ServiceStatus::NOT_FOUND;
    }
    
    if (product_repository_.remove(id)) {
        return ServiceStatus::SUCCESS;
    }
    
    return ServiceStatus::OPERATION_FAILED;
}

// --- Price Update ---
ServiceStatus ProductService::update_price(int id, double new_price) {
    if (new_price < 0) {
        return ServiceStatus::INVALID_INPUT;
    }

    if (!product_repository_.find_by_id(id).has_value()) {
        return ServiceStatus::NOT_FOUND;
    }

    if (product_repository_.update_default_selling_price(id, new_price)) {
        return ServiceStatus::SUCCESS;
    }

    return ServiceStatus::OPERATION_FAILED;
}

