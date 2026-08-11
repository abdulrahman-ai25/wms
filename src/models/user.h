#pragma once

#include <optional>
#include <stdexcept>
#include <string>

enum class Role
{
    ADMIN,
    EMPLOYEE
};

inline std::string role_to_string(Role role)
{
    return role == Role::ADMIN ? "admin" : "employee";
}

inline Role role_from_string(const std::string& str)
{
    if (str == "admin")
    {
        return Role::ADMIN;
    }
    if (str == "employee")
    {
        return Role::EMPLOYEE;
    }
    throw std::invalid_argument("Invalid user role");
}

class User
{
private:
    std::optional<int> id_;
    std::string username_;
    std::string password_hash_;
    Role role_;
    std::optional<std::string> created_at_;

public:
    User(int id, const std::string& username, const std::string& password_hash, Role role,
         const std::string& created_at)
        : id_(id), username_(username), password_hash_(password_hash), role_(role), created_at_(created_at)
    {
    }
    User(const std::string& username, const std::string& password_hash, Role role)
        : id_(std::nullopt), username_(username), password_hash_(password_hash), role_(role), created_at_(std::nullopt)
    {
    }

    std::optional<int> get_id() const { return id_; }

    const std::string& get_username() const { return username_; }

    const std::string& get_password_hash() const { return password_hash_; }

    Role get_role() const { return role_; }

    std::optional<std::string> get_created_at() const { return created_at_; }

    void set_username(const std::string& username) { username_ = username; }

    void set_password_hash(const std::string& password_hash) { password_hash_ = password_hash; }

    void set_role(Role role) { role_ = role; }

    std::string to_string() const
    {
        return "ID: " + (id_.has_value()?std::to_string(id_.value()):"N/A") + " | Username: " + username_ /*+ " | " + password_hash_ */ 
        +" | Role: " + role_to_string(role_) + " | Created At: " + (created_at_.has_value()?created_at_.value():"N/A");
    }
};
