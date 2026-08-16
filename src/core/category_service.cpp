#include "category_service.h"
#include "category.h"
#include <optional>


CategoryService::CategoryService(CategoryRepository& category_repository):category_repository_(category_repository)
{}


    
    std::vector<Category> CategoryService::get_all_categories() {
        return category_repository_.get_all();
    }

    std::optional<Category> CategoryService::get_category_by_id(int id)
    {
        return category_repository_.find_by_id(id);
    }

    ServiceStatus CategoryService::add_category(const std::string& name, const std::string& description)
    {
        if (category_repository_.exists_by_name(name))
        {
            return ServiceStatus::ALREADY_EXISTS;
        }
        if (!category_repository_.create(Category(name, description)))
        {
            return ServiceStatus::OPERATION_FAILED;
        }
        return ServiceStatus::SUCCESS;
    }

    ServiceStatus CategoryService::update_category(int id, const std::string& name, const std::string& description)
    {
        std::optional<Category> category=category_repository_.find_by_id(id);
        if (!category.has_value())
        {
            return ServiceStatus::NOT_FOUND;
        }
        if(!name.empty())
            category->set_name(name);
        if(!description.empty())
            category->set_description(description);
       

        if (!category_repository_.update(category.value()))
        {
            return ServiceStatus::OPERATION_FAILED;
        }
        return ServiceStatus::SUCCESS;
    }

    ServiceStatus CategoryService::delete_category(int id)
    {
        if (!category_repository_.find_by_id(id).has_value())
        {
            return ServiceStatus::NOT_FOUND;
        }
        if (!category_repository_.remove(id))
        {
            return ServiceStatus::OPERATION_FAILED;
        }
        return ServiceStatus::SUCCESS;
    }


