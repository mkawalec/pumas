#include "Serializer.hpp"

#include "helpers.hpp"
#include "exceptions.hpp"

#include <boost/shared_array.hpp>
#include <iostream>

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
        std::cout << name << std::endl;
        throw SerializerNotFound("Serializer " + name + " is not found");
    }

    /* ****             GnuplotSerializer           **** */

    GnuplotSerializer::GnuplotSerializer()
    {
        name = "gnuplot";
        description = "Outputs to Gnuplot compatible text format. " 
                      "Requires the auxiliary output file";
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
    }

    GnuplotSerializer gnuplot_serializer_instance;

    /* ****             VMDSerializer               **** */

    VMDSerializer::VMDSerializer()
    {
        name = "vmd";
        description = "Outputs to VMD compatible XYZ file format";
        scale = 10.0;

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
                
                if (current_state[index].is_land) *output << "land ";
                else *output << "water ";

                *output << i << " " << j << " 0" << std::endl;

            }
        }
    }
               
    VMDSerializer vmd_serializer_instance;

    /* ****             PlainPPMSerializer               **** */

    PlainPPMSerializer::PlainPPMSerializer()
    {
        name = "plainppm";
        description = "Outputs Plain PPM file. "
                      "Requires the auxiliary output file.";
        scale = 10.0;

        Serializer::output_methods.push_back(this);
    }

    void PlainPPMSerializer::serialize(std::ofstream *output_hares, 
            std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
            size_t size_x, size_t size_y)
    {
        //PlainPPM magic number
        *output_hares << "P3" << std::endl;
        *output_pumas << "P3" << std::endl;
        // width and height
        *output_hares << size_x << " " << size_y << std::endl;
        *output_pumas << size_x << " " << size_y << std::endl;
        // MaxVal so that each sample is 1 byte.
        *output_hares << 255  << std::endl;
        *output_pumas << 255  << std::endl;
        // raster where we have one value per line to respect the character limit per line
        for (int j = 0; (unsigned)j < size_y; ++j) {
            for (int i = 0; (unsigned)i < size_x; ++i) {
                size_t index = j * size_x + i;

                *output_hares << current_state[index].hare_density << std::endl;
                *output_pumas << current_state[index].puma_density << std::endl;
            }
        }
    }
               
    PlainPPMSerializer plainppm_serializer_instance;

}


