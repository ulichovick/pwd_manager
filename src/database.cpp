#include "sqlite3.h"
#include "database.h"
#include "accounts.h"
#include "users.h"
#include "migracion.h"
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <map>
#include <optional>

/* database class definitinition | declaration */
SqlitoSeguro::Database::Database(const std::filesystem::path& path) : m_path(path)
{
    if (!std::filesystem::exists(path.parent_path()))
    {
        std::filesystem::create_directories(path.parent_path());
    }
    std::string dbPath {path.string()};

    bool isNew = !std::filesystem::exists(dbPath);

    int rc = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (rc != SQLITE_OK)
    {
        std::string err = sqlite3_errmsg(db);
        sqlite3_close(db);
        db = nullptr;
        throw std::runtime_error("SQLite open failed: " + err);
    }
    std::filesystem::permissions(
        path.parent_path(),
        std::filesystem::perms::owner_all,
        std::filesystem::perm_options::replace);

    if (isNew) {
    std::filesystem::permissions(
        dbPath,
        std::filesystem::perms::owner_read |
        std::filesystem::perms::owner_write,
        std::filesystem::perm_options::replace
    );
    }
    sqlite3_exec(db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, nullptr);
}

SqlitoSeguro::Database::~Database()
{
    if (db)
    {
        sqlite3_close(db);
    }
}

int SqlitoSeguro::Database::executeScalar(const std::string& query)
{
    sqlite3_stmt* stmt = nullptr;
    int rc;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr );
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error executing Query: " << sqlite3_errmsg(db) << "\n";
    }
    rc = sqlite3_step(stmt);
    int res {};
    if (rc == SQLITE_ROW)
    {
        
        res = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return res;
}

void SqlitoSeguro::Database::backupDatabase()
{
    sqlite3* backupDB = nullptr;
    std::string nstrPath = m_path.parent_path().string();

    const auto time = std::chrono::system_clock::now();
    nstrPath = nstrPath + "/backup_" + std::format("{:%Y-%m-%d}", time) +".db";
    int rc = sqlite3_open_v2(nstrPath.c_str() , &backupDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK)
        throw std::runtime_error("Cannot open backup database");
    sqlite3_backup* backup = sqlite3_backup_init(
        backupDB,
        "main",
        db,
        "main"
    );

    if (sqlite3_backup_step(backup, -1) != SQLITE_DONE)
    {
        sqlite3_backup_finish(backup);
        throw std::runtime_error("Backup failed");
    }
    sqlite3_close(backupDB);
    std::filesystem::permissions(nstrPath,
        std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
        std::filesystem::perm_options::replace);
}

/* actualizar para bind_int los ID */
void SqlitoSeguro::Database::executeDML(const std::string& query, std::map<int, std::string>& values, int usrid, int accid, std::optional<int> posid, std::optional<int>posaccid)
{
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR AL PREPARAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
    }

    if (posid.has_value() && posaccid.has_value())
    {
        sqlite3_bind_int(stmt, posid.value(), usrid);
        sqlite3_bind_int(stmt, posaccid.value(), accid);
    }
    
    if (!values.empty())
    {
        for (auto const& [key, value]: values)
        {
            sqlite3_bind_text(stmt, key, value.c_str(), -1, SQLITE_STATIC);
        }
    }
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE)
    {
        std::cerr << "ERROR AL EJECUTAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
    }
    else
    {
        std::cout << "consulta ejecutada exitosamente! " << "\n";
        sqlite3_finalize(stmt);
    }
    
}

void SqlitoSeguro::Database::executeDML(const std::string& query, std::map<int, std::string>& values, std::optional<int> usrid, std::optional<int> posid)
{
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR AL PREPARAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
    }

    if (posid.has_value())
    {
        sqlite3_bind_int(stmt, posid.value(), usrid.value());
    }
    
    if (!values.empty())
    {
        for (auto const& [key, value]: values)
        {
            sqlite3_bind_text(stmt, key, value.c_str(), -1, SQLITE_STATIC);
        }
    }
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE)
    {
        std::cerr << "ERROR AL EJECUTAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
    }
    else
    {
        std::cout << "consulta ejecutada exitosamente! " << "\n";
        sqlite3_finalize(stmt);
    }
    
}

std::map<int, std::vector<std::string>> SqlitoSeguro::Database::executeDQL(const std::string& query, std::map<int, std::string>& values)
{
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR AL PREPARAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
    }
    for (auto const& [key, value]: values)
    {
        sqlite3_bind_text(stmt, key, value.c_str(), -1, SQLITE_STATIC);
    }

    int id;
    std::vector<std::string> row;
    int cols {sqlite3_column_count(stmt)};
    std::map<int, std::vector<std::string>> resultados;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
        for (size_t i = 1; i < cols; i++)
        {
            const unsigned char* cell = sqlite3_column_text(stmt, i);
            std::string strCell = reinterpret_cast<const char*>(cell);
            row.push_back(strCell);
        }
        resultados.insert({id, row});
    }
    sqlite3_finalize(stmt);
    return resultados;
}


std::map<int, std::vector<std::string>> SqlitoSeguro::Database::executeDQL(const std::string& query, int usrId, std::optional<int> accId)
{
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR AL PREPARAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
    }
    
    sqlite3_bind_int(stmt, 1, usrId);
    if (accId.has_value())
    {
        sqlite3_bind_int(stmt, 2, accId.value());
    }

    int id;
    std::vector<std::string> row;
    int cols {sqlite3_column_count(stmt)};
    std::map<int, std::vector<std::string>> resultados;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
        if (cols > 2)
        {
            for (size_t i = 0; i < cols; i++)
            {
                const unsigned char* cell = sqlite3_column_text(stmt, i);
                std::string strCell = reinterpret_cast<const char*>(cell);
                row.push_back(strCell);
            }
            resultados.insert({id, row});
        }
        else
        {
            const unsigned char* cell = sqlite3_column_text(stmt, 1);
            std::string strCell = reinterpret_cast<const char*>(cell);
            std::vector<std::string> row{strCell};
            resultados.insert({id, row});
        }
        
    }
    sqlite3_finalize(stmt);
    return resultados;
}
