#include <iostream>
#include <sqlite3.h>

using std::cerr;
using std::cout;
using std::endl;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv) {
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;

    rc= sqlite3_open("kfsxdb.sqlite", &db);
    if (rc) {
        cout << "Error opening db" << endl;
        return -1;
    }

    rc = sqlite3_exec(db, "select * from difficulties", callback, 0, &zErrMsg);
    if (rc!=SQLITE_OK){
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    return 0;
}
