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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/* database class definitinition | declaration */
SqlitoSeguro::Database::Database(const std::filesystem::path& path) : m_path(path)
{
    if (!std::filesystem::exists(path))
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
    sqlite3_exec(db, "PRAGMA foreign_keys=ON;", callback, 0, &zErrMsg);
}

SqlitoSeguro::Database::~Database()
{
    if (db)
    {
        sqlite3_close(db);
        std::cout << "Database connection closed." << "\n";
    }
}

/* void SqlitoSeguro::Database::initialize()
{
    if (migrador->getSchemaVersion() == 0)
    {
        this->createSchema();
        int newVer {migrador->setSchemaVersion(1)};
        this->backupDatabase();
    }
    else
    {
        std::cout << "Existing Database successfully opened!" << "\n";
    }
} */

void SqlitoSeguro::Database::createSchema()
{
    const char* userSql = "CREATE TABLE users ("
        "id INTEGER PRIMARY KEY,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL,"
        "created_at TEXT NOT NULL"
        ");" ;

    const char* accountsSql = "CREATE TABLE accounts ("
        "id INTEGER PRIMARY KEY,"
        "user_id INTEGER NOT NULL,"
        "service TEXT NOT NULL,"
        "login TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "created_at TEXT NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

    int exitCode = sqlite3_exec(db, userSql, NULL, 0, &zErrMsg);

    if (exitCode != SQLITE_OK) {
        std::cerr << "Error creating table Users: " << sqlite3_errmsg(db) << "\n";
    } 
    else {
        std::cout << "Table Users created successfully" << "\n";
    }

    exitCode = sqlite3_exec(db, accountsSql, NULL, 0, &zErrMsg);

    if (exitCode != SQLITE_OK) {
        std::cerr << "Error creating table Accounts: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Table Accounts created successfully" << "\n";
    }
}

int SqlitoSeguro::Database::executeScalar(std::string& query)
{
    int rc;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr );
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

    if(!backup)
    {
        throw std::runtime_error("backup failed!");
    }
    sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
    sqlite3_close(backupDB);
    std::filesystem::permissions(nstrPath,
        std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
        std::filesystem::perm_options::replace);
}

void SqlitoSeguro::Database::executeDML(std::string& query, std::map<int, std::string>& values)
{
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR AL PREPARAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
    }
    if (currSess.id)
    {
        sqlite3_bind_int(stmt, 1, currSess.id);
    }
    for (auto const& [key, value]: values)
    {
        sqlite3_bind_text(stmt, key, value.c_str(), -1, SQLITE_STATIC);
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

std::map<int, std::vector<std::string>> SqlitoSeguro::Database::executeDQL(std::string& query, std::map<int, std::string>& values)
{
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

/* Cambiar el retorno de la funcion a un mapa de vectores */
std::vector<std::string> SqlitoSeguro::Database::executeDQL(std::string& query)
{
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR AL PREPARAR LA CONSULTA! " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_bind_int(stmt, 1, currSess.id);

    int cols {sqlite3_column_count(stmt)};
    std::vector<std::string> resultados;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        for (size_t i = 0; i < cols; i++)
        {
            const unsigned char* cell = sqlite3_column_text(stmt, i);
            std::string strCell = reinterpret_cast<const char*>(cell);
            resultados.push_back(strCell);
        }
    }
    sqlite3_finalize(stmt);
    return resultados;
}
