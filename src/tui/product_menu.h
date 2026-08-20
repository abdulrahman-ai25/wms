#pragma once

#include "i_menu.h"
#include "product_service.h"


class ProductMenu:public IMenu  {

    public:
    ProductMenu(ProductService& product_service);
    MenuResult run() override;
    std::string get_title() const override { return "Products"; }

    private:

    ProductService& product_service_;
    void list_products();
    void search_products();
    void add_product();
    void update_product();
    void delete_product();
    void update_price();
    void show_low_stock_products();
    void show_out_of_stock_products();
    void show_statistics();

};