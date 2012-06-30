#ifndef DB_LOCAL_HPP
#define DB_LOCAL_HPP

#include <string>
#include <sqlite3.h>

#include "../CD.hpp"

class db_local
{
    public:
        db_local      ();
        ~db_local     ();
        bool optimize ();

    private:
        sqlite3*    data;
        std::string path;

        bool        createDB ();
        bool        exec     (std::string SQLCommand, int (*callback) (void*,int,char**,char**) = NULL);
        bool        exec     (std::string SQLCommand, void (*callback) ());


        // Callback
        static int         callback_checkVersion (void* nothing, int parametre, char** value, char** colonneName);
        static void        callback_test ();


};

#endif
