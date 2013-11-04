#include "Serializer.hpp"
#include "Simulator.hpp"
#include "exceptions.hpp"

#include <fstream>
#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
namespace po = boost::program_options;

const std::string version = "0.0.1";

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
        double *dt, double *end_time,
        size_t *print_every, size_t *notify_after,
        std::string *output_fn, std::string *aux_output_fn)
{
    double r, a, b, m, k, l;
    std::string output_methods_desc="", output_method,
        input_filename, input_data_filename;

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
        ("data-file,d", po::value<std::string>(&input_data_filename),
            "file with input parameters")
        ("output,o", 
            po::value<std::string>(output_fn)->default_value("output"),
            "the main output file, or hares output file for methods requiring auxiliary outputs")
        ("aux,auxiliary-output",
            po::value<std::string>(aux_output_fn)->default_value(""),
            "auxiliary output file, ie. puma output file, used by some output methods")
        ("output-format,f",
            po::value<std::string>(&output_method)->default_value("vmd"),
            ("The currently available output methods are: \n" + 
            output_methods_desc).c_str())
        ("notify-after,n", po::value<size_t>(notify_after)->default_value(30), 
            "print progress to stdout every n frames")
        ;

    po::options_description simulation_opts("Simulation options");
    simulation_opts.add_options()
        ("end_time,e", po::value<double>(end_time)->default_value(1000),
            "time at which the simulation ends")
        ("dt", po::value<double>(dt)->default_value(0.01), 
            "interval between computation steps")
        ("print-every,p", po::value<size_t>(print_every)->default_value(100), 
            "number of iterations between two output frames")
        ;

    po::options_description simulation_params("Simulation parameters");
    simulation_params.add_options()
        ("r,r", po::value<double>(&r)->default_value(0.08), 
            "birth rate of hares")
        ("a,a", po::value<double>(&a)->default_value(0.04),
            "predation rate at which pumas eat hares")
        ("b,b", po::value<double>(&b)->default_value(0.02),
            "birth rate of pumas per one hare eaten")
        ("m,m", po::value<double>(&m)->default_value(0.06),
            "puma mortality rate")
        ("k,k", po::value<double>(&k)->default_value(0.2),
            "diffusion rate for hares")
        ("l,l", po::value<double>(&l)->default_value(0.2),
            "diffusion rate for pumas")
        ;

    po::options_description hidden_opts("Hidden parameters");
    hidden_opts.add_options()
        ("input-file,I", po::value<std::string>(&input_filename),
            "input file containing a landmap")
        ;

    po::options_description cmdline_opts;
    cmdline_opts.add(generic_opts).add(file_opts).
        add(simulation_opts).add(simulation_params).
        add(hidden_opts);

    po::options_description config_file_opts;
    config_file_opts.add(file_opts).add(simulation_opts).
        add(simulation_params);

    po::options_description visible_opts;
    visible_opts.add(generic_opts).add(file_opts).
        add(simulation_opts).add(simulation_params);
    
    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(cmdline_opts).positional(p).run(), vm);
    po::notify(vm);

    /* If an input file was provided, load the options
     * from it, with the command-line options taking 
     * precedence
     */
    if (vm.count("data-file")) {
        std::ifstream input_data(input_data_filename);
        if (!input_data) {
            std::cerr << "Could not open input file " << 
                input_data_filename << "!" << std::endl;
            throw PUMA::ProgramDeathRequest();
        }
        po::store(po::parse_config_file(input_data, config_file_opts),
                vm);
        po::notify(vm);
    }

    if (vm.count("help")) {
        std::cerr << visible_opts << std::endl;
        throw PUMA::ProgramDeathRequest();
    } else if (vm.count("version")) {
        std::cerr << "The version running is " << 
            version << std::endl;
        throw PUMA::ProgramDeathRequest();
    } else if (!vm.count("input-file")) {
        std::cerr << "You need to provide an input file\n";
        throw PUMA::ProgramDeathRequest();
    }

    std::ifstream input(input_filename);
    PUMA::Simulator *simulation = initialize(&input);

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
    size_t print_every, notify_after;
    double dt, end_time;
    std::string output_fn, aux_output_fn;
    PUMA::Simulator *simulation = NULL;

    try {
        simulation = read_params(argc, argv, &dt, &end_time, 
                &print_every, &notify_after, &output_fn, &aux_output_fn);
    } catch (const PUMA::ProgramDeathRequest& e) {
        return 0;
    } catch (const PUMA::SerializerNotFound& e) {
        std::cerr << "The serializer you asked for could not be found\n";
        return -1;
    }

    std::ofstream output, aux_output;
    output.open(output_fn);
    if (aux_output_fn != "") aux_output.open(aux_output_fn);

    average_densities averages;

    // The main loop
    for (size_t i = 0; i * dt < end_time; ++i) {
        simulation->apply_step();

        averages = simulation->get_averages();
        if (i%(print_every * notify_after) == 0) { 
            std::cout << i / print_every << " frames had been written" 
                << std::endl;
            std::cout << "Average hare and puma densities after " << i / print_every 
                << " frames are " << averages.hares << " and " << averages.pumas
                << " respectively." << std::endl;
        }

        if (i%print_every == 0) simulation->serialize(&output, &aux_output);
    }

    return 0;
}
