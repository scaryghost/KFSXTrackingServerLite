#include "Utils.h"


namespace etsai {
namespace kfsxtslite {

using std::string;
using std::vector;

int Utils::hashCode(const std::string &str) {
    int hash= 0;

    for(unsigned int i= 0; i < str.size(); i++) {
        code= (code << 5) - code + (int)str[i];
    }
    return code;
}

std::vector<std::string> Utils::split(const std::string &str, char separator) {
    vector<string> elems;
    unsigned int i= 0;

    while(i < str.size()) {
        int loc= str.find(separator, i);

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
