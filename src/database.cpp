#include "sqlite3.h"
#include "database.h"
#include <iostream>


std::string definePaths (bool isDbPath)
{
    const char* home {std::getenv("HOME")};
    std::filesystem::path directorio {std::filesystem::path(home) / ".local" / "share" / "pwd_manager"};
    if (!isDbPath)
    {
        std::string directorio_str {directorio.string()};
        return directorio_str;
    }
    std::filesystem::path db_path {directorio / "vault.db"};
    std::string db_str {db_path.string()};
    return db_str;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/* database class definitinition | declaration */
SqlitoSeguro::Database::Database()
{
    std::string dirPath {definePaths(false)};
    std::string dbPath {definePaths(true)};

    int rc {sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE, NULL)};
    if (rc != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << " la base de datos no existe! ... Creando la base de datos. " << "\n";
        std::filesystem::create_directories(dirPath);

        rc = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

        if (rc == SQLITE_OK)
        {
            std::cout << " base de datos creada!" << "\n";
        }
        else
        {
            std::cerr << sqlite3_errmsg(db) << " la base de datos no pudo ser creada " << "\n";
        }
        /* crear tablas y actualizar version del schema*/

        const char* userSql = "CREATE TABLE users ("
        "id INTEGER PRIMARY KEY,"
        "username TEXT NOT NULL UNIQUE,"
        "created_at INTEGER NOT NULL"
        ");" ;

        const char* accountsSql = "CREATE TABLE accounts ("
        "id INTEGER PRIMARY KEY,"
        "user_id INTEGER NOT NULL,"
        "service TEXT NOT NULL,"
        "login TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "created_at INTEGER NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

        int exitCode = sqlite3_exec(db, userSql, NULL, 0, &zErrMsg);

        if (exitCode != SQLITE_OK) {
            std::cerr << "Error creating table: " << sqlite3_errmsg(db) << "\n";
        } 
        else {
            std::cout << "Table created successfully" << "\n";
        }

        exitCode = sqlite3_exec(db, accountsSql, NULL, 0, &zErrMsg);

        if (exitCode != SQLITE_OK) {
            std::cerr << "Error creating table: " << sqlite3_errmsg(db) << "\n";
        } else {
            std::cout << "Table created successfully" << "\n";
        }

        /* int newVer {this->setSchemaVersion()};

        std::cout << "Version actualiada exitosamente: " << newVer << "\n"; */
        }
    else
    {
        std::cout << "Base de datos abierta exitosamente" << "\n";
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
    sqlite3_finalize(stmt);
    return schemaVersion;
}

int SqlitoSeguro::Database::setSchemaVersion()
{
    int rc;
    int ver {SqlitoSeguro::Database::getSchemaVersion()};
    ver++;
    std::string sql = "PRAGMA user_version="+ std::to_string(ver);
    std::cout  << "version: " << ver << "\n";

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to set user_version PRAGMA: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Database successfully set to " << std::to_string(ver) << std::endl;
    }
    sqlite3_finalize(stmt);
    ver = SqlitoSeguro::Database::getSchemaVersion();
    return ver;
}

