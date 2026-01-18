#include "sqlite3.h"
#include <iostream>

namespace SqlitoSeguro
{
    class Database
    {
        private: 
            sqlite3_stmt* stmt;
            sqlite3* db;
            char *zErrMsg = 0;

        public:
            Database() : db(nullptr), stmt(nullptr) 
            {
                //modificar el path, tiene que quedar en carpeta ~/.local/share/your_app/ (si no existe la carpeta crearla)
                int rc {sqlite3_open("test.db", &db)};
                if (rc != SQLITE_OK)
                {
                    std::cerr << "La base de datos no existe... creandola" << "\n";
                    //crear base de datos y tablas
                }
                else
                {
                    std::cout << "Opened database successfully" << "\n";
                }
                
            }
    };
}
