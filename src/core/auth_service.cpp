#include "auth_service.h"
#include "picosha2.h"
#include <random>
#include <string>

AuthService::AuthService(UserRepository& user_repository, Session& session)
    : user_repository_(user_repository), session_(session) {}

ServiceStatus AuthService::register_user(const std::string& username,
                                const std::string& plain_password,
                                Role role)
{
    if (user_repository_.exists_by_username(username))
    {
        return ServiceStatus::ALREADY_EXISTS;
    }

    std::string hashed_password = hash_password(plain_password);
    User new_user(username, hashed_password, role);

    return user_repository_.create(new_user) ? ServiceStatus::SUCCESS : ServiceStatus::ALREADY_EXISTS;
}

ServiceStatus AuthService::login(const std::string& username, const std::string& plain_password)
{
    if (session_.is_logged_in())
    {
        return ServiceStatus::USER_ALREADY_LOGGED_IN;
    }

    std::optional<User> found_user = user_repository_.find_by_username(username);

    if (!found_user.has_value())
    {
        return ServiceStatus::NOT_FOUND;
    }

    if (!verify_password(plain_password, found_user->get_password_hash()))
    {
        return ServiceStatus::INVALID_PASSWORD;
    }

    session_.login(found_user.value());
    return ServiceStatus::SUCCESS;
}

ServiceStatus AuthService::logout()
{
    if (!session_.is_logged_in())
    {
        return ServiceStatus::USER_NOT_LOGGED_IN;
    }
    session_.logout();
    return ServiceStatus::SUCCESS;
}

bool AuthService::is_logged_in() const
{
    return session_.is_logged_in();
}


ServiceStatus AuthService::change_password(int id, const std::string& old_password, const std::string& new_password)
{
    std::optional<User> found_user = user_repository_.find_by_id(id);
    if (!found_user.has_value())
    {
        return ServiceStatus::NOT_FOUND;
    }

    if (!verify_password(old_password, found_user->get_password_hash()))
    {
        return ServiceStatus::INVALID_PASSWORD;
    }

    std::string new_hashed_password = hash_password(new_password);
    found_user->set_password_hash(new_hashed_password);
    user_repository_.update(found_user.value());
    return ServiceStatus::SUCCESS;
}




std::string AuthService::sha256(const std::string& input)
{
    return picosha2::hash256_hex_string(input);
}

std::string AuthService::generate_salt(size_t length)
{
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, sizeof(charset) - 2);

    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
        salt += charset[distribution(generator)];
    }
    return salt;
}

std::string AuthService::hash_password(const std::string& plain_password)
{
    std::string salt = generate_salt();
    std::string hashed = sha256(salt + plain_password);
    return salt + ":" + hashed;
}

bool AuthService::verify_password(const std::string& plain_password, const std::string& stored_hash)
{
    size_t separator_pos = stored_hash.find(':');
    if (separator_pos == std::string::npos)
    {
        return false;
    }

    std::string salt = stored_hash.substr(0, separator_pos);
    std::string expected_hash = stored_hash.substr(separator_pos + 1);

    return sha256(salt + plain_password) == expected_hash;
}