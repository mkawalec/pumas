#ifndef PUMA_helpers_h
#define PUMA_helpers_h

/** The basic stucture holding info about the landscape
 *  tile. If the element is_land, then pumas and hares can migrate
 *  to it and breed on it.
 */
struct landscape {
    double hare_density, puma_density;
    bool is_land;
};

<<<<<<< HEAD
struct average_densities{
    double hares, pumas;
};
=======
template <typename T>
void ignore(T &&) { }
>>>>>>> b137d7e64e874ed8064b287d6c499d4a9a7b4409

#endif
