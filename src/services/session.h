#pragma once
#include "user.h"
#include <optional>

class Session
{
public:
    Session() = default;
    ~Session() = default;
    void login(const User& user) { current_user_ = user; }
    void logout() { current_user_ = std::nullopt; }

    bool is_logged_in() const { return current_user_.has_value(); }
    const User& get_current_user() const { return current_user_.value(); }

private:
    std::optional<User> current_user_;
};