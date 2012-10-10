#ifndef ETSAI_KFSXTSLITE_UTILS_H
#define ETSAI_KFSXTSLITE_UTILS_H

#include <string>
#include <vector>

namespace etsai {
namespace kfsxtslite {

/**
 * Utility functions used by the project
 * @author etsai
 */
class Utils {
public:
    /**
     * Generates a hash code for the given string.  The algorithm is the one used by Java for 
     * creating a hash code from a String object
     * @param   str     Input string
     * @return  Hash code of the input string
     */
    static int hashCode(const std::string &str);
    /**
     * Split a string around the given separator
     * @param   str     String to split apart
     * @param   separator   Character to split around
     * @return  List of strings around the separtor
     */
    static std::vector<std::string> split(const std::string &str, char separator);
    /**
     * Join a vector of strings into 1 string, separated by the given character
     * @param   elems   Strings to merge into one
     * @param   separator   Character to insert in between each string
     * @return  Merged string with the separator between the individual strings
     */
    static std::string join(const std::vector<std::string> elems, char separator);
};  //class Utils

}   //namespace kfsxtslite
}   //namespace etsai

#endif  //ETSAI_KFSXTSLITE_UTILS_H
