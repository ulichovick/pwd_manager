#include "sqlite3.h"
#include "database.h"
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/* database class definitinition | declaration */
/* mover todos los metodos para construir la bd (getschema, setschema) a otra clase */
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
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cout << "Database connection closed." << "\n";
    }
}

void SqlitoSeguro::Database::initialize()
{
    if (this->getSchemaVersion() == 0)
    {
        this->createSchema();
        int newVer {this->setSchemaVersion(1)};
        this->backupDatabase();
    }
    else
    {
        std::cout << "Database Exists!" << "\n";
    }
}

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


int SqlitoSeguro::Database::getSchemaVersion()
{
    int rc;
    rc = sqlite3_prepare_v2(db, "PRAGMA user_version;", -1, &stmt, nullptr );
    rc = sqlite3_step(stmt);
    int schemaVersion {};
    if (rc == SQLITE_ROW)
    {
        schemaVersion = sqlite3_column_int(stmt, 0);
    }
    /* sqlite3_finalize(stmt); */
    return schemaVersion;
}

/* convertir el execute en un prepared statement */
int SqlitoSeguro::Database::setSchemaVersion(int current_ver)
{
    int rc;
    std::string sql = "PRAGMA user_version="+ std::to_string(current_ver);

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to set user_version PRAGMA: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Database version successfully set to " << std::to_string(current_ver) << "\n";
    }
    /* sqlite3_finalize(stmt); */
    current_ver = SqlitoSeguro::Database::getSchemaVersion();
    return current_ver;
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

void SqlitoSeguro::Database::executeQuery(std::string& query, std::map<int, std::string>& values)
{
    for (auto const& [key, value]: values)
    {
        std::cout << "clave: " << key << " valor: " << value << "\n";
    }
    
}

