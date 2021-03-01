#include "kuro_os.h"

#include <time.h>

namespace kuro
{
    f64
    os_seconds()
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec + now.tv_nsec / 1000000000.0;
    }
}