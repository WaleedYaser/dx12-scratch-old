#include "kuro/kuro_os.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace kuro
{
    f64
    os_seconds()
    {
        static LARGE_INTEGER frequency;
        if (frequency.QuadPart == 0)
            ::QueryPerformanceFrequency(&frequency);

        LARGE_INTEGER now;
        ::QueryPerformanceCounter(&now);
        return (f64)now.QuadPart / (f64)frequency.QuadPart;
    }
}