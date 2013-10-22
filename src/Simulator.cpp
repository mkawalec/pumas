#include "Simulator.hpp"
#include "exceptions.hpp"
#include <iostream>

#include <sys/time.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>

namespace PUMA {

    Simulator::Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt) 
    {
        /** Using Mersenne-Twister as the random number generator
         *  as it has much better statistics that plain
         *  linear congruential bit that comes with gcc
         */
        timeval tv;
        gettimeofday(&tv, NULL);
        boost::mt19937 rng;
        boost::random::uniform_real_distribution<> random_data(0, 5);
        rng.seed(1000000 * tv.tv_sec + tv.tv_usec);
	
        // The value too small is also illegal, we want to filter
        // these out too
        if (dt < 1e-15) {
            throw IllegalValue("The step should be non-zero "
                    "and bigger than the accuracy of a double");
        }

        // Initializing the diffusion constants
        r = 0.08;
        a = 0.04; 
        b = 0.02;
        m = 0.06;
        k = 0.2; 
        l = 0.2;

        /** Allocating the memory for the halo cell, 
         *  so that we don't do a terrible amount of mallocks
         *  later on in the program
         */
        halo_cell = new landscape;
        halo_cell->puma_density = 0.0;
        halo_cell->hare_density = 0.0;
        halo_cell->is_land = false;

        current_state.reset(new landscape[dim_x * dim_y]);
        temp_state.reset(new landscape[dim_x * dim_y]);

        for (size_t i = 0; i < dim_y; ++i) {
            for (size_t j = 0; j < dim_x; ++j) {
                size_t index = i * dim_x + j;
                current_state[index].is_land = land_map[index];

                if (current_state[index].is_land) {
                    current_state[index].hare_density = random_data(rng);
                    current_state[index].puma_density = random_data(rng);
                } else {
                    current_state[index].hare_density = 0;
                    current_state[index].puma_density = 0;
                }
            }
        }
    }

    /** Removes the data structures that cannot
     *  destruct on their own
     */
    Simulator::~Simulator() 
    {
        free(halo_cell);
    }

    void Simulator::apply_step() 
    {
        temp_state.swap(current_state);

        for (int i = 0; (unsigned)i < size_y; ++i) {
            for (int j = 0; (unsigned)j < size_x; ++j) {
                size_t index = i * size_x + j;

                size_t nLand = get_cell(i + 1, j)->is_land
                    + get_cell(i - 1, j)->is_land
                    + get_cell(i, j + 1)->is_land
                    + get_cell(i, j - 1)->is_land;

                if (current_state[index].is_land) {
                    current_state[index].hare_density = get_cell(i,j)->hare_density
                        + dt * (r * get_cell(i,j)->hare_density
                                - a * get_cell(i,j)->hare_density * get_cell(i,j)->puma_density
                                + k * ((get_cell(i - 1, j)->hare_density + get_cell(i + 1, j)->hare_density
                                        + get_cell(i, j - 1)->hare_density + get_cell(i,j + 1)->hare_density)
                                    - nLand * get_cell(i,j)->hare_density));

                    current_state[index].puma_density = get_cell(i,j)->puma_density
                        + dt * (- m * get_cell(i,j)->puma_density
                                + b * get_cell(i,j)->puma_density * get_cell(i,j)->hare_density
                                + k * ((get_cell(i - 1, j)->puma_density + get_cell(i + 1, j)->puma_density
                                        + get_cell(i, j - 1)->puma_density + get_cell(i,j + 1)->puma_density)
                                    - nLand * get_cell(i,j)->puma_density));

                }
            }
        }
    }


    /** Returns a pointer to a cell at given coordinates.
     *  If there is no cell at the given coordinates (due to the
     *  boundary conditions), returns a pointer to a special 'empty cell',
     *  what acts as a water cell for all intents and purposes.
     */
    landscape* Simulator::get_cell(int i, int j)
    {
        if (i < 0 || (unsigned)i >= size_x || j < 0 || (unsigned)j >= size_y) 
            return halo_cell;
        else
            return &temp_state[i + size_x * j];
    }

    void Simulator::serialize(std::ofstream *output_hares, std::ofstream *output_pumas)
    {
        for (size_t i = 0; i < size_x; ++i) {
            for (size_t j = 0; j < size_y; ++j) {
                *output_hares << current_state[i + size_x*j].hare_density << " ";
                *output_pumas << current_state[i + size_x*j].puma_density << " ";
            }
            *output_hares << std::endl;
            *output_pumas << std::endl;
        }

        // Closing to make sure nothing corrupts the output
        output_hares->close();
        output_pumas->close();
    }

    boost::shared_array<landscape> TestSimulator::get_current()
    {
        return current_state;
    }

    boost::shared_array<landscape> TestSimulator::get_temp()
    {
        return temp_state;
    }
}
