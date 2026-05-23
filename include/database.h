#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <optional>

namespace SqlitoSeguro
{
    class Database
    {
        private: 
            sqlite3* db;
            const std::filesystem::path m_path;

        public:
            Database(const std::filesystem::path& path);
            ~Database();

            Database(const Database&) = delete;
            Database& operator=(const Database&) = delete;

            Database(Database&&) = delete;
            Database& operator=(Database&&) = delete;
            /* void initialize(); */
            void createSchema();
            int executeScalar(const std::string& query);
            int setSchemaVersion(int current_ver);
            void backupDatabase();
            void executeDML(const std::string& query, std::map<int, std::string>& values,int usrid, int accid, std::optional<int> posid = std::nullopt, std::optional<int>posaccid = std::nullopt);
            void executeDML(const std::string& query, std::map<int, std::string>& values, std::optional<int> usrid = std::nullopt, std::optional<int> posid = std::nullopt);
            std::map<int, std::vector<std::string>> executeDQL(const std::string& query, std::map<int, std::string>& values);
            std::map<int, std::vector<std::string>> executeDQL(const std::string& query, int usrId, std::optional<int> accId = std::nullopt);
    };
}

#endif
