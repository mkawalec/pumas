#include "Simulator.hpp"
#include "exceptions.hpp"
#include <iostream>

#include <boost/scoped_ptr.hpp>

namespace PUMA {
    Simulator::Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt) 
    {
        // We want to have a predictable random seed
        srand(112233);

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

        for (size_t i = 0; i < dim_x; ++i) {
            for (size_t j = 0; j < dim_y; ++j) {
                current_state[i + dim_x * j].is_land = land_map[i + dim_x * j];

                if (current_state[i + dim_x * j].is_land) {
                    // The puma and hare densities are set for every field
                    // to be in range of 0 to 5
                    current_state[i + dim_x * j].hare_density = 5.0
                        * (rand() / (double) RAND_MAX);
                    current_state[i + dim_x * j].puma_density = 5.0
                        * (rand() / (double) RAND_MAX);
                } else {
                    current_state[i + dim_x * j].hare_density = 0;
                    current_state[i + dim_x * j].puma_density = 0;
                }
            }
        }
    }

    void Simulator::apply_step() 
    {
        temp_state.swap(current_state);

        for (size_t i = 0; i < size_x; ++i) {
            for (size_t j = 0; j < size_y; ++j) {

                /** TODO: Maybe we could overload the constructor st.
                 *   we could parse initial densities as well.
                 *   Alternatively we give this constructor another
                 *   variable that defines whether or not we use
                 *   random initial densities and a pointer to doubles
                 *   for hare densities and puma densities.
                 *   I mean like this:
                 *   Simulator(size_t dim_x, size_t dim_y,
                 *       bool *land_map, double dt, bool wantRandom,
                 *       double *haresDensities, double *pumasDensities)
                 */

                size_t nLand = get_cell(i + 1, j)->is_land
                    + get_cell(i - 1, j)->is_land
                    + get_cell(i, j + 1)->is_land
                    + get_cell(i, j - 1)->is_land;

                if (current_state[i + dim_x * j].is_land) {
                    current_state[i + dim_x * j].hare_density = get_cell(i,j)->hare_density
                        + dt * (r * get_cell(i,j)->hare_density
                                - a * get_cell(i,j)->hare_density * get_cell(i,j)->puma_density
                                + k * ((get_cell(i - 1, j)->hare_density + get_cell(i + 1, j)->hare_density
                                        + get_cell(i, j - 1)->hare_density + get_cell(i,j + 1)->hare_density)
                                    - nLand * get_cell(i,j)->hare_density));

                }
            }
        }
    }

    landscape* Simulator::get_cell(int i, int j)
    {
        if (i < 0 || i >= size_x || j < 0 || j >= size_y) 
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
}
