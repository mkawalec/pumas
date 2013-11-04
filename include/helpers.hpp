#ifndef PUMA_helpers_h
#define PUMA_helpers_h

namespace PUMA {
    /** The basic stucture holding information about a landscape
     *  tile. If an element is_land, then pumas and hares can migrate
     *  to it and breed on it.
     */
    struct landscape {
        double hare_density, puma_density;
        bool is_land;
    };

    struct average_densities { 
        double hares, pumas;
    };

    /** Structure containing RGB colors
     */
    struct rgb {
        double r, g, b;
    };

    /** A function template accepting any
     *  parameter and doing nothing to it. Useful
     *  for explicit marking of a parameter as unused,
     *  both for helping a programmer and muting compiler
     *  warnings
     */
    template <typename T>
    void ignore(T &&) { }

}
#endif
