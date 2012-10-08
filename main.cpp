#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <sqlite3.h>

#include "Stoughton1004Lib/CLI/cli.h"
#include "Stoughton1004Lib/Logging/logging.h"
#include "Stoughton1004Lib/Network/DatagramPacket.h"
#include "Stoughton1004Lib/Network/DatagramSocket.h"

using namespace Stoughton1004Lib;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

Logger *logger;
ConsoleHandler consoleH;
FileHandler fileH;

void start(sqlite3 *db, int udpPort);
int dbCallback(void *tableName, int argc, char **argv, char **azColName);

int main(int argc, char **argv) {
    string dbPath;
    int udpPort= 6000;
    CLIBuilder *cli= CLIBuilder::getBuilder();

    cli->setUsage("kfsxtslite [options]");
    cli->addOption(Option("-port", 0, 1, [&udpPort](const Arguments &args) -> void {
        udpPort= args.asInteger(0);
    }).withDescription("UDP port to listen for packets").withArgName("no"))
    .addOption(Option("-dbpath", 0, 1, [&dbPath](const Arguments &args) -> void {
        dbPath= args.asString(0);
    }).withDescription("Sqlite database to use").withArgName("path").withRequired(true))
    .addOption(Option("-h", [&cli](const Arguments &args) -> void {
        cli->displayUsage();
    }));
    cli->parse(argc, argv);

    logger= Logger::getLogger("KFSXTrackingServerLite");
    logger->addHandler(&consoleH);
    logger->addHandler(&fileH);

    sqlite3 *db;
    int rc;
    rc= sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        logger->log(Level::SEVERE, "Cannot open database: " + dbPath);
        return -1;
    }

    start(db, udpPort);

/**
    char *zErrMsg;
    rc= sqlite3_exec(db, "select * from difficulties", callback, NULL, &zErrMsg);
    if (rc!=SQLITE_OK) {
        logger->log(Level::SEVERE, string("SQL error: ") + zErrMsg);
        sqlite3_free(zErrMsg);
    }
*/
    sqlite3_close(db);
    return 0;
}

int dbCallback(void *tableName, int argc, char **argv, char **azColName) {
    for(int i=0; i<argc; i++){
        logger->log(Level::INFO, string(azColName[i]) + " = " + (argv[i] ? argv[i] : "NULL"));
    }
    return 0;
}

void start(sqlite3 *db, int udpPort) {
    stringstream info(stringstream::out);
    DatagramSocket socket;
    DatagramPacket packet(INT_MAX);

    try {
        socket.bind(udpPort);
        info << "Starting kfsxtslite server on UDP port: " << udpPort;
        logger->log(Level::INFO, info.str());
        while(true) {
            socket.receive(packet);
            logger->log(Level::INFO, "Received: " + packet.getData());
        }
    } catch (S1004LibException &ex) {
        logger->log(Level::SEVERE, ex.what());
    }
}
