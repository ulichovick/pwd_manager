#ifndef MIGRACION_H
#define MIGRACION_H
#include <iostream>
#include "database.h"

namespace SqlitoSeguro
{
    class migrationManager
    {
        private:
            Database& db;
        public:
            migrationManager(Database& database): db(database){};
            void initialize();
            bool addUrl();
            void createSchema();
            int getSchemaVersion();
            int setSchemaVersion(int current_ver);
    };
}

#endif
