#include "Serializer.hpp"
#include "Simulator.hpp"

#include <fstream>
#include <iostream>

PUMA::Simulator* initialize(std::ifstream *input, double dt, size_t oversampling)
{
    size_t size_x, size_y;
    *input >> size_x >> size_y;

    bool *land_map = new bool[size_x*size_y];
    for (size_t j = 0; j < size_y; ++j) {
        for (size_t i = 0; i < size_x; ++i) {
            *input >> land_map[i + size_x * j];
        }
    }
    input->close();

    PUMA::Simulator *simulation = 
        new PUMA::Simulator(size_x, size_y, land_map, dt / oversampling);
    delete[] land_map;

    return simulation;
}


int main(int argc, char *argv[])
{
    // Speeds up IO when there is a lot of IO to be done,
    // at the cost of making printf/scanf nonsafe
    std::ios_base::sync_with_stdio(0);

    
    if (argc < 5) {
        std::cout << "This program should be run as follows: " <<
            argv[0] << " input_filename output_filename timesteps_number dt" << std::endl;
        return -1;
    }

    std::string filename(argv[2]);
    size_t total_timesteps = std::stoi(argv[3]);
    size_t oversampling = 100;
    double dt = strtod(argv[4], NULL);

    std::ifstream input(argv[1]);
    PUMA::Simulator *simulation = initialize(&input, dt, oversampling);

    // Bind the current output method
    simulation->current_serializer = PUMA::Serializer::choose_output_method("vmd");
    std::ofstream output(filename);

    // The main loop
    for (size_t i = 0; i < total_timesteps * oversampling; ++i) {
        simulation->apply_step();
        if (i%oversampling == 0) simulation->serialize(&output, NULL);
    }

    return 0;
}
