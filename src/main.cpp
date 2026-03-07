#include <filesystem>
#include <iostream>
#include "sqlite3.h"
#include "database.h"
#include "accounts.h"
#include "users.h"

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

        SqlitoSeguro::userManager usrs(db1);
        std::string nombre;
        std::string contrasena;
        int elecc;
        std::cout << "¿Desea crear un usuario o iniciar sesión? (1=crear usuario | 2=iniciar sesión)" << "\n";
        std::cin >> elecc;
        if (elecc==1)
        {
            std::cout << "introduzca el nombre de usuario a crear: " << "\n";
            std::cin >> nombre;
            std::cout << "introduzca la contraseña: " << "\n";
            std::cin >> contrasena;
            usrs.createUser(nombre, contrasena);
        }
        else if (elecc=2)
        {
            std::cout << "introduzca el nombre de usuario: " << "\n";
            std::cin >> nombre;
            std::cout << "introduzca la contraseña: " << "\n";
            std::cin >> contrasena;
            usrs.Authenticate(nombre, contrasena);
        }
        else
        {
            std::cout << "selección no valida!, terminando el programa..." << "\n";
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    

    
    return 0;
}
