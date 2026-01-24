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
            sqlite3_stmt* stmt;
            sqlite3* db;
            char *zErrMsg = 0;

        public:
            Database(): db(nullptr)
            {
                const char* home {std::getenv("HOME")};
                std::filesystem::path directorio {std::filesystem::path(home) / ".local" / "share" / "pwd_manager"};
                std::string directorio_str {directorio.string()};
                
                std::filesystem::path db_path {directorio / "vault.db"};
                std::string db_str {db_path.string()};
                
                int rc {sqlite3_open_v2(db_str.c_str(), &db, SQLITE_OPEN_READWRITE, NULL)};

                if (rc != SQLITE_OK)
                {
                    //std::cerr << sqlite3_errmsg(db) << " la base de datos no existe! Creando la base de datos, " << "\n";
                    std::filesystem::create_directories(directorio_str);

                    rc = sqlite3_open_v2(db_str.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

                    if (rc == SQLITE_OK)
                    {
                        std::cout << " base de datos creada " << rc << "\n";
                    }
                    else
                    {
                        //std::cerr << sqlite3_errmsg(db) << " la base de datos no pudo ser creada " << "\n";
                    }
                    
                    }
                else
                {
                    std::cout << "Base de datos abierta exitosamente" << "\n";
                }
                
            }
            ~Database()
            {
                if (db)
                {
                    sqlite3_close(db);
                    std::cout << "Database connection closed." << "\n";
                }
                
            }
    };
}

#endif
