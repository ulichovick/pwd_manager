#include <iostream>
#include <map>
#include <vector>
#include "database.h"
#include "migracion.h"

void SqlitoSeguro::migrationManager::initialize()
{
    if (this->getSchemaVersion() == 0)
    {
        db.createSchema();
        int newVer {this->setSchemaVersion(1)};
        db.backupDatabase();
    }
    else
    {
        std::cout << "Existing Database successfully opened!" << "\n";
    }
}


int SqlitoSeguro::migrationManager::getSchemaVersion()
{
    int rc;
    std::string query {"PRAGMA user_version;"};
    /* rc = sqlite3_prepare_v2(db, "PRAGMA user_version;", -1, &stmt, nullptr ); */
    /* rc = sqlite3_step(stmt); */
    int schemaVersion {db.executeScalar(query)};
    return schemaVersion;
}

int SqlitoSeguro::migrationManager::setSchemaVersion(int current_ver)
{
    int rc;
    std::string query = "PRAGMA user_version=?;";
    std::map<int, std::string> values {
        {1, std::to_string(current_ver)}
    };
    std::map<int, std::vector<std::string>> res {db.executeDQL(query, values)};
    int cur_ver{};
    for (const auto& [key, value] : res)
    {
        cur_ver = std::stoi(value[0]);
        std::cout << "bienvenido " << value[0] << "\n";
    }
    return cur_ver;
}
