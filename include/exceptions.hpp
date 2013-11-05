#ifndef PUMA_exceptions_h
#define PUMA_exceptions_h

#include <string>

namespace PUMA {

    /** \brief The exception base class, every PUMA
     *      exception should inherit from it
     */
    struct Exception {
        std::string what() { return message;}

    protected:
        std::string message;

    public:
        Exception(std::string msg) { message = msg;}
        Exception() { message = "";}
    };

    /** \brief thrown when a variable has an
     *      illegal value.
     *
     *  The gcc version we are compiling against
     *  has no support for constructor inheritance,
     *  so we have to write that longhand.
     */
    struct IllegalValue : public Exception {
        IllegalValue(std::string msg) : Exception(msg) {};
        IllegalValue() : Exception() {};
    };

    /** \brief thrown if the requested
     *      serializer class cannot be found
     */
    struct SerializerNotFound : public Exception {
        SerializerNotFound(std::string msg) : Exception(msg) {};
        SerializerNotFound() : Exception() {};
    };

    /** \brief thrown if a non-main function wants
     *      to terminate program execution.
     *
     *  It's suggested to catch this exception and
     *  terminate gracefully
     */
    struct ProgramDeathRequest : public Exception {
        ProgramDeathRequest(std::string msg) : Exception(msg) {}
        ProgramDeathRequest() : Exception() {};
    };
}

#endif
