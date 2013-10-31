#ifndef PUMA_Serializer_hpp
#define PUMA_Serializer_hpp

#include <fstream>
#include <list>
#include <string>
#include <boost/shared_array.hpp>

#include "helpers.hpp"

namespace PUMA {

    class Serializer {

    public:
        static std::list<Serializer*> output_methods;
        virtual void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y) = 0;
        Serializer() {};
    };

    class GnuplotSerializer : public Serializer {

    public:
        GnuplotSerializer();
        ~GnuplotSerializer();

        std::string name;
        std::string description;

        void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y);
    };
}

#endif
