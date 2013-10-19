#ifndef PUMA_helpers_h
#define PUMA_helpers_h

#include <boost/random.hpp>
#include <ctime>

/** The basic stucture holding info about the landscape
 *  tile. If the element is_land, then pumas and hares can migrate
 *  to it and breed on it.
 */
struct landscape {
    double hare_density, puma_density;
    bool is_land;
};

double random(boost::mt19937 generator)
{
    static boost::uniform_01<boost::mt19937> dist(generator);
    return dist();
}

#endif
