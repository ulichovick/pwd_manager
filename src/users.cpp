#include <iostream>
#include "database.h"
#include "users.h"
#include <map>
#include <chrono>
#include <format>



void SqlitoSeguro::userManager::createUser(const std::string& username,
                                            const std::string& password)
{
    std::string query {"INSERT INTO USERS (username, password, created_at) VALUES (?, ?, ?);"};
    const auto now = std::chrono::system_clock::now();
    auto floorTime {std::chrono::floor<std::chrono::seconds>(now)};
    auto time {std::chrono::zoned_time{std::chrono::current_zone(), floorTime}};
    std::string hora {std::format("{:%Y-%m-%d-%T}", time)};
    std::map<int, std::string> values {
        {1, username},
        {2, password},
        {3, hora}
    };
    db.executeDML(query, values);
}

void SqlitoSeguro::userManager::Authenticate(const std::string& username,
                                            const std::string& password)
{
    std::string query {"SELECT username FROM users WHERE username=? AND password=?;"};
    std::map<int, std::string> values{
        {1, username},
        {2, password}
    };
    db.executeDQL(query, values);
}
