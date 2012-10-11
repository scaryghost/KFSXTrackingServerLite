#include "Utils.h"


namespace etsai {
namespace kfsxtslite {

using std::string;
using std::vector;

int Utils::hashCode(const std::string &str) {
    int hash= 0;

    for(size_t i= 0; i < str.size(); i++) {
        hash= (hash << 5) - hash + (int)str[i];
    }
    return hash;
}

std::vector<std::string> Utils::split(const std::string &str, char separator) {
    vector<string> elems;
    size_t i= 0;
    size_t loc= 0;

    while(loc >= 0 && i < str.size()) {
        loc= str.find(separator, i);
        if (loc != i) {
            elems.push_back(str.substr(i, (loc - i)));
        }
        i= loc + 1;
    }
    return elems;
}

std::string Utils::join(const std::vector<std::string> elems, char separator) {
    string joinedStr;

    for(auto it= elems.begin(); it != elems.end(); it++) {
        if (it != elems.begin()) {
            joinedStr+= separator;
        }
        joinedStr+= (*it);
    }
    return joinedStr;
}

}
}
