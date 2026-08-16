#pragma once

#include "i_menu.h"
#include "category_service.h"


class CategoryMenu:public IMenu  {

    public:
    CategoryMenu(CategoryService& category_service);
    MenuResult run() override;
    std::string get_title() const override { return "Categories"; }

    private:

    CategoryService& category_service_;
    void list_categories();
    void show_category();
    void add_category();
    void delete_category();
    void update_category();
    void get_products_in_category();


};