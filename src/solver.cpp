#include "Serializer.hpp"
#include "Simulator.hpp"
#include "exceptions.hpp"

#include <fstream>
#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
namespace po = boost::program_options;

PUMA::Simulator* initialize(std::ifstream *map_input)
{
    size_t size_x, size_y;
    *map_input >> size_x >> size_y;

    bool *land_map = new bool[size_x*size_y];
    for (size_t j = 0; j < size_y; ++j) {
        for (size_t i = 0; i < size_x; ++i) {
            *map_input >> land_map[i + size_x * j];
        }
    }
    map_input->close();

    PUMA::Simulator *simulation = 
        new PUMA::Simulator(size_x, size_y, land_map);
    delete[] land_map;

    return simulation;
}

PUMA::Simulator* read_params(int argc, char *argv[],
        std::ifstream *input, double *dt, double *end_time,
        size_t *oversampling, size_t *notify_after,
        std::string *output_fn, std::string *aux_output_fn)
{
    double r, a, b, m, k, l;
    std::string output_methods_desc="", output_method;

    std::list<PUMA::Serializer*>::iterator it;
    for (it = PUMA::Serializer::output_methods.begin();
            it != PUMA::Serializer::output_methods.end(); ++it) {
        output_methods_desc += (*it)->name + ": \t" + 
            (*it)->description + "\n\n";
    }

    po::options_description generic_opts("Generic options");
    generic_opts.add_options()
        ("version,v", "print program version and exit")
        ("help,h", "produce help message")
        ;

    po::options_description file_opts("IO options");
    file_opts.add_options()
        ("output,o", 
            po::value<std::string>(output_fn)->default_value("output"),
            "the main output file")
        ("aux,auxiliary-output",
            po::value<std::string>(aux_output_fn)->default_value(""),
            "auxiliary output file, used by some output methods")
        ("output-format,f",
            po::value<std::string>(&output_method)->default_value("vmd"),
            ("The currently available output methods are: \n\n" + 
            output_methods_desc).c_str())
        ("notify-after,n", po::value<size_t>(notify_after)->default_value(30), 
            "print progress to stdout every n frames")
        ;

    po::options_description simulation_opts("Simulation options");
    simulation_opts.add_options()
        ("end_time,e", po::value<double>(end_time)->default_value(1000),
            "time at which the simulation ends")
        ("dt", po::value<double>(dt)->default_value(1), 
            "interval between output frames")
        ("oversampling,O", po::value<size_t>(oversampling)->default_value(100), 
            "number of iterations between two output frames")
        ;

    po::options_description simulation_params("Simulation parameters");
    simulation_params.add_options()
        ("r", po::value<double>(&r)->default_value(0.08), 
            "birth rate of hares")
        ("a", po::value<double>(&a)->default_value(0.04),
            "predation rate at which pumas eat hares")
        ("b", po::value<double>(&b)->default_value(0.02),
            "birth rate of pumas per one hare eaten")
        ("m", po::value<double>(&m)->default_value(0.06),
            "puma mortality rate")
        ("k", po::value<double>(&k)->default_value(0.2),
            "diffusion rate for hares")
        ("l", po::value<double>(&l)->default_value(0.2),
            "diffusion rate for pumas")
        ;

    po::options_description cmdline_opts;
    cmdline_opts.add(generic_opts).add(file_opts).
        add(simulation_opts).add(simulation_params);

    po::options_description config_file_opts;
    config_file_opts.add(file_opts).add(simulation_opts).
        add(simulation_params);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_opts), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << cmdline_opts << std::endl;
        throw PUMA::ProgramDeathRequest();
    }

    PUMA::Simulator *simulation = initialize(input);

    // Set the equation parameters
    simulation->r = vm["r"].as<double>();
    simulation->a = vm["a"].as<double>();
    simulation->b = vm["b"].as<double>();
    simulation->m = vm["m"].as<double>();
    simulation->k = vm["k"].as<double>();
    simulation->l = vm["l"].as<double>();

    // Bind the current output method
    simulation->current_serializer = 
        PUMA::Serializer::choose_output_method(output_method);

    return simulation;
}


int main(int argc, char *argv[])
{
    // Speeds up IO when there is a lot of IO to be done,
    // at the cost of making printf/scanf nonsafe
    std::ios_base::sync_with_stdio(0);

    // Command line parameters
    size_t oversampling, notify_after;
    double dt, end_time;
    std::string output_fn, aux_output_fn;
    
    if (argc < 2) {
        std::cout << "You need to specify an input file\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    PUMA::Simulator *simulation = NULL;

    try {
        simulation = read_params(argc, argv, &input, &dt, &end_time, 
                &oversampling, &notify_after, &output_fn, &aux_output_fn);
    } catch (PUMA::ProgramDeathRequest e) {
        return 0;
    } catch (PUMA::SerializerNotFound e) {
        std::cout << "The serializer you asked for could not be found\n";
        return -1;
    }

    std::ofstream output, aux_output;
    output.open(output_fn);
    if (aux_output_fn != "") aux_output.open(aux_output_fn);

    // The main loop
    for (size_t i = 0; i * dt / oversampling < end_time; ++i) {
        if (i%(oversampling * notify_after) == 0) 
            std::cout << i / oversampling << " frames had been written" << std::endl;

        simulation->apply_step();
        if (i%oversampling == 0) simulation->serialize(&output, &aux_output);
    }

    return 0;
}
