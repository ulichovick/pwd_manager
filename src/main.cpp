#include <filesystem>
#include <iostream>
#include "sqlite3.h"
#include "database.h"
#include "accounts.h"
#include "users.h"
#include "migracion.h"

namespace SqlitoSeguro
{
    struct session{
        int userId;
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
    try
    {
        const char* home {std::getenv("HOME")};
        std::filesystem::path directorio {std::filesystem::path(home) / ".local" / "share" / "pwd_manager" / "vault.db"};
        SqlitoSeguro::Database db1(directorio);
        
        SqlitoSeguro::migrationManager migrador(db1,2);
        migrador.initialize();

        SqlitoSeguro::session currSess;

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
            currSess.userId = usrs.Authenticate();
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
                accs.addAccount(servicio, nombre, contrasena, currSess.userId);
            }
            else
            {
                accs.listAccounts(currSess.userId);
                std::cout << "Introduzca el ID de la cuenta de la que se quiere ver mas detalles: " << "\n";
                std::cin >> elecc;
                currSess.accId = accs.detailAccount(currSess.userId, elecc);
                std::cout << "¿Desea realizar una acción relacionada a esta cue nta? (1=eliminar | 2=editar)" << "\n";
                std::cin >> elecc;
                if (elecc==1)
                {
                    std::cout << "¿está seguro que desea eliminar la cuenta?" << "\n";
                    std::cin >> elecc;
                    if (elecc=1)
                    {
                        accs.deleteAccount(currSess.userId , currSess.accId);
                    }
                }
                else if(elecc==2)
                {
                    std::string servicio;
                    std::cout << "introduzca el nombre del servicio donde se encuentra la cuenta: " << "\n";
                    std::cin >> servicio;
                    std::cout << "introduzca el nombre de usuario: " << "\n";
                    std::cin >> nombre;
                    std::cout << "introduzca la contraseña: " << "\n";
                    std::cin >> contrasena;
                    accs.editAccount(servicio, nombre, contrasena, currSess.userId , currSess.accId);
                }
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
