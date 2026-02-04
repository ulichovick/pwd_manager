#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>

namespace SqlitoSeguro
{
    class Database
    {
        private: 
            sqlite3_stmt* stmt = nullptr;
            sqlite3* db = nullptr;
            char *zErrMsg = 0;

        public:
            Database(const std::filesystem::path& path);
            ~Database();
            void initialize();
            void createSchema();
            int getSchemaVersion();
            int setSchemaVersion(int current_ver);
    };
}

#endif
