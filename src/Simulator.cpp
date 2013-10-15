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
		for (int i = 0; i < dim_x; i++){
			for (int j = 0; j < dim_y; j++){

				current_state[i+ dim_x * j].is_land = land_map[i + dim_x * j];

				/** TODO: Maybe we could overload the constructor st. 
				*	we could parse initial densities as well.
				*	Alternatively we give this constructor another 
				*	variable that defines whether or not we use 
				*	random initial densities and a pointer to doubles 
				*	for hare densities and puma densities. 
				*	I mean like this: 
				*	Simulator(size_t dim_x, size_t dim_y, 
				*		bool *land_map, double dt, bool wantRandom, 
				*		double *haresDensities, double *pumasDensities)
				*/

				if (current_state[i + dim_x * j].is_land) {

					//Random initial densities in the range from 0 to 5.

					current_state[i + dim_x * j].hare_density = 
								5.0 * ((double) rand() / (double) RAND_MAX);
					current_state[i + dim_x * j].puma_density = 
								5.0 * ((double) rand() / (double) RAND_MAX);
				}
				else {
					current_state[i + dim_x * j].hare_density = 0;
					current_state[i + dim_x * j].puma_density = 0;
				}
			}
		}
	}

	void Simulator::apply_step(){

	}

	void Simulator::serialize(std::ofstream *output_hares, std::ofstream *output_pumas){

	}

}
