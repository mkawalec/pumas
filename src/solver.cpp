#include "Serializer.hpp"
#include "Simulator.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    // Speeds up IO when there is a lot of IO to be done,
    // at the cost of making printf/scanf nonsafe
    std::ios_base::sync_with_stdio(0);

    if (argc < 4) {
        std::cout << "This program should be run as follows: " <<
            argv[0] << " input_filename timesteps_number dt" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1]);
    size_t total_timesteps = std::stoi(argv[2]);
    double dt = strtod(argv[3], NULL);

    size_t size_x, size_y;
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

    // Bind the current output method
    simulation.current_serializer = PUMA::Serializer::choose_output_method("vmd");

    std::ofstream output("output.xyz");
    // The main loop
    for (size_t i = 0; i < total_timesteps; ++i) {
        simulation.apply_step();
        simulation.serialize(&output, NULL);
    }

    return 0;
}
