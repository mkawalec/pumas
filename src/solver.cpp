#include "Simulator.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	// TODO: initial onditions should be defined in a file which needs to be read in

	int total_timesteps = 100;
    std::ifstream input;
    std::ofstream* phare_densities = new std::ofstream;;
	std::ofstream* ppuma_densities = new std::ofstream;;
    size_t size_x;
	size_t size_y;
	double dt = 0.1;
	
    //change name to whatever is needed
	// TODO: I think we need to cahnge the for loops in the definitions of the methods -c
    input.open ("input.txt"); 
    input >> size_x >> size_y;
    bool * land_map = new bool[size_x*size_y];
	for (size_t j = 0; j < size_y; ++j) {
        for (size_t i = 0; i < size_x; ++i) {
	        input >> land_map[i+size_x*j];
		}
	}
	input.close();

	
	PUMA::Simulator simulation(size_x, size_y,land_map,dt);
	delete [] land_map;

	for (int i = 0; i < total_timesteps; ++i) {

	    simulation.apply_step();
	    phare_densities->open("Hare_Densities.txt");
	    ppuma_densities->open("Puma_Densities.txt");
	    simulation.serialize(phare_densities,ppuma_densities);

	}

	simulation.~Simulator();
	delete phare_densities;
	delete ppuma_densities;
    
    return 0;
}
