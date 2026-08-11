#pragma once
#include "user.h"
#include "user_repository.h"
#include "service_status.h"
#include "auth_service.h"
#include <string>
#include <vector>
class UsersService
{
public:
    UsersService(UserRepository& user_repository,AuthService& auth_service);
    ~UsersService();
    std::vector<User> get_all_users(Role caller_role) const;

    std::optional<User> get_user_by_id(Role caller_role, int id) const;

    ServiceStatus add_user(Role caller_role, const std::string& username, const std::string& plain_password, Role user_role = Role::EMPLOYEE);

    ServiceStatus update_role(Role caller_role, int id, Role new_role);

    ServiceStatus delete_user(Role caller_role, int id);

    ServiceStatus update_username(Role caller_role, int id, const std::string& new_username);

private:
    UserRepository& user_repository_;
    AuthService& auth_service_; 
};