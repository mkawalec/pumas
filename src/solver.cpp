#include "Simulator.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    int total_timesteps = 100;
    double dt = 0.1;
    std::ofstream hare_densities, puma_densities;
    size_t size_x, size_y;

    std::ifstream input("input.txt");
    input >> size_x >> size_y;
    bool *land_map = new bool[size_x*size_y];

    for (size_t j = 0; j < size_y; ++j) {
        for (size_t i = 0; i < size_x; ++i) {
            input >> land_map[i + size_x * j];
        }
    }
    input.close();

    PUMA::Simulator simulation(size_x, size_y, land_map, dt);
    delete[] land_map;

    for (size_t i = 0; i < total_timesteps; ++i) {
        simulation.apply_step();

        hare_densities.open("hare_hensities_" + std::to_string(i) + ".dat");
        puma_densities.open("puma_densities_" + std::to_string(i) + ".dat");
        simulation.serialize(&hare_densities, &puma_densities);
    }

    return 0;
}
