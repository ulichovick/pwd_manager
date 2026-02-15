#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>
#include "database.h"

namespace SqlitoSeguro
{
    class accountsManager
    /*
    solo declarar las clases, toda la definicion  se implementa en el .cpp de la clase
    */
    {
        private:
            Database& db;
        public:
            //constructor
            accountsManager(Database& database);

            void addAccount(int userID,
                            const std::string& service,
                            const std::string& username,
                            const std::string& password);
    };
}

#endif
