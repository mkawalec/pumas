#include "Serializer.hpp"
#include "helpers.hpp"
#include <boost/shared_array.hpp>

namespace PUMA {

    std::list<Serializer*> Serializer::output_methods;

    GnuplotSerializer::GnuplotSerializer()
    {
        description = "Outputs to Gnuplot compatible text format";
        Serializer::output_methods.push_back(this);
    }

    GnuplotSerializer::~GnuplotSerializer()
    {
        // As an instance is about to be destructed, remove any
        // references to this memory region
        Serializer::output_methods.remove(this);
    }

    void GnuplotSerializer::serialize(std::ofstream *output_hares, 
            std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
            size_t size_x, size_t size_y)
    {
        for (int j = 0; (unsigned)j < size_y; ++j) {
            for (int i = 0; (unsigned)i < size_x; ++i) {
                size_t index = j * size_x + i;

                *output_hares << current_state[index].hare_density << " ";
                *output_pumas << current_state[index].puma_density << " ";
            }
            *output_hares << std::endl;
            *output_pumas << std::endl;
        }

        // Closing to make sure nothing corrupts the output
        output_hares->close();
        output_pumas->close();
    }

    GnuplotSerializer an_instance;
}


