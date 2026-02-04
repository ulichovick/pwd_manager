#include "sqlite3.h"
#include "database.h"
#include <filesystem>
#include <iostream>

//mandar esto como funcion miembro de una clase

int main()
{
    /*
    sqlite3_exec(db, "PRAGMA foreign_keys=ON;", callback, 0, &zErrMsg);
    sqlite3_exec(db, "PRAGMA user_version=1;", callback, 0, &zErrMsg);
    */

    try
    {
        const char* home {std::getenv("HOME")};
        std::filesystem::path directorio {std::filesystem::path(home) / ".local" / "share" / "pwd_manager" / "vault.db"};
        SqlitoSeguro::Database db1(directorio);

        db1.initialize();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    

    
    return 0;
}
