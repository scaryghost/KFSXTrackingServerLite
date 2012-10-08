#ifndef ETSAI_KFSXTSLITE_TIME_H
#define ETSAI_KFSXTSLITE_TIME_H

#include <stdexcept>
#include <string>

namespace etsai {
namespace kfsxtslite {

/**
 * Manipulates time from a D days HH:MM:SS representation
 * @author etsai
 */
class Time {
public:
    /**
     * Constructs object from the given number of seconds
     * @param   seconds     Number of seconds
     */
    Time(long seconds);
    /**
     * Constructs object from a C string in the format "D days HH:MM:SS"
     * @param   timeStr     String representation of the values
     * @throw   std::runtime_error  If the input does not match the specified format
     */
    Time(const char* timeStr) throw(std::runtime_error);
    /**
     * Constructs object from a string object in he format "D days HH:MM:SS"
     * @param   timeStr     String object representing the values
     * @throw   std::runtime_error  If the input does not match the specified format
     */
    Time(const std::string &timeStr) throw(std::runtime_error);

    /**
     * Add two Time objects together, storing the new value into the calling object
     * @param   offset  Values to add
     * @return  Reference to the calling object
     */
    Time& add(const Time& offset);

    /**
     * Get the number of days
     * @return days
     */
    int getDays() const { return days; }
    /**
     * Get the number of hours
     * @return hours
     */
    int getHours() const { return hours; }
    /**
     * Get the number of minutes
     * @return minutes
     */
    int getMinutes() const { return minutes; }
    /**
     * Get the number of seconds
     * @return seconds
     */
    int getSeconds() const { return seconds; }
    /**
     * Convert the object into a string
     * @return String representation of the object
     */
    std::string toString() const;
private:
    /**
     * Common initializer function shared by the string constructors
     * @param   timeStr     String representation of the values in form "D days HH:MM:SS"
     * @throw   std::runtime_error  If the input does not match the specified format
     */
    void initFromStr(const std::string &timeStr) throw(std::runtime_error);

    int days;       ///< Number of days
    int hours;      ///< Number of hours
    int minutes;    ///< Number of minutes
    int seconds;    ///< Number of seconds
    
};  //class Time

}   //namespace kfsxtslite
}   //namespace etsai

#endif //ETSAI_KFSXTSLITE_TIME_H
