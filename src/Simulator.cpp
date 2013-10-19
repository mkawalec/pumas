#include "Simulator.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace PUMA {

Simulator::Simulator(size_t dim_x, size_t dim_y, bool *land_map, double dt) {
	// We want to have a predictable random seed
	srand(112233);

	// The value too small is also illegal, we want to filter
	// these out too
	if (dt < 1e-15) {
		throw IllegalValue("The step should be non-zero "
				"and bigger than the accuracy of a double");
	}

	current_state.reset(new landscape[dim_x * dim_y]);
	for (size_t i = 0; i < dim_x; ++i) {
		for (size_t j = 0; j < dim_y; ++j) {
			current_state[i + dim_x * j].is_land = land_map[i + dim_x * j];

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

void Simulator::apply_step() {

		temp_state.swap(current_state);
		int nLand;

		for (size_t i = 0; i < dim_x; ++i) {
			for (size_t j = 0; j < dim_y; ++j) {

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
				int nDesnsiy;
				nLand = temp.state[i +1 +dim_x*j].island
						+ temp.state[i-1 +dim_x*j].island
						+ temp.state[i +dim_x*j].island
						+ temp.state[i +dim_x*j].island;

				if (current_state[i + dim_x * j].is_land) {
					current_state[i + dim_x * j].hare_density = temp_state[i + dim_x * j].hare_density
					+ size_t*(r*temp_state[i + dim_x * j].hare_density
							- a*temp_state[i + dim_x * j].hare_density*temp_state[i + dim_x * j].puma_density
							+ k*((checkHareTemp(i-1,j) + checkHareTemp(i+1,j)
											+ checkHareTemp(i,j-1) + checkHareTemp(i,j+1))
									- nLand*temp_state[i + dim_x j].hare_density));

					current_state[i + dim_x * j].puma_density = 5.0
							* (rand() / (double) RAND_MAX);
				}
			}
		}
}


void Simulator::checkHareTemp(int i, int j){
	int limit = size_x * ((i+1)/size_X) - i+1;
	if (limit == 0 || limit == 1) return 0;
	else return temp_state.[i + size_x*j].hare_density;
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
