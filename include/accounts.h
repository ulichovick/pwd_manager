#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>
#include "database.h"

namespace SqlitoSeguro
{
    struct Accounts{
        int id;
        int userId;
        std::string service;
        std::string username;
        std::string password;
    };

    class accountManager
    /*
    solo declarar las clases, toda la definicion  se implementa en el .cpp de la clase
    */
    {
        private:
            Database& db;
        public:
            //constructor
            accountManager(Database& database): db(database){};

            void addAccount(int userID,
                            const std::string& service,
                            const std::string& username,
                            const std::string& password);
    };
}

#endif
