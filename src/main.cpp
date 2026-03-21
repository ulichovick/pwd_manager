#include <filesystem>
#include <iostream>
#include "sqlite3.h"
#include "database.h"
#include "accounts.h"
#include "users.h"
#include "migracion.h"

int main()
{
    try
    {
        const char* home {std::getenv("HOME")};
        std::filesystem::path directorio {std::filesystem::path(home) / ".local" / "share" / "pwd_manager" / "vault.db"};
        SqlitoSeguro::Database db1(directorio);
        
        SqlitoSeguro::migrationManager migrador(db1,2);
        migrador.initialize();

        std::string nombre;
        std::string contrasena;
        int elecc;
        std::cout << "¿Desea crear un usuario o iniciar sesión? (1=crear usuario | 2=iniciar sesión)" << "\n";
        std::cin >> elecc;

        std::cout << "introduzca el nombre de usuario: " << "\n";
        std::cin >> nombre;
        std::cout << "introduzca la contraseña: " << "\n";
        std::cin >> contrasena;
        SqlitoSeguro::userManager usrs(db1, nombre, contrasena);
        SqlitoSeguro::accountManager accs(db1);

        if (elecc==1)
        {
            usrs.createUser();
        }
        else if (elecc=2)
        {
            usrs.Authenticate();
            std::cout << "¿Desea crear una cuenta nueva o listas las cuentas asociadas a este usuario? (1=Crear | 2=Listar)" << "\n";
            std::cin >> elecc;
            if (elecc==1)
            {
                std::string servicio;
                std::cout << "introduzca el nombre del servicio donde se encuentra la cuenta: " << "\n";
                std::cin >> servicio;
                std::cout << "introduzca el nombre de usuario: " << "\n";
                std::cin >> nombre;
                std::cout << "introduzca la contraseña: " << "\n";
                std::cin >> contrasena;
                accs.addAccount(servicio, nombre, contrasena);
            }
            else
            {
                accs.listAccounts();
            }
            
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
