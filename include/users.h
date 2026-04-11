#ifndef USERS_H
#define USERS_H
#include <iostream>
#include "database.h"

namespace SqlitoSeguro
{
    class userManager
    {
        
        private:
            Database& db;
            std::string m_usuario;
            std::string m_contrasena;

        public:
            //constructor
            userManager(Database& database, 
                        std::string_view usuario, 
                        std::string_view contrasena): db(database),
                            m_usuario(usuario),
                            m_contrasena(contrasena){};

            void createUser();
            int Authenticate();
    };
}

#endif
