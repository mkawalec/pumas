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
     *  The Simulation takes into account birth rates, predation rates,
     *  mortality rates of both predators and prey as well as 
     *  migration rates from cell to cell.
     */
    class Simulator {
    protected:
        /** \brief Pointer to the current state of simulation.
         *
         *  \warning When referencing, take into account that it 
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

        /** \brief A proxy function enabling easy implementation
         *      of boundary conditions
         *  \param x X coordinate of requested cell
         *  \param y Y coordinate of requested cell
         *  \return pointer to a cell at these coordinates
         *
         *  If there is no cell at given coordinates it retuns
         *  a pointer to a special empty cell that acts
         *  as a water cell for all intents and purposes.
         */
        virtual landscape* get_cell(int x, int y);

        /** A special 'empty cell' used as a nonexistent cell
         *  in the boundary conditions checker
         */
        landscape* halo_cell;

    public:
        /** \brief initializes a simulation instance with some
         *      input data
         *  \param dim_x size in X dimension of the supplied land_map
         *  \param dim_y size in Y dimension of land_map
         *  \param land_map a 1D array of X*Y elements specifying
         *      which tiles are land and which water
         */
        Simulator(size_t dim_x, size_t dim_y, bool *land_map);
        ~Simulator();

        /// Birth rate of hares
        double r;
        /// Predation rate at which pumas eat hares
        double  a;
        /// birth rate of pumas per one hare eaten
        double b;
        /// puma mortality rate
        double m;
        /// Diffusion rate for hares
        double k;
        /// Diffusion ratefor pumas
        double l;

        /** \brief Calculates the average densities of pumas/hares
         *  over all land cells of the current state. 
         *
         *  \return a pair of numbers, first of which is a hare 
         *      density, the second puma density
         */
        average_densities get_averages();

        /** \brief Applies the next time step to the Simulation.
         *
         *  Based on a discretization of two coupled 
         *  partial differential equations linking pumas and hares 
         *  in a predator prey model that includes diffusion over some 
         *  landscape. 
         */
        virtual void apply_step();

        /** If set its value is used as a pointer to currently
         *  used serializer class
         */
        Serializer* current_serializer;

        /** \brief dispatches serialization to one of
         *      serializers available at runtime
         *  \param main_output main output stream
         *  \param aux_output some output methods require a
         *      second output stream
         */
        virtual void serialize(std::ofstream *main_output, std::ofstream *aux_output=NULL);
    };

    /** \brief Used for tests to provide a more
     *      in-depth access to Simulator internals
     */
    class TestSimulator : public Simulator {
    protected:
        double dt;
    public:
        TestSimulator(size_t dim_x, size_t dim_y, bool *land_map, double dt) :
            Simulator(dim_x, dim_y, land_map), dt(dt) {};

        /** \brief allows manually setting the densities of pumas
         *  and hares on a rectangular grid of points
         *  \param hare value that the hare density
         *      should be set to
         *  \param puma value that the puma density 
         *      should be set to
         *  \param x_dim x dimension of the rectangle 
         *      in which this change is to be applied
         *  \param y_dim y dimension of the rectangle 
         *      in which this change is to be applied
         */
        void set_densities_const(double hare, double puma, size_t x_dim, size_t y_dim);

        /** \brief provides access to the current state
         *  \return a pointer to current state
         *
         *  \warning the pointer becomes invalid after
         *      apply_step is ran!
         */
        boost::shared_array<landscape> get_current();

        /** \brief provides access to the temprorary
         *      simulation state
         *  \return a pointer to temprorary state
         *
         *  \warning the pointer becomes invalid after
         *      apply_step is ran!
         */
        boost::shared_array<landscape> get_temp();
    };
}

#endif
