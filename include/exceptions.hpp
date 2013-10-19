#ifndef PUMA_exceptions_h
#define PUMA_exceptions_h

#include <string>

namespace PUMA {

    /** The exception base class, every PUMA
     *  exception should inherit from it
     */
    struct Exception {
        std::string what() { return message;}

        protected:
        std::string message;
    };

    /** Exception thrown when a variable has an
     *  illegal value.
     */
    struct IllegalValue : public Exception {
        IllegalValue(std::string msg) { message = msg;}
        IllegalValue() { message = "";}
    };
}

#endif
