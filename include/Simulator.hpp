#ifndef PUMA_Simulator_h
#define PUMA_Simulator_h

#include "helpers.hpp"
#include <fstream>
#include <boost/shared_array.hpp>
#include <time.h>

namespace PUMA {

    class Simulator {
        protected:
            /** Pointer to the current state of simulation.
             *  When referencing, take into account that it 
             *  **does not** point to the same place in memory
             *  at all times
             */
            boost::shared_array<landscape> current_state;

            /** Temprorary, internal state used when calculating
             *  the puma and hare densities
             */
            boost::shared_array<landscape> temp_state;

            double dt;
            /// X and Y sizes of the simulation area
            size_t size_x, size_y;

            /** A proxy function that enables easy implementation
             *  of boundary conditions
             */
            inline landscape* get_cell(int i, int j);

            /** A special 'empty cell' used as a nonexistent cell
             *  in the boundary conditions checker
             */
            landscape* halo_cell;

        public:
            // This should read in the data and throw an exception(s)
            // if the input data is misformed (ie. dim_x || dim_y == 0 etc)
            Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt);
            ~Simulator();

            /** These are the diffusion constants used in the differential equation
             * r = Birth rate of hares
             * a = Predation rate at which pumas ear hares
             * b = Birth rate of pumas per one hare eaten
             * m = Puma mortality rate
             * k = Diffusion rate for hares
             * l = Diffusion rate for pumas
             */
            double r, a, b, m, k, l;

            void apply_step();

            // After serializing puma and hare densities to relevant streams
            // both streams should be closed
            virtual void serialize(std::ofstream *output_hares, std::ofstream *output_pumas);
    };

    class TestSimulator : public Simulator {
        public:
            TestSimulator(size_t dim_x, size_t dim_y, bool *land_map, double dt) :
                Simulator(dim_x, dim_y, land_map, dt) {};

            boost::shared_array<landscape> get_current();
            boost::shared_array<landscape> get_temp();
    };
}

#endif
