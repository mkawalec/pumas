#include "Simulator.hpp"
#include "exceptions.hpp"
#include <iostream>


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
        
        current_state.reset(new landscape[dim_x*dim_y]);
        for (size_t i = 0; i < dim_x; ++i) {
            for (size_t j = 0; j < dim_y; ++j) {
                current_state[i+ dim_x * j].is_land = land_map[i + dim_x * j];

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

                if (current_state[i + dim_x * j].is_land) {
                    // The puma and hare densities are set for every field
                    // to be in range of 0 to 5
                    current_state[i + dim_x * j].hare_density = 
                                5.0 * (rand() / (double) RAND_MAX);
                    current_state[i + dim_x * j].puma_density = 
                                5.0 * (rand() / (double) RAND_MAX);
                } else {
                    current_state[i + dim_x * j].hare_density = 0;
                    current_state[i + dim_x * j].puma_density = 0;
                }
            }
        }
    }

    void Simulator::apply_step()
    {
    }

     void Simulator::serialize(std::ofstream *output_hares, std::ofstream *output_pumas)
    {
		output_hares.open ("output_hares.txt");
		output_hares << "# hare density for predator and prey" << endl;
		output_hares << "# i j h land=1" << endl;

		output_pumas.open ("output_pumas.txt");
		output_pumas << "# pumas density for predator and prey" << endl;
		output_pumas << "# i j p land=1" << endl;
		 for (size_t i = 0; i < dim_x; ++i){
            for (size_t j = 0; j < dim_y; ++j){
             output_hares << i << " " << j << " " << current_state[i + dim_x*j].hares_density << " " << current_state[i + dim_x*j].is_land << endl;
             output_pumas << i << " " << j << " " << current_state[i + dim_x*j].pumas_density << " " << current_state[i + dim_x*j].is_land << endl;
			}
		 }
		 output_hares.close();
		 output_pumas.close();
    }


}
