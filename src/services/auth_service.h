#pragma once
#include "user.h"
#include "user_repository.h"
#include "session.h"
#include "service_status.h"
#include <string>

class AuthService
{
public:
    AuthService(UserRepository& user_repository, Session& session);

    ServiceStatus register_user(const std::string& username, const std::string& plain_password, Role role = Role::EMPLOYEE);
    ServiceStatus login(const std::string& username, const std::string& plain_password);
    ServiceStatus logout();
    bool is_logged_in() const;
    ServiceStatus change_password(int id, const std::string& old_password, const std::string& new_password);

private:
    UserRepository& user_repository_;
    Session& session_;

    static std::string hash_password(const std::string& plain_password);
    static bool verify_password(const std::string& plain_password, const std::string& stored_hash);
    static std::string generate_salt(size_t length = 16);
    static std::string sha256(const std::string& input);
};