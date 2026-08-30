#include <iostream>
#include <map>
#include <vector>
#include "database.h"
#include "migracion.h"

void SqlitoSeguro::migrationManager::initialize()
{
    if (this->getSchemaVersion() == 0)
    {
        this->createSchema();
        int newVer {this->setSchemaVersion(1)};
        db.backupDatabase();
    }
}

/* cambiar la columna created_at a last_updated */
void SqlitoSeguro::migrationManager::createSchema()
{
    std::string userSql = "CREATE TABLE users ("
        "id INTEGER PRIMARY KEY,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL,"
        "created_at TEXT NOT NULL"
        ");" ;

    std::string accountsSql = "CREATE TABLE accounts ("
        "id INTEGER PRIMARY KEY,"
        "user_id INTEGER NOT NULL,"
        "service TEXT NOT NULL,"
        "login TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "created_at TEXT NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

    int exitCode {};

    exitCode = db.executeScalar(userSql);

    if (exitCode != SQLITE_OK) {
        std::cerr << "Error creating table Users: " << "\n";
    } 
    else {
        std::cout << "Table Users created successfully" << "\n";
    }

    exitCode = db.executeScalar(accountsSql);

    if (exitCode != SQLITE_OK) {
        std::cerr << "Error creating table Accounts: " << "\n";
    } else {
        std::cout << "Table Accounts created successfully" << "\n";
    }
}

int SqlitoSeguro::migrationManager::getSchemaVersion()
{
    int rc;
    std::string query {"PRAGMA user_version;"};
    int schemaVersion {db.executeScalar(query)};
    return schemaVersion;
}

int SqlitoSeguro::migrationManager::setSchemaVersion(int current_ver)
{
    int rc;
    std::string query = "PRAGMA user_version="+ std::to_string(current_ver);;
    int res {db.executeScalar(query)};
    std::cout << "Aplicación actualizada exitosamente de la versión " << res <<  " a la versión "<< this->getSchemaVersion() << "\n";
    return res;
}
