#include "sqlite3.h"
#include "database.h"
#include <filesystem>
#include <iostream>

/**
 * callback that idk wtf it does but it's required 
 */
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

//mandar esta mierda a una clase de bases de datos con metodos get schema, set schema
int getSchemaVersion(sqlite3* db, sqlite3_stmt* stmt)
{
    int rc;
    rc = sqlite3_prepare_v2(db, "PRAGMA user_version;", -1, &stmt, nullptr );
    rc = sqlite3_step(stmt);
    int schemaVersion {};
    if (rc == SQLITE_ROW)
    {
        schemaVersion = sqlite3_column_int(stmt, 0);
    }
    return schemaVersion;
}

int main()
{
    int rc;
    char *zErrMsg = 0;

    /*
    rc = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << " la base de datos no existe! Creando la base de datos, " << "\n";
        //crear base de datos
        rc = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc == SQLITE_OK)
        {
            std::cout << " base de datos creada " << rc << "\n";
        }
        

    }
    else
    {
        std::cout << "Opened database successfully" << "\n";

    }
    

    sqlite3_exec(db, "PRAGMA foreign_keys=ON;", callback, 0, &zErrMsg);
    sqlite3_exec(db, "PRAGMA user_version=1;", callback, 0, &zErrMsg);
    */

    SqlitoSeguro::Database db1;


    //int version {getSchemaVersion(db, stmt)};
    //std::cout << "Schema version: " << version << "\n";
    
    //sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);" , callback, 0, &zErrMsg);
    //sqlite3_exec(db, "INSERT INTO users (name, age) VALUES ('Alice', 25);", callback, 0, &zErrMsg);
    //sqlite3_exec(db, "SELECT * FROM users;", callback, 0, &zErrMsg);
    sqlite3_free(zErrMsg);
    //sqlite3_close(db);
    return 0;
}
