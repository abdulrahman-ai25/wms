#include "category_menu.h"

#include "category.h"

#include <iostream>

CategoryMenu::CategoryMenu(CategoryService& category_service) : category_service_(category_service) {}

MenuResult CategoryMenu::run()
{
    while (true)
    {
        std::cout << "Categories Menu" << std::endl;
        std::cout << "1. List all categories" << std::endl;
        std::cout << "2. Show category" << std::endl;
        std::cout << "3. Add category" << std::endl;
        std::cout << "4. Delete category" << std::endl;
        std::cout << "5. Update category" << std::endl;
        std::cout << "6. Get all products in category" << std::endl;
        std::cout << "7. Back" << std::endl;
        std::cout << "0. Exit" << std::endl;
        int choice;
        std::cin >> choice;
        switch (choice)
        {
            case 1:
                list_categories();
                break;
            case 2:
                show_category();
                break;
            case 3:
                add_category();
                break;
            case 4:
                delete_category();
                break;
            case 5:
                update_category();
                break;
            case 6:
                get_products_in_category();
                break;
            case 7:
                return MenuResult::BACK;
            case 0:
                return MenuResult::EXIT;
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
        }
    }
}

void CategoryMenu::list_categories()
{
    std::vector<Category> categories = category_service_.get_all_categories();
    if (categories.empty())
    {
        std::cout << "No categories found." << std::endl;
        return;
    }
    for (const auto& category : categories)
        std::cout << category.to_string() << std::endl;
}

void CategoryMenu::show_category()
{
    int id;
    std::cout << "Enter category id: ";
    std::cin >> id;
    std::optional<Category> category = category_service_.get_category_by_id(id);
    if (category.has_value())
    {
        std::cout << category.value().to_string() << std::endl;
    }
    else
    {
        std::cout << "Category not found" << std::endl;
    }
}

void CategoryMenu::add_category()
{
    std::string name, description;

    std::cout << "Enter category name: ";
    read_str(name);
    std::cout << "Enter category description: ";
    read_str(description);

    ServiceStatus status = category_service_.add_category(name, description);

    if (status == ServiceStatus::SUCCESS)
        std::cout << "Category added successfully" << std::endl;
    else if (status == ServiceStatus::ALREADY_EXISTS)
        std::cout << "Category already exists" << std::endl;
    else
        std::cout << "Failed to add category" << std::endl;
}

void CategoryMenu::delete_category() {
        int id;
    std::cout << "Enter category id: ";
    std::cin >> id;
    ServiceStatus status = category_service_.delete_category(id);
    if (status == ServiceStatus::SUCCESS)
        std::cout << "Category deleted successfully" << std::endl;
    else if (status == ServiceStatus::NOT_FOUND)
        std::cout << "Category not found" << std::endl;
    else
        std::cout << "Failed to delete category" << std::endl;
}

void CategoryMenu::update_category() {
   int id;
    std::string new_name;
    std::string new_description;
    std::cout << "Enter category id: ";
    std::cin >> id;
    std::cout << "Enter new name: ";
    read_str(new_name,true);
    std::cout << "Enter new description: ";
    read_str(new_description,true);
    ServiceStatus status = category_service_.update_category(id, new_name, new_description);
    if (status == ServiceStatus::SUCCESS)
        std::cout << "Category updated successfully" << std::endl;
    else if (status == ServiceStatus::NOT_FOUND)
        std::cout << "Category not found" << std::endl;
    else
        std::cout << "Failed to update category" << std::endl;
}
// TODO: implement all products in category
void CategoryMenu::get_products_in_category() {}    
