#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <map>

namespace SqlitoSeguro
{
    class Database
    {
        private: 
            sqlite3_stmt* stmt = nullptr;
            sqlite3* db = nullptr;
            char *zErrMsg = 0;
            const std::filesystem::path m_path;

        public:
            Database(const std::filesystem::path& path);
            ~Database();
            void initialize();
            void createSchema();
            int getSchemaVersion();
            int setSchemaVersion(int current_ver);
            void backupDatabase();
            void executeDML(std::string& query, std::map<int, std::string>& values);
            void executeDQL(std::string& query, std::map<int, std::string>& values);
    };
}

#endif
