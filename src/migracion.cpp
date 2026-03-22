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
    std::string query = "PRAGMA user_version="+ std::to_string(current_ver);;
    int res {db.executeScalar(query)};
    std::cout << "Aplicación actualizada exitosamente de la versión " << res <<  " a la versión "<< this->getSchemaVersion() << "\n";
    return res;
}
