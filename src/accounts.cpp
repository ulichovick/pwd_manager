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
        {1, std::to_string(usrId)},
        {2, service},
        {3, login},
        {4, password},
        {5, hora}
    };
    db.executeDML(query, values);
}

/* arreglar el listar cuentas */
void SqlitoSeguro::accountManager::listAccounts(int usrId)
{
    std::string query {"SELECT id, service FROM accounts WHERE user_id=?;"};
    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query, usrId);
    std::cout << "ID" << "\t"  << " Nombre " << "\n";
    for (const auto& [key, values] : res)
    {
        std::cout << key << "\t" << values[0] << "\n";
    }
}

void SqlitoSeguro::accountManager::detailAccount(int usrId, int accId)
{
    std::string query {"SELECT id, service, login, password, created_at  FROM accounts WHERE user_id=? AND id=?;"};
    std::map<int, std::vector<std::string>> res;
    res = db.executeDQL(query, usrId, accId);
    std::cout << "ID" << "\t" << "Nombre" << "\t" << "Username" << "\t" << "Contraseña" << "\t" << "Fecha de creación" << "\t" << "\n";
    for (const auto& [key, values] : res)
    {
        for (const auto& cuenta : values)
        {
            std::cout << cuenta << "\t";
        }
    }
    std::cout << "\n";
}

void SqlitoSeguro::accountManager::deleteAccount(int usrId, int accId)
{
    std::string query {"DELETE FROM accounts WHERE user_id=? AND id=?;"};
    std::map<int, std::vector<std::string>> res;
    std::map<int, std::string> values {
        {1, std::to_string(usrId)},
        {2, std::to_string(accId)}
    };
    db.executeDML(query, values);
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
        {5, std::to_string(usrId)},
        {6, std::to_string(accId)}
    };
    db.executeDML(query, values);
    std::cout << "cuenta editada exitosamente" << "\n";
}
