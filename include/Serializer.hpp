#ifndef PUMA_Serializer_hpp
#define PUMA_Serializer_hpp

#include <fstream>
#include <list>
#include <string>
#include <boost/shared_array.hpp>

#include "helpers.hpp"
#include "exceptions.hpp"

namespace PUMA {

    /** \brief A scaffold class to build serializers from. 
     *
     *  Any serializer that is used by the Simulator 
     *  must inherit from this class and implement a 
     *  constructor, destructor and 'void serialize'. 
     *
     *  Also at least the name must be set and setting the description
     *  is strongly encouraged.
     */
    class Serializer {

    protected:
        /** \brief Function used to remove an instance pointer
         *      from the list of output methods.
         *  \param instance_pointer pointer to a Serializer
         *      instance present on output_methods list
         */
        void remove_instance(Serializer *instance_pointer);

    public:
        /** List containing currently available
         *  output methods.
         */
        static std::list<Serializer*> output_methods;

        /** \brief Returns a pointer to an output method 
         *      specified by its name.
         *  \param name serializer name
         *  \exception SerializerNotFound when name does not correspond
         *      to a name of a registered output method
         *  \return pointer to a requested Serializer instance
         */
        static Serializer* choose_output_method(std::string name) 
            throw(SerializerNotFound);

        /// Name of the Serializer
        std::string name;

        /// Human-readable Serializer description
        std::string description;

        /// A suggested extension for a given serializer
        std::string extension;

        /// Output value scaling
        double scale;

        // If set files splitting will be forced
        bool force_files_split;

        /** \brief Writes the puma/hare densities to
         *      the specified output stream(s)
         *  \param output_hares a pointer to an output stream
         *      to which densities of hares will go
         *  \param output_pumas a pointer to an output stream
         *      to which densities of pumas will go
         *  \param current_state contains the simulation state
         *      that will be serialized
         *  \param size_x X dimension of current_state
         *  \param size_y Y dimension of current_state
         */      
        virtual void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, 
                boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y) = 0;

    };

    /** \brief Outputs to a format compatible with a gnuplot
     *      script inside contrib directory
     */
    class GnuplotSerializer : public Serializer {

    public:
        GnuplotSerializer();
        ~GnuplotSerializer() { remove_instance(this); };

        void serialize(std::ofstream *output_hares, 
                std::ofstream *output_pumas, 
                boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y);
    };

    /** \brief Outputs to a VMD compatible XYZ file format. 
     *  \warning Requires to write considerably more data
     *      than the other serializers thus slowing everything
     *      down
     */
    class VMDSerializer : public Serializer {

    public:
        VMDSerializer();
        ~VMDSerializer() { remove_instance(this); };

        /** \brief Writes the puma/hare densities to
         *      the specified output stream
         *  \param output_hares a pointer to an output stream
         *      to which densities of hares will go
         *  \param nothing an unused pointer to the second,
         *      unneeded output stream
         *  \param current_state contains the simulation state
         *      that will be serialized
         *  \param size_x X dimension of current_state
         *  \param size_y Y dimension of current_state
         */      
        void serialize(std::ofstream *output, 
                std::ofstream *nothing, 
                boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y);
    };

    /// \brief Outputs to a PlainPPM format
    class PlainPPMSerializer : public Serializer {

    private:
        /** \brief converts colour values in HSV space
         *      to ones in RGB.
         *
         *  \return a triplet of RGB colour values
         */
        rgb densitiesToRGB(double hare_density, double puma_density);

    public:
        PlainPPMSerializer();
        ~PlainPPMSerializer() { remove_instance(this); };

        /** \brief Writes the puma/hare densities to
         *      the specified output stream
         *  \param output_hares a pointer to an output stream
         *      to which densities of hares will go
         *  \param nothing an unused pointer to the second,
         *      unneeded output stream
         *  \param current_state contains the simulation state
         *      that will be serialized
         *  \param size_x X dimension of current_state
         *  \param size_y Y dimension of current_state
         */      
        void serialize(std::ofstream *output, 
                std::ofstream *nothing, 
                boost::shared_array<landscape> current_state,
                size_t size_x, size_t size_y);
    };
}

#endif
