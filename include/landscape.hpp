#ifndef PUMAS_landscape_h
#define PUMAS_landscape_h

/** The basic stucture holding info about the landscape
 *  tile. If the element is_land, then pumas and hares can migrate
 *  to it and breed on it.
 */
struct landscape {
    double hare_density, puma_density;
    bool is_land;

};

#endif
