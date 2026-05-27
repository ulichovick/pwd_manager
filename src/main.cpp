#include <filesystem>
#include <iostream>
#include "sqlite3.h"
#include "database.h"
#include "accounts.h"
#include "users.h"
#include "migracion.h"
#include "login.h"
#include <optional>

namespace SqlitoSeguro
{
    struct session{
        std::optional<int> userId;
        int accId;

        bool isLogged() const
        {
            return  userId != 0;
        }

        void clear()
        {
            userId = 0;
            accId = 0;
        }
    };
} 


int main()
{
    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    try
    {
        const char* home {std::getenv("HOME")};
        std::filesystem::path directorio {std::filesystem::path(home) / ".local" / "share" / "pwd_manager" / "vault.db"};
        SqlitoSeguro::Database db1(directorio);
        
        SqlitoSeguro::migrationManager migrador(db1,2);
        migrador.initialize();

        SqlitoSeguro::session currSess;

        SqlitoSeguro::userManager usrs(db1);
        SqlitoSeguro::accountManager accs(db1);

        SqlitoSeguro::loginWindow loginWind(usrs, accs);
        loginWind.show();
        return Fl::run();

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    
}
