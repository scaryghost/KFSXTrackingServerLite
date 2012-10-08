#include "Time.h"

#include <cstdlib>
#include <sstream>
#ifndef WIN32
#include <regex.h>
#endif  //WIN32

namespace etsai {
namespace kfsxtslite {

using std::atoi;
using std::runtime_error;
using std::string;
using std::stringstream;

Time::Time(long seconds) {
    this->seconds= seconds % 60;
    minutes= (seconds / 60) % 60;
    hours= (seconds / 3600) % 24;
    days= (seconds / 3600) / 24;
}

Time::Time(const char* timeStr) throw(std::runtime_error) {
    initFromStr(timeStr);
}

Time::Time(const std::string &timeStr) throw(std::runtime_error) {
    initFromStr(timeStr);
}

void Time::initFromStr(const std::string &timeStr) throw(std::runtime_error) {
    //D days HH:MM:SS
    regex_t expr;
    regmatch_t matcher[5];
    int status;

    status= regcomp(&expr, "([0-9]+) days ([0-9]{2}):([0-9]{2}):([0-9]{2})", REG_EXTENDED);
    if (status) {
        throw runtime_error(__FUNCTION__ + string(" - Error compiling regular expression"));
    }
    status= regexec(&expr, timeStr.c_str(), sizeof(matcher), matcher, 0);
    if (!status) {
        days= atoi(timeStr.substr(matcher[1].rm_so, matcher[1].rm_eo - matcher[0].rm_so + 1).c_str());
        hours= atoi(timeStr.substr(matcher[2].rm_so, matcher[2].rm_eo - matcher[1].rm_so + 1).c_str());
        minutes= atoi(timeStr.substr(matcher[3].rm_so, matcher[3].rm_eo - matcher[2].rm_so + 1).c_str());
        seconds= atoi(timeStr.substr(matcher[4].rm_so, matcher[4].rm_eo - matcher[3].rm_so + 1).c_str());
        regfree(&expr);
    } else if (status == REG_NOMATCH) {
        regfree(&expr);
        throw runtime_error(__FUNCTION__ + string(" - No match for: ") + timeStr);
    } else {
        char errMsg[128];
        regerror(status, &expr, errMsg, sizeof(errMsg));
        regfree(&expr);
        throw runtime_error(string(__FUNCTION__ + string(" - Regex match failed: ") + errMsg));
    }
}


Time& Time::add(const Time& offset) {
    seconds+= offset.seconds;
    minutes+= (seconds / 60) + offset.minutes;
    seconds%= 60;
    hours+= (minutes / 60) + offset.hours;
    minutes%= 60;
    days+= (hours / 24) + offset.days;
    hours%= 24;

    return *this;
}

std::string Time::toString() const {
    stringstream ss(stringstream::out);
    
    ss << days << " days ";
    if (hours < 10) {
        ss << "0" << hours;
    } else {
        ss << hours;
    }
    ss << ":";
    if (minutes < 10) {
        ss << "0" << minutes;
    } else {
        ss << minutes;
    }
    ss << ":";
    if (seconds < 10) {
        ss << "0" << seconds;
    } else {
        ss << seconds;
    }
    
    return ss.str();
}

}
}
