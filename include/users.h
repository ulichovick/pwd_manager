#ifndef USERS_H
#define USERS_H
#include <iostream>
#include "database.h"
#include <optional>

namespace SqlitoSeguro
{
    class userManager
    {
        
        private:
            Database& db;
        public:
            //constructor
            userManager(Database& database): db(database){};

            void createUser(std::string username, std::string password);
            std::optional<int> Authenticate(std::string& username, std::string& password);
    };
}

#endif
