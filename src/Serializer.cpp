#include "Serializer.hpp"

#include "helpers.hpp"
#include "exceptions.hpp"

#include <boost/shared_array.hpp>

namespace PUMA {

    /* ****             Serializer                  **** */

    std::list<Serializer*> Serializer::output_methods;

    void Serializer::remove_instance(Serializer *instance_pointer)
    {
        // As an instance is about to be destructed, remove any
        // references to this memory region
        Serializer::output_methods.remove(instance_pointer);
    }

    Serializer* Serializer::choose_output_method(std::string name)
    {
        for (std::list<Serializer*>::iterator it=output_methods.begin(); 
                it != output_methods.end(); ++it) {
            if ((*it)->name == name) return *it;
        }

        throw SerializerNotFound("Serializer " + name + " is not found");
    }

    /* ****             GnuplotSerializer           **** */

    GnuplotSerializer::GnuplotSerializer()
    {
        name = "gnuplot";
        description = "Outputs to Gnuplot compatible text format";
        Serializer::output_methods.push_back(this);
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

    GnuplotSerializer gnuplot_serializer_instance;

    /* ****             VMDSerializer               **** */

    VMDSerializer::VMDSerializer()
    {
        name = "vmd";
        description = "Outputs to VMD compatible XYZ file format";
        Serializer::output_methods.push_back(this);
    }

    void VMDSerializer::serialize(std::ofstream *output, 
            std::ofstream *nothing, boost::shared_array<landscape> current_state,
            size_t size_x, size_t size_y)
    {

        *output << 3 * size_x * size_y - 1<< std::endl;
        for (int j = 0; (unsigned)j < size_y; ++j) {
            for (int i = 0; (unsigned)i < size_x; ++i) {
                size_t index = j * size_x + i;
    
                *output << "hares " << i << " " << j << " " << 
                    current_state[index].hare_density * scale << std::endl;
                *output << "pumas " << i << " " << j << " " <<
                    current_state[index].puma_density * scale << std::endl;
                *output << "baseline " << i << " " << j << " 0" << std::endl;
            }
        }
    }
               
    VMDSerializer vmd_serializer_instance;
}


