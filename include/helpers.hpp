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

    /// \brief Structure containing RGB colors
    struct rgb {
        double r, g, b;
    };

    double get_time_micro_s();

    void format_time(double microsecs);

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
