#include "Simulator.hpp"

namespace PUMA {
// TODO: The inner parts of the simulator class should be defined here
    Simulator::Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt)
    {
        srand(112233);
        try {
            if (dim_x <= 0) throw 1;
        } catch(int x){
            std::cout << "x-Dimension has to be larger than 0. ERROR: " << x << std::endl;
        } 

        try {
            if (dim_y <= 0) throw 2;
        } catch(int x) {
            std::cout << "y-Dimension has to be larger than 0. ERROR: " << x << std::endl;
        }
        try {
            if (dt <= 0) throw 3;
            this->dt = dt;
        } catch (int x) {
            std::cout << "Time step dt has to be larger than 0. ERROR: "<< x << std::endl;
        }
        
        current_state.reset(new landscape[dim_x*dim_y]);
        for (size_t i = 0; i < dim_x; ++i){
            for (size_t j = 0; j < dim_y; ++j){
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
