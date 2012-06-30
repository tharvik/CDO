#include <stdio.h>
#include <sqlite3.h>
#include <iostream>

#include "db_local.hpp"
#include "../commun.hpp"

void db_local::callback_test ()
{
    std::cout << "asd" << std::endl;
}

// Constructeur: Check for existing DB and load it or create a new one
db_local::db_local()
{
    this->path = "Database.sqlite3.db";

    int returnCode = sqlite3_open_v2 (this->path.c_str(), &this->data, SQLITE_OPEN_READONLY, NULL);

    if (returnCode != SQLITE_OK)
    {
        this->createDB ();
    }

    else
    {
        bool errorState = this->exec ("SELECT version FROM preferences", this->callback_checkVersion);
    }

}

int db_local::callback_checkVersion (void* nothing, int parametre, char** value, char** colonneName)
{
    std::cout << value [0] << std::endl;


}

db_local::~db_local ()
{
    sqlite3_close (this->data);
}

// Create a new database file and fill it to a working state
bool db_local::createDB ()
{
    std::cout << "Creating a new databases... " << std::flush;

    // Create database file
    int returnCode = sqlite3_open_v2 (this->path.c_str(), &this->data, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (returnCode != SQLITE_OK)
    {
        printError (returnCode, sqlite3_errmsg (this->data));
        return false;
    }

    // Fill database
    bool errorState = this->exec ("CREATE TABLE album                                      \
                                   (                                                       \
                                       id        TEXT   ,                                  \
                                       name      TEXT   ,                                  \
                                       date      INTEGER,                                  \
                                       barcode   INTEGER,                                  \
                                       trackList TEXT   ,                                  \
                                       cover     BLOB                                      \
                                   );                                                      \
                                   CREATE INDEX index_name ON album (name);                \
                                   CREATE INDEX index_date ON album (date)                 \
                                  ");

         errorState = this->exec ("CREATE TABLE preferences                                \
                                   (                                                       \
                                       version   INTERGER                                  \
                                   )                                                       \
                                  ");

    errorState = errorState | this->optimize ();

    std::cout << "done!" << std::endl;

    return errorState;
}

// Wrapper around the wrapper (sqlite3_exec)
// Return 0 on no error; 1 on errors
bool db_local::exec (std::string SQLCommand, int (*callback) (void*,int,char**,char**))
{
    char * errorMessage;
    int returnCode = sqlite3_exec (this->data, SQLCommand.c_str (), callback, 0, &errorMessage);

    if (returnCode != SQLITE_OK)
    {
        printError   (returnCode, errorMessage);
        sqlite3_free (errorMessage);
        return 1;
    }

    return 0;
}

// Rebuild indexes, vacuum DB, ...
bool db_local::optimize ()
{
    bool errorState =              this->exec ("VACUUM               ");
    errorState      = errorState | this->exec ("ANALYZE              ");
    errorState      = errorState | this->exec ("ANALYZE sqlite_master");
    errorState      = errorState | this->exec ("REINDEX              ");

    return errorState;
}

