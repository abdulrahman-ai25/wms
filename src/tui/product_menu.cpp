#include "product_menu.h"

#include <iostream>
#include <limits>


ProductMenu::ProductMenu(ProductService& product_service) : product_service_(product_service) {}

MenuResult ProductMenu::run()
{
    std::cout << "\n===== Products Menu =====" << std::endl;
    std::cout << "1. List products" << std::endl;
    std::cout << "2. Search products" << std::endl;
    std::cout << "3. Add product" << std::endl;
    std::cout << "4. Update product" << std::endl;
    std::cout << "5. Delete product" << std::endl;
    std::cout << "6. Update price" << std::endl;
    std::cout << "7. Low-stock products" << std::endl;
    std::cout << "8. Out-of-stock products" << std::endl;
    std::cout << "9. Product statistics" << std::endl;
    std::cout << "0. Back" << std::endl;
    while (true)
    {
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        switch (choice)
        {
            case 1:
                list_products();
                break;
            case 2:
                search_products();
                break;
            case 3:
                add_product();
                break;
            case 4:
                update_product();
                break;
            case 5:
                delete_product();
                break;
            case 6:
                update_price();
                break;
            case 7:
                show_low_stock_products();
                break;
            case 8:
                show_out_of_stock_products();
                break;
            case 9:
                show_statistics();
                break;
            case 0:
                return MenuResult::BACK;
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
        }
    }
}

void ProductMenu::list_products() {
    std::vector<Product> products = product_service_.get_all_products();
    if (products.empty()) {
        std::cout << "No products found." << std::endl;
        return;
    }
    for (const auto& product : products) {
        std::cout << product.to_string() << std::endl;
    }
}

void ProductMenu::search_products() {
    std::string name;
    std::cout << "Enter product name to search: ";
    read_str(name);

    std::vector<Product> products = product_service_.get_products_by_name(name);
    if (products.empty()) {
        std::cout << "No products found matching \"" << name << "\"." << std::endl;
        return;
    }
    std::cout << products.size() << " product(s) found:" << std::endl;
    for (const auto& product : products) {
        std::cout << product.to_string() << std::endl;
    }
}

void ProductMenu::add_product() {
    std::string name;
    std::string description;
    double default_selling_price;
    int quantity;
    int minimum_quantity;
    int category_id;

    std::cout << "Enter product name: ";
    read_str(name);
    std::cout << "Enter category ID: ";
    std::cin >> category_id;
    std::cout << "Enter initial quantity: ";
    std::cin >> quantity;
    std::cout << "Enter minimum quantity: ";
    std::cin >> minimum_quantity;
    std::cout << "Enter default selling price: ";
    std::cin >> default_selling_price;
    std::cout << "Enter description (or press Enter to skip): ";
    read_str(description, true);

    ServiceStatus status = product_service_.add_product(name, category_id, quantity, minimum_quantity, default_selling_price, description);
    switch (status) {
        case ServiceStatus::SUCCESS:
            std::cout << "Product added successfully." << std::endl;
            break;
        case ServiceStatus::INVALID_INPUT:
            std::cout << "Invalid input. Check that values are not negative." << std::endl;
            break;
        case ServiceStatus::NOT_FOUND:
            std::cout << "Category not found." << std::endl;
            break;
        default:
            std::cout << "Failed to add product." << std::endl;
            break;
    }
}

void ProductMenu::update_product() {
    int id;
    std::cout << "Enter product ID: ";
    std::cin >> id;

    std::optional<Product> product = product_service_.get_product_by_id(id);
    if (!product.has_value()) {
        std::cout << "Product not found." << std::endl;
        return;
    }

    std::cout << "Current: " << product->to_string() << std::endl;
    std::cout << "(Press Enter to keep current value)" << std::endl;

    std::string name;
    std::string description;
    std::string input;

    std::cout << "New name [" << product->get_name() << "]: ";
    read_str(name, true);

    std::optional<int> category_id;
    std::cout << "New category ID [" << product->get_category_id() << "]: ";
    read_str(input, true);
    if (!input.empty()) {
        try { category_id = std::stoi(input); }
        catch (...) { std::cout << "Invalid number, keeping current value." << std::endl; }
    }

    std::optional<int> minimum_quantity;
    std::cout << "New minimum quantity [" << product->get_minimum_quantity() << "]: ";
    read_str(input, true);
    if (!input.empty()) {
        try { minimum_quantity = std::stoi(input); }
        catch (...) { std::cout << "Invalid number, keeping current value." << std::endl; }
    }

    std::optional<double> default_selling_price;
    std::cout << "New default selling price [" << product->get_default_selling_price() << "]: ";
    read_str(input, true);
    if (!input.empty()) {
        try { default_selling_price = std::stod(input); }
        catch (...) { std::cout << "Invalid number, keeping current value." << std::endl; }
    }

    std::cout << "New description [" << product->get_description() << "]: ";
    read_str(description, true);

    ServiceStatus status = product_service_.update_product(id, name, category_id, minimum_quantity, default_selling_price, description);
    switch (status) {
        case ServiceStatus::SUCCESS:
            std::cout << "Product updated successfully." << std::endl;
            break;
        case ServiceStatus::NOT_FOUND:
            std::cout << "Product or category not found." << std::endl;
            break;
        case ServiceStatus::INVALID_INPUT:
            std::cout << "Invalid input." << std::endl;
            break;
        default:
            std::cout << "Failed to update product." << std::endl;
            break;
    }
}

void ProductMenu::delete_product() {
    int id;
    std::cout << "Enter product ID: ";
    std::cin >> id;

    ServiceStatus status = product_service_.delete_product(id);
    switch (status) {
        case ServiceStatus::SUCCESS:
            std::cout << "Product deleted successfully." << std::endl;
            break;
        case ServiceStatus::NOT_FOUND:
            std::cout << "Product not found." << std::endl;
            break;
        default:
            std::cout << "Failed to delete product." << std::endl;
            break;
    }
}

void ProductMenu::update_price() {
    int id;
    double new_price;
    std::cout << "Enter product ID: ";
    std::cin >> id;
    std::cout << "Enter new price: ";
    std::cin >> new_price;

    ServiceStatus status = product_service_.update_price(id, new_price);
    switch (status) {
        case ServiceStatus::SUCCESS:
            std::cout << "Price updated successfully." << std::endl;
            break;
        case ServiceStatus::NOT_FOUND:
            std::cout << "Product not found." << std::endl;
            break;
        case ServiceStatus::INVALID_INPUT:
            std::cout << "Invalid price. Price cannot be negative." << std::endl;
            break;
        default:
            std::cout << "Failed to update price." << std::endl;
            break;
    }
}

void ProductMenu::show_low_stock_products() {
    std::vector<Product> products = product_service_.get_low_stock_products();
    if (products.empty()) {
        std::cout << "No low-stock products." << std::endl;
        return;
    }
    std::cout << "Low-stock products (" << products.size() << "):" << std::endl;
    for (const auto& product : products) {
        std::cout << product.to_string() << std::endl;
    }
}

void ProductMenu::show_out_of_stock_products() {
    std::vector<Product> products = product_service_.get_out_of_stock_products();
    if (products.empty()) {
        std::cout << "No out-of-stock products." << std::endl;
        return;
    }
    std::cout << "Out-of-stock products (" << products.size() << "):" << std::endl;
    for (const auto& product : products) {
        std::cout << product.to_string() << std::endl;
    }
}

void ProductMenu::show_statistics() {
    std::cout << "\n===== Product Statistics =====" << std::endl;
    std::cout << "Total products:       " << product_service_.get_total_products_count() << std::endl;
    std::cout << "Low-stock products:   " << product_service_.get_low_stock_count() << std::endl;
    std::cout << "Out-of-stock products:" << product_service_.get_out_of_stock_count() << std::endl;
}

