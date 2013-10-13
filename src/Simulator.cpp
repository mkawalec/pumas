#include "Simulator.hpp"

namespace PUMA {
// TODO: The inner parts of the simulator class should be defined here
	Simulator::Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt)
	{
		srand(time(NULL));
		try{
			if (dim_x <= 0) throw 1;
		}
		catch(int x){
			std::cout << "x-Dimension has to be larger than 0. ERROR: " << x << std::endl;
		}
		try{
			if (dim_y <= 0) throw 2;
		}
		catch(int x){
			std::cout << "y-Dimension has to be larger than 0. ERROR: " << x << std::endl;
		}
		try{
			if (dt <= 0) throw 3;
			this->dt = dt;
		} catch (int x){
			std::cout << "Time step dt has to be larger than 0. ERROR: "<< x << std::endl;
		}
		

		current_state.reset(new landscape[dim_x*dim_y]);
		for (int x_counter = 0; x_counter < dim_x; x_counter++){
			for (int y_counter = 0; y_counter < dim_y; y_counter++){
				double initial_hares_density = 5.0 * ((double) rand() / (double) RAND_MAX);
				double initial_pumas_density = 5.0 * ((double) rand() / (double) RAND_MAX);
				current_state[x_counter+ dim_x*y_counter].is_land = land_map[x_counter + dim_x*y_counter];

				// Random initial densities in the range from 0 to 5. Maybe we could overload the constructor st. we could parse initial densities as well.
				//Alternatively we give this constructor another variable that defines whether or not we use random initial densities and a pointer to doubles 
				// for hare densities and puma densities. 
				// I mean like this: Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt, bool wantRandom, double *haresDensities, double *pumasDensities)

				if (current_state[x_counter+ dim_x*y_counter].is_land) {
					current_state[x_counter+ dim_x*y_counter].hare_density = initial_hares_density;
					current_state[x_counter+ dim_x*y_counter].puma_density = initial_pumas_density;
				}
				else {
					current_state[x_counter+ dim_x*y_counter].hare_density = 0;
					current_state[x_counter+ dim_x*y_counter].puma_density = 0;
				}
			}
		}
		
		
	}




	void Simulator::apply_step(){

	}

	void Simulator::serialize(std::ofstream *output_hares, std::ofstream *output_pumas){

	}

}
