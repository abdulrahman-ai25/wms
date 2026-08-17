#pragma once
#include <optional>
#include <string>

class Product{
  //read data from migration and convert to model :
    // CREATE TABLE IF NOT EXISTS products (
    //         id INTEGER PRIMARY KEY AUTOINCREMENT,
    //         name TEXT NOT NULL,
    //         category_id INTEGER NOT NULL,
    //         quantity INTEGER NOT NULL DEFAULT 0 CHECK(quantity >= 0),
    //         minimum_quantity INTEGER NOT NULL DEFAULT 10 CHECK(minimum_quantity >= 0),
    //         default_selling_price REAL NOT NULL DEFAULT 0 CHECK(default_selling_price >= 0),
    //         description TEXT,
    //         FOREIGN KEY (category_id) REFERENCES categories(id)
    //     );
    private:
    std::optional<int> id_;
    std::string name_;
    int category_id_;
    int quantity_;
    int minimum_quantity_;
    double default_selling_price_;
    std::string description_;
    std::string category_name_;

    public:
        Product(int id, const std::string& name, int category_id, int quantity, int minimum_quantity, double default_selling_price, const std::string& description)
        : id_(id), name_(name), category_id_(category_id), quantity_(quantity), minimum_quantity_(minimum_quantity), default_selling_price_(default_selling_price), description_(description)
        {
        }
        Product(const std::string& name, int category_id, int quantity, int minimum_quantity, double default_selling_price, const std::string& description)
        : id_(std::nullopt), name_(name), category_id_(category_id), quantity_(quantity), minimum_quantity_(minimum_quantity), default_selling_price_(default_selling_price), description_(description)
        {
        }

        std::optional<int> get_id() const { return id_; }

        const std::string& get_name() const { return name_; }

        int get_category_id() const { return category_id_; }

        int get_quantity() const { return quantity_; }

        int get_minimum_quantity() const { return minimum_quantity_; }

        double get_default_selling_price() const { return default_selling_price_; }

        const std::string& get_description() const { return description_; }

        void set_name(const std::string& name) { name_ = name; }

        void set_category_id(int category_id) { category_id_ = category_id; }

        void set_quantity(int quantity) { quantity_ = quantity; }

        void set_minimum_quantity(int minimum_quantity) { minimum_quantity_ = minimum_quantity; }

        void set_default_selling_price(double default_selling_price) { default_selling_price_ = default_selling_price; }

        void set_description(const std::string& description) { description_ = description; }

        void set_category_name(const std::string& category_name) { category_name_ = category_name; }

        std::string to_string() const
        {
            return "ID: " + (id_.has_value()?std::to_string(id_.value()):"N/A") + " | Name: " + name_ + " | Category Name: " + category_name_ + " | Quantity: " + std::to_string(quantity_) + " | Minimum Quantity: " + std::to_string(minimum_quantity_) + " | Default Selling Price: " + std::to_string(default_selling_price_) + " | Description: " + description_;
        }

};