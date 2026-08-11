#include "users_service.h"
#include "auth_service.h"
UsersService::UsersService(UserRepository& user_repository,AuthService& auth_service) : user_repository_(user_repository) ,auth_service_(auth_service){}

UsersService::~UsersService() {}

std::vector<User> UsersService::get_all_users(Role caller_role) const
{
    if (caller_role != Role::ADMIN)
    {
        return {};
    }
    return user_repository_.get_all();
}

std::optional<User> UsersService::get_user_by_id(Role caller_role,int id) const
{
    if(caller_role == Role::ADMIN){
        return user_repository_.find_by_id(id);
    }
    return std::nullopt;
}


ServiceStatus UsersService::add_user(Role caller_role, const std::string& username, const std::string& plain_password, Role user_role)
{
    if(caller_role != Role::ADMIN){
        return ServiceStatus::PERMISSION_DENIED;
    }
    return auth_service_.register_user(username, plain_password, user_role);
}

ServiceStatus UsersService::update_role(Role caller_role, int id, Role new_role)
{
    if (caller_role != Role::ADMIN)
    {
        return ServiceStatus::PERMISSION_DENIED;
    }
    std::optional<User> found_user = user_repository_.find_by_id(id);
    if (!found_user.has_value())
    {
        return ServiceStatus::USER_NOT_FOUND;
    }
    found_user->set_role(new_role);
    user_repository_.update(found_user.value());
    return ServiceStatus::SUCCESS;
}

ServiceStatus UsersService::delete_user(Role caller_role, int id)
{
    if (caller_role != Role::ADMIN)
    {
        return ServiceStatus::PERMISSION_DENIED;
    }
    if (!user_repository_.find_by_id(id).has_value())
    {
        return ServiceStatus::USER_NOT_FOUND;
    }
    user_repository_.remove(id);
    return ServiceStatus::SUCCESS;
}

ServiceStatus UsersService::update_username(Role caller_role, int id, const std::string& new_username)
{
    if (caller_role != Role::ADMIN)
    {
        return ServiceStatus::PERMISSION_DENIED;
    }
    std::optional<User> found_user = user_repository_.find_by_id(id);
    if (!found_user.has_value())
    {
        return ServiceStatus::USER_NOT_FOUND;
    }
    if (user_repository_.exists_by_username(new_username))
    {
        return ServiceStatus::USER_ALREADY_EXISTS;
    }
    found_user->set_username(new_username);
    user_repository_.update(found_user.value());
    return ServiceStatus::SUCCESS;
}
