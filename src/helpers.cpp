#include "helpers.hpp"

namespace PUMA {

    long get_time_micro_s() 
    {
        timeval tv;
        gettimeofday(&tv,NULL);
        return (long)(1000000 * tv.tv_sec + tv.tv_usec);
    }

    void format_time(long microsecs)
    {
        long h,m,s,ms,us;
        h = microsecs / (unsigned long)(1000000.0 * 60.0 * 60.0);
        m = (microsecs / (1000000 * 60)) % 60;
        s = (microsecs / 1000000) % 60;
        ms = (microsecs / 1000) % 1000;
        us = microsecs % 1000;

        std::cout << h << "h: " << m << "m: " << s << "s: " << ms << "ms: " << us << "us" << std::endl;
    }

}
