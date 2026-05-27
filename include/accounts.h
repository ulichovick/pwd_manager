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
                            const std::string& password, 
                            int usrId);
            std::map<int, std::vector<std::string>> listAccounts(int usrId);
            int detailAccount(int usrId, int accId);
            void deleteAccount(int usrId, int accId);
            void editAccount(const std::string& service,
                                            const std::string& login,
                                            const std::string& password,
                                            int usrId,
                                            int accId);
    };
}

#endif
