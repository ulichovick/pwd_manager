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

/* arreglar el listar cuentas */
void SqlitoSeguro::accountManager::listAccounts()
{
    std::string query {"SELECT id, service FROM accounts WHERE user_id=?"};
    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query);
    std::cout << "ID" << "\t"  << " Nombre " << "\n";
    for (const auto& [key, values] : res)
    {
        std::cout << key << "\t" << values[0] << "\n";
    }
}

void SqlitoSeguro::accountManager::detailAccount(int accId)
{
    std::string query {"SELECT id, service, login, password, created_at  FROM accounts WHERE user_id=? AND id=?;"};
    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query, accId);
    std::cout << "ID" << "\t" << "Nombre" << "\t" << "Username" << "\t" << "Contraseña" << "\t" << "Fecha de creación" << "\t" << "\n";
    for (const auto& [key, values] : res)
    {
        currSess.accId = key;
        for (const auto& cuenta : values)
        {
            std::cout << cuenta << "\t";
        }
    }
    std::cout << "\n";
}

void SqlitoSeguro::accountManager::deleteAccount()
{
    std::string query {"DELETE FROM accounts WHERE user_id=? AND id=?;"};
    std::map<int, std::vector<std::string>> res;
    db.executeDML(query);
}
