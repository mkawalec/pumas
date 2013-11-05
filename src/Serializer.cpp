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
        throw(SerializerNotFound)
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
        extension = "xyz";
        scale = 10.0;

        Serializer::output_methods.push_back(this);
    }

    void VMDSerializer::serialize(std::ofstream *output, 
            std::ofstream *nothing, boost::shared_array<landscape> current_state,
            size_t size_x, size_t size_y)
    {
        ignore(nothing);

        /* Set the number of particles to be three times
         * the number of coordinates (one for simulation
         * plane, one for hare density and one for pumas
         * density). 
         *
         * Then print XYZ file, with hares as sulfur atoms,
         * pumas as hydrogen, land as carbon and water as
         * oxygen.
         */
        *output << 3 * size_x * size_y - 1<< std::endl;
        for (int j = 0; (unsigned)j < size_y; ++j) {
            for (int i = 0; (unsigned)i < size_x; ++i) {
                size_t index = j * size_x + i;

                *output << "S " << i << " " << j << " " << 
                    current_state[index].hare_density * scale << std::endl;
                *output << "H " << i << " " << j << " " <<
                    current_state[index].puma_density * scale << std::endl;

                if (current_state[index].is_land) *output << "C ";
                else *output << "O ";

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
        extension = "ppm";
        scale = 10.0;
        force_files_split = true;

        Serializer::output_methods.push_back(this);
    }

    /**This code is partially taken from 
     * Ref: http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb
     */
    rgb PlainPPMSerializer::densitiesToRGB(double hare_density, double puma_density)
    {

        double v ,s ,H, x, y, z, remH;
        int floorH;
        double max_difference = 2.5;
        rgb RGB;

        H = ((hare_density - puma_density) / max_difference + 0.5) * 6.0;

        v = 0.6;
        s = 0.7;
        floorH = (int)H;
        remH = H - floorH;

        x = v * (1.0 - s);
        y = v * (1.0 - (s * remH));
        z = v * (1.0 - (s * (1.0 - remH)));

        switch(floorH) {
            case 0:
                RGB.r = (int)(v * 255);
                RGB.g = (int)(z * 255);
                RGB.b = (int)(x * 255);
                break;
            case 1:
                RGB.r = (int)(y * 255);
                RGB.g = (int)(v * 255);
                RGB.b = (int)(x * 255);
                break;
            case 2:
                RGB.r = (int)(x * 255);
                RGB.g = (int)(v * 255);
                RGB.b = (int)(z * 255);
                break;
            case 3:
                RGB.r = (int)(x * 255);
                RGB.g = (int)(y * 255);
                RGB.b = (int)(v * 255);
                break;
            case 4:
                RGB.r = (int)(z * 255);
                RGB.g = (int)(x * 255);
                RGB.b = (int)(v * 255);
                break;
            case 5:
            default:
                RGB.r = (int)(v * 255);
                RGB.g = (int)(x * 255);
                RGB.b = (int)(y * 255);
                break;
        }
        return RGB;
    }

    void PlainPPMSerializer::serialize(std::ofstream *output, 
            std::ofstream *nothing, boost::shared_array<landscape> current_state,
            size_t size_x, size_t size_y)
    {
        ignore(nothing);
        rgb colours;

        //PlainPPM magic number
        *output << "P3" << std::endl;

        // width and height
        *output << size_x << " " << size_y << std::endl;
        // MaxVal so that each sample is 1 byte.
        *output << 255 << std::endl;

        // raster where we have one value per line to respect the character limit per line
        for (int j = 0; (unsigned)j < size_y; ++j) {
            for (int i = 0; (unsigned)i < size_x; ++i) {
                size_t index = j * size_x + i;

                colours = densitiesToRGB(current_state[index].hare_density, current_state[index].puma_density);
                *output << colours.r << " " << colours.g << " " << colours.b << std::endl;
            }
        }
    }

    PlainPPMSerializer plainppm_serializer_instance;

}


