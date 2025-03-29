#include <iostream>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main()
{
    sqlite3 *db;
    //int rc;
    char *zErrMsg = 0;

    sqlite3_open("test.db", &db);

    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);" , callback, 0, &zErrMsg);
    sqlite3_exec(db, "INSERT INTO users (name, age) VALUES ('Alice', 25);", callback, 0, &zErrMsg);
    sqlite3_exec(db, "SELECT * FROM users;", callback, 0, &zErrMsg);
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
    return 0;
}
