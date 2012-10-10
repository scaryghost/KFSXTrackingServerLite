#include "Content.h"

#include <cstdlib>
#include <sstream>

namespace etsai {
namespace kfsxtslite {

using std::atoi;
using std::atol;
using std::runtime_error;
using std::stringstream;

Content::Content(const std::string &dbPath) throw(std::runtime_error) {
    if (sqlite3_open(dbPath.c_str(), &db)) {
        throw runtime_error("Cannot open database: " + dbPath);
    }
}

Content::~Content() {
    sqlite3_close(db);
}

Content::Content& updateDiff(const std::string &name, const std::string &length, int wins, int losses, int wave, Time time) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int id= hashCode(name + "-" + length);
    auto diffTable= [&wins, &losses, &time](void *tableName, int argc, char **argv, char **colName) -> int {
        wins+= atoi(argv[3]);
        losses+= atoi(argv[4]);
        wave+= atoi(argv[5]);
        time.add(argv[6]);
    };

    select << "select * from difficulties where id=" << id;
    sqlite3_exec(db, select.str().c_str(), diffTable, NULL, &errMsg);

    upsert << "replace into difficulties (id, name, length, wins, losses, wave, time) values (" <<
        id << ", coalesce(( select name from difficulties where id=" << id << "),\'" << name << "\'), " << 
        "coalesce(( select length from difficulties where id=" << id << "),\'" << length << "\'), " <<
        wins << ", " << losses << ", " << wave << ", \'" << time.toString() << "\');";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);

    return *this;
}

Content::Content& updateLevel(const std::string &name, int wins, int losses, Time time) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int  id= hashCode(name);
    auto levelTable= [&wins, &losses, &time](void *tableName, int argc, char **argv, char **colName) -> int {
        wins+= atoi(argv[2]);
        losses+= atoi(argv[3]);
        time.add(argv[4]);
    };
    
    select << "select * from levels where id=" << id;
    sqlite3_exec(db, select.str().c_str(), levelTable, NULL, &errMsg);

    upsert << "replace into levels (id, name, wins, losses, time) values (" <<
        id << ", coalesce(( select name from levels where id=" << id << "),\'" << name << "\'), " <<
        wins << ", " << losses << ", \'" << time.toString() << "\');";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);

    return *this;
}

Content::Content& updatePlayer(const std::string &steamID64, const std::string &category, std::unordered_map<std::string, int> stats) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int id= hashCode(steamID64 + "-" + category);
    vector<string> newStats(stats.size());
    auto playerTable= [&stats](void *tableName, int argc, char **argv, char **colName) -> int {
        vector<string> statPairs= Utils::split(argv[3], ',');

        for(auto it= statPair.begin(); it != statPair.end(); it++) {
            vector<string> keyval= split(*it, '=');
            if (stats.count(keyval[0]) == 0) {
                stats[keyval[0]]= atoi(keyval[1]);
            } else {
                stats[keyval[0]]+= atoi(keyval[1]);
            }
        }
    };

    select << "select * from player where id=" << id;
    sqlite3_exec(db, select.str().c_str(), playerTable, NULL, &errMsg);

    for(auto it= stats.begin(); it != stats.end(); it++) {
        stringstream keyval(stringstream::out);
        keyval << it->first << "=" << it->second;
        newStats.push_back(keyval.str());
    }
    upsert << "replace into player (id, steamid, stats, category) values (" << 
        id << ", coalesce(( select steamid from player where id=" << id << "),\'" << steamID64 << "\'), \'" << 
        Utils::join(newStats) << "\', coalesce(( select category from player where id=" << id << "),\'" << category << "\'));";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);

    return *this;
}

Content::Content& updateAggregate(const std::string &category, const std::string &stat, long value) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int id= hashCode(stat + "-" + category);
    auto aggregateTable= [&value](void *tableName, int argc, char **argv, char **colName) -> int {
        value+= atol(argv[2]);
    };

    select << "select * from aggregate where id=" << id;
    sqlite3_exec(db, select.str().c_str(), aggregateTable, NULL, &errMsg);

    upsert << "replace into aggregate (id, stat, value, category) values (" << 
        id << ", coalesce(( select stat from aggregate where id=" << id << "),\'" << stat << "\'), \'" << value << 
        "\', coalesce(( select category from aggregate where id=" << id << "),\'" << category << "\;));";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);

    return *this;
}

Content::Content& updateRecord(const std::string &steamID64, int wins, int losses, int disconnects) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int id= hashCode(steamID64);
    auto recordTable= [&wins, &losses, &disconnects](void *tableName, int argc, char **argv, char **colName) -> int {
        wins+= atoi(argv[2]);
        losses+= atoi(argv[3]);
        disconnects+= atoi(argv[4]);
    }

    select << "select * from records where id=" << id;
    sqlite3_exec(db, select.str().c_str(), recordTable, NULL, &errMsg);

    upsert << "replace into records (id, steamid, wins, losses, disconnects) values (" << 
        id << ", coalesce(( select steamid from records where id=" << id << "),\'" << steamID64 << "\'), " << 
        wins << ", " << losses << ", " << disconnects << ");";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);

    return *this;
}

Content::Content& updateDeaths(const std::string &name, int value) {
    char errMsg[128];
    stringstream select(stringstream::out), upsert(stringstream::out);
    int id= hashCode(name);
    auto deathTable= [&value](void *tableName, int argc, char **argv, char **colName) -> int {
        value+= atoi(argv[2]);
    };

    select << "select * from deaths where id=" << id;
    sqlite3_exec(db, select.str().c_str(), deathTable, NULL, &errMsg);

    upsert << "replace into deaths (id, name, count) values (" << id << 
        ", coalesce(( select name from deaths where id=" << id << "),\'" << name << "\'), " << value << ");";
    sqlite3_exec(db, upsert.str().c_str(), NULL, NULL, errMsg);
    
    return *this;
}

}
}
