#ifndef PUMA_helpers_h
#define PUMA_helpers_h

#include <utility>
#include <sys/time.h>
#include <iostream>

namespace PUMA {
    /// A convenience typedef
    typedef std::pair<double, double> average_densities;

    /** \brief The basic stucture holding information 
     *      about a landscape tile. 
     *
     *  If an element is_land, then pumas and hares can migrate
     *  to it and breed on it.
     */
    struct landscape {
        double hare_density, puma_density;
        bool is_land;
    };

    /** \brief Structure containing RGB colours
     *
     *  Useful structure when converting data to colours
     */
    struct rgb {
        double r, g, b;
    };

    /** \brief function that determines the current 
     *      system time in micro seconds
     */
    long get_time_micro_s();

    /** \brief formats a number of microseconds into a
     *      a more readable form
     *
     *  formats microseconds to "h:m:s:ms:us"
     *  \param microsecs the number of microseconds
     *      to be formatted
     */
    void format_time(long microsecs);

    /** \brief A function template accepting any
     *      parameter and doing nothing to it. 
     *
     *  Useful for explicit marking of a parameter as unused,
     *  both for helping a programmer and muting compiler
     *  warnings
     */
    template <typename T>
    void ignore(T &&) { }

}
#endif
