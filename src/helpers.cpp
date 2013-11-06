#include "helpers.hpp"

namespace PUMA {

    double get_time_micro_s() 
    {
        timeval tv;
        gettimeofday(&tv,NULL);
        return 1000000 * tv.tv_sec + tv.tv_usec;
    }

    void format_time(double microsecs)
    {
        long int h,m,s,ms,us;
        h = microsecs / (1000000 * 60 * 60);
        m = (long int)(microsecs / (1000000 * 60)) % 60;
        s = (long int)(microsecs / 1000000) % 60;
        ms = (long int)(microsecs / 1000) % 1000;
        us = (long int)microsecs % 1000;

        std::cout << h << "h: " << m << "m: " << s << "s: " << ms << "ms: " << us << "us" << std::endl;
    }

}
