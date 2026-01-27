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

    SqlitoSeguro::Database db1;
    return 0;
}
