#ifndef PUMA_helpers_h
#define PUMA_helpers_h

namespace PUMA {
    /** The basic stucture holding info about the landscape
     *  tile. If the element is_land, then pumas and hares can migrate
     *  to it and breed on it.
     */
    struct landscape {
        double hare_density, puma_density;
        bool is_land;
    };

    struct average_densities { 
        double hares, pumas;
    };

    struct rgb {
        double r, g, b;
    };

    template <typename T>
        void ignore(T &&) { }

}
#endif
