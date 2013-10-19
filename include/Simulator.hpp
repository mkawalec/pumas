#ifndef PUMA_Simulator_h
#define PUMA_Simulator_h

#include "helpers.hpp"
#include <fstream>
#include <boost/scoped_array.hpp>
#include <time.h>

namespace PUMA {

    class Simulator {
        protected:
        boost::scoped_array<landscape> current_state;
        boost::scoped_array<landscape> temp_state;

        double dt;
        size_t size_x, size_y;

        landscape* get_cell(int i, int j);

        public:
        // This should read in the data and throw an exception(s)
        // if the input data is misformed (ie. dim_x || dim_y == 0 etc)
        Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt);

        /** These are the diffusion constants used in the differential equation
         * r = Birth rate of hares
         * a = Predation rate at which pumas ear hares
         * b = Birth rate of pumas per one hare eaten
         * m = Puma mortality rate
         * k = Diffusion rate for hares
         * l = Diffusion rate for pumas
         */
        double r=0.08, a=0.04, b=0.02, m-0.06, k=0.2, l=0.2;

        void apply_step();

        // After serializing puma and hare densities to relevant streams
        // both streams should be closed
        void serialize(std::ofstream *output_hares, std::ofstream *output_pumas);
    };

}

#endif
