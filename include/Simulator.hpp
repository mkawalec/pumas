#ifndef PUMAS_Simulator_h
#define PUMAS_Simulator_h

#include "helpers.hpp"
#include <fstream>

class Simulator
{
    private:
        landscape *current_state;
        landscape *temp_state;
        double dt;

    public:
        // This should read in the data and throw an exception(s)
        // if the input data is misformed (ie. dim_x || dim_y == 0 etc)
        Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt){}

        void apply_step(){}

        // After serializing puma and hare densities to relevant streams
        // both streams should be closed
        void serialize(std::ofstream *output_hares, std::ofstream *output_pumas){}
};

#endif
