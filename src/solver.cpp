#include "Serializer.hpp"
#include "Simulator.hpp"
#include "exceptions.hpp"
#include "helpers.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
namespace po = boost::program_options;

const std::string version = "1.0";

/** \brief Creates a Simulator instance and initializes
 *      it with input data
 *  \param map_input pointer to a stream from which an
 *      input map can be read
 *  \return pointer to the created Simulator instance
 */
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

/** \brief Parses command line and config file params
 *      and sets the required values
 *  \param argc number of command line arguments
 *  \param argv array containing the command line arguments
 *  \param dt interval between loop iterations
 *  \param end_time time at which simulation ends
 *  \param print_every number of interations after which
 *      an output frame is printed
 *  \param notify_after number of frames after which
 *      a progress notification is printed
 *  \param output_fn filename of the main output file
 *  \param aux_output_fn filename of an auxiliary output
 *      file
 *  \param split_files if set to true each new frame will
 *      be printed in a separate file
 *  \return pointer to a completely set up Simulator
 *      instance
 */
PUMA::Simulator* read_params(int argc, char *argv[],
        double *dt, double *end_time,
        size_t *print_every, int *notify_after,
        std::string *output_fn, std::string *aux_output_fn,
        std::string *output_extension, bool *split_files)
{
    double r, a, b, m, k, l;
    std::string output_methods_desc="", output_method,
        input_filename, input_data_filename;

    /* Build an information string for different Serializers
     * from their names and descriptions
     */
    std::list<PUMA::Serializer*>::iterator it;
    for (it = PUMA::Serializer::output_methods.begin();
            it != PUMA::Serializer::output_methods.end(); ++it) {
        output_methods_desc += (*it)->name + ": \t" + 
            (*it)->description + "\n\n";
    }

    /* Define different parameter groups,
     * for decent presentation and easy management
     */
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
        ("aux,u",
         po::value<std::string>(aux_output_fn),
         "auxiliary output file, ie. puma output file, used by some output methods")
        ("output-format,f",
         po::value<std::string>(&output_method)->default_value("vmd"),
         ("The currently available output methods are: \n" + 
          output_methods_desc).c_str())
        ("output-extension,x",
          po::value<std::string>(output_extension),
          "override an output method defined output extension")
        ("notify-after,n", po::value<int>(notify_after)->default_value(30), 
         "print progress to stdout every n frames. Set to -1 to "
         "mute progress messages")
        ("split-files", po::value<bool>(split_files)->default_value(false),
         "print each frame in a separate output file. Setting to"
         " true overrides settings requested by chosen Serializer")
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

    /* Group the command line options in more
     * convenient groups, as we don't want the input
     * file option to be visible and generic options
     * should only be callable from the command line
     */
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

    // Handle generic options
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

    simulation->dt = *dt;

    // Bind the current output method
    simulation->current_serializer = 
        PUMA::Serializer::choose_output_method(output_method);

    return simulation;
}


int main(int argc, char *argv[])
{
    // Speeds up IO when there is a lot of IO to be done,
    // at the cost of making printf/scanf nonsafe to use
    std::ios_base::sync_with_stdio(0);

    // Parameters for the application
    int notify_after;
    bool split_files;
    size_t print_every;
    double dt, end_time;
    std::string output_fn, aux_output_fn, output_extension;
    PUMA::Simulator *simulation = NULL;

    /* Initialize the simulation, stopping execution
     * in case of nonrecoverable errors
     */
    try {
        simulation = read_params(argc, argv, &dt, &end_time, 
                &print_every, &notify_after, &output_fn, &aux_output_fn,
                &output_extension, &split_files);
    } catch (const PUMA::ProgramDeathRequest& e) {
        return 0;
    } catch (const PUMA::SerializerNotFound& e) {
        std::cerr << "The serializer you asked for could not be found\n";
        return -1;
    }

    std::ofstream output, aux_output;
    if (simulation->current_serializer->force_files_split)
        split_files = true;

    if (output_extension.length() == 0)
        output_extension = simulation->current_serializer->extension;

    if (!split_files) {
        output.open(output_fn + '.' + output_extension);
        if (aux_output_fn.length() > 0) 
            aux_output.open(aux_output_fn + '.' + output_extension);
    }

    // Starts Stopwatch
    long start_time = PUMA::get_time_micro_s();

    // The main loop
    for (size_t i = 0; i * dt < end_time; ++i) {
        simulation->apply_step();

        /* Only print a notification message if they are
         * not turned off. Print a new one every notify_after frames
         */
        if (notify_after != -1 && i%(print_every * notify_after) == 0) { 
            std::cout << i / print_every << " frames had been written\n";

            PUMA::average_densities averages = simulation->get_averages();
            std::cout << "Average hare and puma densities after " << i / print_every 
                << " frames are " << averages.first << " and " << averages.second
                << " respectively." << std::endl;
        }

        if (i%print_every == 0) {
            /* If file splitting is requested (either by the user or the
             * currently used Serializer) pad the consecutive output
             * files numbers with zeros.
             *
             * Otherwise, just serialize into output file(s)
             */
            if (split_files) {
                std::ostringstream output_number;
                std::string name1;

                output_number.width(log(end_time/(dt * print_every ))/log(10) + 1);
                output_number << std::setfill('0') << i / print_every;

                output.open(output_fn + output_number.str() + '.' + output_extension);
                if (aux_output_fn.length() > 0) 
                    aux_output.open(aux_output_fn + output_number.str() + 
                            '.' + output_extension);
                
                simulation->serialize(&output, &aux_output);

                output.close();
                if (aux_output_fn.length() > 0)
                    aux_output.close();
            } else {
                simulation->serialize(&output, &aux_output);
            }
        }
    }

    // Close the output files, if they require closing
    if (!split_files) {
        output.close();
        if (aux_output_fn.length() > 0) aux_output.close();
    }

    // Outputs the total runtime
    PUMA::format_time(PUMA::get_time_micro_s() - start_time); 

    return 0;
}
