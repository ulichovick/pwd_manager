#ifndef USERS_H
#define USERS_H
#include <iostream>
#include "database.h"

namespace SqlitoSeguro
{
    class userManager
    {
        /*
        definir: constructor (crear), actualizar y verificar existencia
        */
        private:
            Database& db;

        public:
            //constructor
            userManager(Database& database);

            void createUser(const std::string& username,
                        const std::string& password);
    };
}

#endif
