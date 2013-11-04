#ifndef PUMA_Simulator_hpp
#define PUMA_Simulator_hpp

#include "helpers.hpp"
#include "Serializer.hpp"
#include <fstream>
#include <boost/shared_array.hpp>
#include <time.h>

namespace PUMA {

    /** \brief Predator-Prey simulation
     *
     *  The Simulator class simulates a 
     *  Predator (pumas) - Prey (hares) system on a landmap
     *  that consists of discrete land and water cells.
     *  The Simulation takes into account birth rates, predation rates
     *  mortality rates of both predators and prey as well as 
     *  migration rates from cell to cell.
     *
     */

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
        Simulator(size_t dim_x, size_t dim_y, bool *land_map);
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

        /** \brief Calculates the average densities of pumas/hares
         *  over all land cells of the current state. 
         */
        average_densities get_averages();

        /** \brief Applies the next time step to the Simulation
         *  which is based on a discretization of two coupled 
         *  partial differential equations linking pumas and hares 
         *  in a predator prey model that includes diffusion over some 
         *  landscape. 
         */
        virtual void apply_step();
        Serializer* current_serializer;

        virtual void serialize(std::ofstream *output_hares, std::ofstream *output_pumas);
    };

    class TestSimulator : public Simulator {
    protected:
        double dt;
    public:
        TestSimulator(size_t dim_x, size_t dim_y, bool *land_map, double dt) :
            Simulator(dim_x, dim_y, land_map), dt(dt) {};

        void set_densities_const(double hare, double puma, size_t x_dim, size_t y_dim);
        boost::shared_array<landscape> get_current();
        boost::shared_array<landscape> get_temp();
    };
}

#endif
