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
            int test;
        public:
            migrationManager(Database& database, int t): db(database),
                                                            test(t){std::cout << "test \n";};
            void initialize();
            int getSchemaVersion();
            int setSchemaVersion(int current_ver);
    };
}

#endif
