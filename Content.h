#include <sqlite3.h>
#include <stdexcept>
#include <unordered_map>

#include "Time.h"
#include "Utils.h"

namespace etsai {
namespace kfsxtslite {

/**
 * Manages the stats content, updating with received values and doing database queries and inserts
 * @author etsai
 */
class Content {
public:
    /**
     * Constructs a Content object, using the given database for persistence
     * @param   dbPath  Path to the database
     * @throws  std::runtime_error  If the database cannot be openned
     */
    Content(const std::string &dbPath) throw(std::runtime_error);
    /**
     * Free the memory used by the sqlite3 object
     */
    ~Content();


private:
    sqlite3 *db;            ///< Pointer to the sqlite database
    
    /**
     * Add values to the given difficulty name and length.  If said name length pairing is not present, 
     * a new one will be inserted into the database
     * @param   name    Difficulty name
     * @param   length  Difficulty length
     * @param   wins    Number of wins to add
     * @param   losses  Number of losses to add
     * @param   time    Time offset to add
     */
    Content& updateDiff(const std::string &name, const std::string &length, int wins, int losses, int wave, Time time);
    /**
     * Add values to the given level name.  If level name does not exist, a new one will be inserted
     * @param   name    Level name
     * @param   wins    Number of wins to add
     * @param   losses  Number of losses to add
     * @param   time    Tim offset to add
     */
    Content& updateLevel(const std::string &name, int wins, int losses, Time time);
    /**
     * Add values to the given player steamID64 and stat category.  If the steamID64/category pairing 
     * does not exist, a new entry will be inserted into the database
     * @param   steamID64   Player's steamID64, http://steamidconverter.com/
     * @param   category    Stat category the set of stats belong to
     * @param   stats       Set of stats and their new offsets to add
     */
    Content& updatePlayer(const std::string &steamID64, const std::string &category, std::unordered_map<std::string, int> stats);
    /**
     * Add value to the given stat in the specified category.  If the category/stat pairing 
     * does not exist, a new entry will be inserted
     * @param   category    Category the stat belongs too
     * @param   stat        Name of the stat
     * @param   value       Offset to add to the value
     */
    Content& updateAggregate(const std::string &category, const std::string &stat, long value);
    /**
     * Add record values to the specified steamID64.  If the steamID64 does not exist, it will be inserted
     * @param   steamID64   Player's steamID64
     * @param   wins        Number of wins to add
     * @param   losses      Number of losses to add
     * @param   disconnects Number of disconnects to add
     */
    Content& updateRecord(const std::string &steamID64, int wins, int losses, int disconnects);
    /**
     * Add tally to the given source of death.  If the death source does not exist, it will be inserted
     * @param   name    Name of the source of death
     * @param   value   Number of deaths caused by the source
     */
    Content& updateDeaths(const std::string &name, int value);

};  //class Content

}   //namespace kfsxtslite
}   //namespace etsai
