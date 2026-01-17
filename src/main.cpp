#include "sqlite3.h"
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
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;
    char *zErrMsg = 0;

    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        std::cerr << "cannot open the database:"<< sqlite3_errmsg(db) <<"  \n";
    }

    sqlite3_exec(db, "PRAGMA foreign_keys=ON;", callback, 0, &zErrMsg);
    sqlite3_exec(db, "PRAGMA user_version=1;", callback, 0, &zErrMsg);


    int version {getSchemaVersion(db, stmt)};
    std::cout << "Schema version: " << version << "\n";
    
    //sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);" , callback, 0, &zErrMsg);
    //sqlite3_exec(db, "INSERT INTO users (name, age) VALUES ('Alice', 25);", callback, 0, &zErrMsg);
    sqlite3_exec(db, "SELECT * FROM users;", callback, 0, &zErrMsg);
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
    return 0;
}
