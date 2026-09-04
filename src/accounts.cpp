#include <iostream>
#include "accounts.h"
#include "database.h"
#include <map>
#include <chrono>
#include <format>
#include <vector>

void SqlitoSeguro::accountManager::addAccount(const std::string& service,
                                            const std::string& login,
                                            const std::string& password,
                                            int usrId)
{
    std::string query {"INSERT INTO accounts (user_id, service, login, password, created_at) VALUES (?, ?, ?, ?, ?);"};
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
    db.executeDML(query, values, usrId);
}

/* arreglar el listar cuentas */
std::map<int, std::vector<std::string>> SqlitoSeguro::accountManager::listAccounts(int usrId)
{
    std::string query {"SELECT id, service FROM accounts WHERE user_id=?;"};

    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query, usrId);
    return res;
}

std::vector<std::string> SqlitoSeguro::accountManager::detailAccount(int usrId, int accId)
{
    std::string query {"SELECT id, service, login, password, created_at FROM accounts WHERE user_id=? AND id=?;"};
    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query, usrId, accId);
    std::vector<std::string> details;
    for (const auto& [key, values] : res)
    {
        for (const auto& cuenta : values)
        {
            details.push_back(cuenta);
        }
    }
    return details;
}

void SqlitoSeguro::accountManager::deleteAccount(int usrId, int accId)
{
    std::string query {"DELETE FROM accounts WHERE user_id=? AND id=?;"};
    std::map<int, std::vector<std::string>> res;
    std::map<int, std::string> values {};
    db.executeDML(query, values, usrId, accId, 1, 2);
}

/* Temporal hasta tener el GUI */
void SqlitoSeguro::accountManager::editAccount(const std::string& service,
                                            const std::string& login,
                                            const std::string& password,
                                            int usrId,
                                            int accId)
{
    std::string query {"UPDATE accounts SET service= ?, login = ?, password = ?, created_at = ? WHERE user_id=? AND id=?;"};
    const auto now = std::chrono::system_clock::now();
    auto floorTime {std::chrono::floor<std::chrono::seconds>(now)};
    auto time {std::chrono::zoned_time{std::chrono::current_zone(), floorTime}};
    std::string hora {std::format("{:%Y-%m-%d-%T}", time)};
    std::map<int, std::string> values {
        {1, service},
        {2, login},
        {3, password},
        {4, hora},
    };
    db.executeDML(query, values, usrId, accId, 5, 6);
    std::cout << "cuenta editada exitosamente" << "\n";
}
