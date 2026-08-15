#pragma once
#include <optional>
#include <string>

class Category{
  
    private:
    std::optional<int> id_;
    std::string name_;
    std::string description_;

    public:
        Category(int id, const std::string& name, const std::string& description="")
        : id_(id), name_(name), description_(description)
        {
        }
        Category(const std::string& name, const std::string& description="")
        : id_(std::nullopt), name_(name), description_(description)
        {
        }

        std::optional<int> get_id() const { return id_; }

        const std::string& get_name() const { return name_; }

        const std::string& get_description() const { return description_; }

        void set_name(const std::string& name) { name_ = name; }

        void set_description(const std::string& description) { description_ = description; }

        std::string to_string() const
        {
            return "ID: " + (id_.has_value()?std::to_string(id_.value()):"N/A") + " | Name: " + name_ + " | Description: " + description_;
        }

    

};