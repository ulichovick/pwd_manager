#include <iostream>
#include "accounts.h"
#include "database.h"
#include <map>
#include <chrono>
#include <format>
#include <vector>

void SqlitoSeguro::accountManager::addAccount(const std::string& service,
                                            const std::string& login,
                                            const std::string& password)
{
    std::string query {"INSERT INTO ACCOUNTS (user_id, service, login, password, created_at) VALUES (?, ?, ?, ?, ?);"};
    const auto now = std::chrono::system_clock::now();
    auto floorTime {std::chrono::floor<std::chrono::seconds>(now)};
    auto time {std::chrono::zoned_time{std::chrono::current_zone(), floorTime}};
    std::string hora {std::format("{:%Y-%m-%d-%T}", time)};
    std::map<int, std::string> values {
        {2, service},
        {3, login},
        {4, password},
        {5, hora}
    };
    db.executeDML(query, values);
}

void SqlitoSeguro::accountManager::listAccounts()
{
    std::string query {"SELECT service FROM accounts WHERE user_id=?;"};
    std::vector<std::string> res;
    res = db.executeDQL(query);
    std::cout << "Cuentas: " << "\n";
    for (const auto& cuenta : res)
    {
        std::cout << cuenta << "\n";
    }
}
