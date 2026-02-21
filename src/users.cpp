#include <iostream>
#include "database.h"
#include "users.h"



void SqlitoSeguro::userManager::createUser(const std::string& username,
                                            const std::string& password)
{
    std::string query {"INSERT INTO USERS (username, password, created_at) VALUES (?, ?, ?);"};
    std::unordered_map<int, std::string> values {
        {1, username},
        {2, password}
    };
    db.executeQuery(query, values);
}
