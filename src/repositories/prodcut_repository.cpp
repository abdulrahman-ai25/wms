#include "product_repository.h"

#include <vector>

Product ProductRepository::map_row(sqlite3_stmt* stmt)
{
    int id = sqlite3_column_int(stmt, 0);
    std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    int category_id = sqlite3_column_int(stmt, 2);
    int quantity = sqlite3_column_int(stmt, 3);
    int minimum_quantity = sqlite3_column_int(stmt, 4);
    double default_selling_price = sqlite3_column_double(stmt, 5);
    std::string description;
    if (sqlite3_column_type(stmt, 6) != SQLITE_NULL)
        description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

    Product product(id, name, category_id, quantity, minimum_quantity, default_selling_price, description);

    if (sqlite3_column_count(stmt) > 7 && sqlite3_column_type(stmt, 7) != SQLITE_NULL)
        product.set_category_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));

    return product;
}

ProductRepository::ProductRepository(Database& db) : database_(db) {}

std::optional<Product> ProductRepository::find_by_id(int id) {
    std::optional<Product> result;

    database_.query(
        "SELECT p.id, p.name, p.category_id, p.quantity, p.minimum_quantity, p.default_selling_price, p.description, c.name "
        "FROM products p "
        "LEFT JOIN categories c ON p.category_id = c.id "
        "WHERE p.id = ?;",
        {id},
        [&result](sqlite3_stmt* stmt) {
            result = ProductRepository::map_row(stmt);
        }
    );

    return result;
}
std::optional<Product> ProductRepository::find_by_name(const std::string& name)
{
    std::optional<Product> result;

    database_.query(
        "SELECT p.id, p.name, p.category_id, p.quantity, p.minimum_quantity, p.default_selling_price, p.description, c.name "
        "FROM products p "
        "LEFT JOIN categories c ON p.category_id = c.id "
        "WHERE p.name = ?;",
        {name},
        [&result](sqlite3_stmt* stmt) {
            result = ProductRepository::map_row(stmt);
        }
    );

    return result;
}

std::vector<Product> ProductRepository::get_all()
{
    std::vector<Product> products;

    database_.query(
        "SELECT p.id, p.name, p.category_id, p.quantity, p.minimum_quantity, p.default_selling_price, p.description, c.name "
        "FROM products p "
        "LEFT JOIN categories c ON p.category_id = c.id;",
        {},
        [&products](sqlite3_stmt* stmt) {
            products.push_back(ProductRepository::map_row(stmt));
        }
    );

    return products;
}

std::vector<Product> ProductRepository::get_low_stock_products()
{
    std::vector<Product> products;

    database_.query(
        "SELECT p.id, p.name, p.category_id, p.quantity, p.minimum_quantity, p.default_selling_price, p.description, c.name "
        "FROM products p "
        "LEFT JOIN categories c ON p.category_id = c.id "
        "WHERE p.quantity > 0 AND p.quantity < p.minimum_quantity;",
        {},
        [&products](sqlite3_stmt* stmt) {
            products.push_back(ProductRepository::map_row(stmt));
        }
    );

    return products;
}

std::vector<Product> ProductRepository::get_out_of_stock_products()
{
    std::vector<Product> products;

    database_.query(
        "SELECT p.id, p.name, p.category_id, p.quantity, p.minimum_quantity, p.default_selling_price, p.description, c.name "
        "FROM products p "
        "LEFT JOIN categories c ON p.category_id = c.id "
        "WHERE p.quantity = 0;",
        {},
        [&products](sqlite3_stmt* stmt) {
            products.push_back(ProductRepository::map_row(stmt));
        }
    );

    return products;
}

std::vector<Product> ProductRepository::get_products_by_category(int category_id)
{
    std::vector<Product> products;

    database_.query(
        "SELECT p.id, p.name, p.category_id, p.quantity, p.minimum_quantity, p.default_selling_price, p.description, c.name "
        "FROM products p "
        "LEFT JOIN categories c ON p.category_id = c.id "
        "WHERE p.category_id = ?;",
        {category_id},
        [&products](sqlite3_stmt* stmt) {
            products.push_back(ProductRepository::map_row(stmt));
        }
    );

    return products;
}

bool ProductRepository::create(const Product& product)
{
    return database_.execute(
        "INSERT INTO products(name, category_id, quantity, minimum_quantity, default_selling_price, description) VALUES (?, ?, ?, ?, ?, ?);",
        {product.get_name(), product.get_category_id(), product.get_quantity(), product.get_minimum_quantity(), product.get_default_selling_price(), product.get_description()}
    );
}
bool ProductRepository::update(const Product& product)
{
    if (!product.get_id().has_value())
    {
        return false;
    }

    return database_.execute(
        "UPDATE products SET name = ?, category_id = ?, quantity = ?, minimum_quantity = ?, default_selling_price = ?, description = ? WHERE id = ?;",
        {product.get_name(), product.get_category_id(), product.get_quantity(), product.get_minimum_quantity(), product.get_default_selling_price(), product.get_description(), product.get_id().value()}
    );
}
bool ProductRepository::remove(int id)
{
    return database_.execute(
        "DELETE FROM products WHERE id = ?;",
        {id}
    );
}
bool ProductRepository::exists_by_name(const std::string& name)
{
    bool found = false;

    database_.query(
        "SELECT 1 FROM products WHERE name = ? LIMIT 1;",
        {name},
        [&found](sqlite3_stmt* /*stmt*/) {
            found = true;
        }
    );

    return found;
}

int ProductRepository::count(){
    int count = 0;

    database_.query(
        "SELECT count(*) FROM products;",
        {},
        [&count](sqlite3_stmt* stmt) {
            count = sqlite3_column_int(stmt, 0);
        }
    );

    return count;
}
int ProductRepository::count_by_category(int category_id){
    int count = 0;

    database_.query(
        "SELECT count(*) FROM products WHERE category_id = ?;",
        {category_id},
        [&count](sqlite3_stmt* stmt) {
            count = sqlite3_column_int(stmt, 0);
        }
    );

    return count;
}
int ProductRepository::count_low_stock_products(){
    int count = 0;

    database_.query(
        "SELECT count(*) FROM products WHERE quantity > 0 AND quantity < minimum_quantity;",
        {},
        [&count](sqlite3_stmt* stmt) {
            count = sqlite3_column_int(stmt, 0);
        }
    );

    return count;
}
int ProductRepository::count_out_of_stock_products(){
    int count = 0;

    database_.query(
        "SELECT count(*) FROM products WHERE quantity = 0;",
        {},
        [&count](sqlite3_stmt* stmt) {
            count = sqlite3_column_int(stmt, 0);
        }
    );

    return count;
}

bool ProductRepository::update_quantity(int id, int quantity)
{
    return database_.execute(
        "UPDATE products SET quantity = ? WHERE id = ?;",
        {quantity, id}
    );
}

bool ProductRepository::update_default_selling_price(int id, double price)
{
    return database_.execute(
        "UPDATE products SET default_selling_price = ? WHERE id = ?;",
        {price, id}
    );
}
