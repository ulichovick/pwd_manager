#include <iostream>
#include "database.h"
#include "users.h"
#include <map>
#include <chrono>
#include <format>
#include <vector>


void SqlitoSeguro::userManager::createUser(std::string username, std::string password)
{
    if (username.empty() || password.empty())
    {
        throw std::invalid_argument("invalid input");
    }
    
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

/* actualizar para pasar como argumento usu y contrasena */
std::optional<int> SqlitoSeguro::userManager::Authenticate(std::string& username, std::string& password)
{
    std::string query {"SELECT id, username, password FROM users WHERE username=?;"};
    std::map<int, std::string> values{
        {1, username}
    };
    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query, values);
    if (res.empty())
        return std::nullopt;
    int usrId {};
    std::string pwd {};
    for (const auto& [key, value] : res)
    {
        usrId = key;
        pwd = value[1];
    }
    if (pwd == password)
    {
        std::cout << "bienvenido " << values[0] << "\n";
        return usrId;
    }
    return std::nullopt;
}
