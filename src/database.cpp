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
        std::cerr << sqlite3_errmsg(db) << " la base de datos no existe! Creando la base de datos, " << "\n";
        std::filesystem::create_directories(dirPath);

        rc = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

        if (rc == SQLITE_OK)
        {
            std::cout << " base de datos creada " << rc << "\n";
        }
        else
        {
            std::cerr << sqlite3_errmsg(db) << " la base de datos no pudo ser creada " << "\n";
        }
        
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
    return schemaVersion;
}

int SqlitoSeguro::Database::setSchemaVersion()
{
    sqlite3_exec(db, "PRAGMA user_version=1;", callback, 0, &zErrMsg);
    int ver {SqlitoSeguro::Database::getSchemaVersion()};
    return ver;
}

