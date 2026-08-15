#pragma once

#include "category_repository.h"
#include "category.h"

#include "service_status.h"

class CategoryService{
    private:
    CategoryRepository& category_repository_;

    public:
    CategoryService(CategoryRepository& category_repository);

    std::vector<Category> get_all_categories();

    std::optional<Category> get_category_by_id(int id);

    ServiceStatus add_category(const std::string& name, const std::string& description="");

    ServiceStatus update_category(int id, const std::string& name, const std::string& description="");

    ServiceStatus delete_category(int id);







};
    


