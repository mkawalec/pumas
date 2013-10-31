#ifndef PUMA_Serializer_hpp
#define PUMA_Serializer_hpp

#include <fstream>
#include <list>
#include <string>
#include <boost/shared_array.hpp>

#include "helpers.hpp"

namespace PUMA {

    class Serializer {

    protected:
        void remove_instance(Serializer *instance_pointer);

    public:
        static std::list<Serializer*> output_methods;
        Serializer* choose_output_method(std::string name);

        std::string name;
        std::string description;

        virtual void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y) = 0;

    };

    class GnuplotSerializer : public Serializer {

    public:
        GnuplotSerializer();
        ~GnuplotSerializer() { remove_instance(this); };

        void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y);
    };

    class VMDSerializer : public Serializer {

    public:
        VMDSerializer();
        ~VMDSerializer() { remove_instance(this); };

        void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y);
    };
}

#endif
