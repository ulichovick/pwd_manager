#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>
#include "database.h"

namespace SqlitoSeguro
{
    class accountManager
    {
        private:
            Database& db;
        public:
            accountManager(Database& database): db(database){};

            void addAccount(const std::string& service,
                            const std::string& login,
                            const std::string& password);
            void listAccounts();
    };
}

#endif
