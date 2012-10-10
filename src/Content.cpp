#include "Content.h"

#include <cstdlib>
#include <sstream>

namespace etsai {
namespace kfsxtslite {

using std::atoi;
using std::runtime_error;
using std::stringstream;

int Content::hashCode(const std::string& str) {
    int hash= 0;

    for(unsigned int i= 0; i < std.size(); i++) {
        code= (code << 5) - code + (int)str[i];
    }
    return code;
}

Content::Content(const std::string &dbPath) throw(std::runtime_error) {
    if (sqlite3_open(dbPath.c_str(), &db)) {
        throw runtime_error("Cannot open database: " + dbPath);
    }
}

Content::Content& addDiff(const std::string &name, const std::string &length, int wins, int losses, int wave, Time time) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int id= hashCode(name + "-" + length);
    auto diffTable= [&wins, &losses, &time](void *tableName, int argc, char **argv, char **colName) -> int {
        wins+= atoi(argv[3]);
        losses+= atoi(argv[4]);
        wave+= atoi(argv[5]);
        time.add(argv[6]);
    }
    select << "select * from difficulties where id=" << hashCode(name);
    sqlite3_exec(db, select.str().c_str(), callback, NULL, errMsg);

    upsert << "replace into difficulties (id, name, length, wins, losses, wave, time) values (" <<
        id << ", coalesce(( select name from difficulties where id=" << id << "),\'" << name << "\'), " << 
        "coalesce(( select length from difficulties where id=" << id << "),\'" << length << "\'), " <<
        wins << ", " << losses << ", " << wave << ", \'" << time.toString() << "\');";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);

}

Content::Content& addLevel(const std::string &name, int wins, int losses, const Time &time) {
}

Content::Content& addPlayer(const std::string &steamID64, const std::string &category, const std::unordered_map<std::string, int> &stats) {
}

Content::Content& addAggregate(const std::string &category, const std::string &stat, int value) {
}

Content::Content& addRecord(const std::string &steamID64, int wins, int losses, int disconnects) {
}

Content::Content& addDeaths(const std::string &name, int value) {
}

}
}
