#pragma comment(lib, "winmm.lib")

#include "kuro/kuro_os.h"

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

    void
    os_sleep(double seconds)
    {
        static bool init = true;
        if (init)
        {
            init = false;
            timeBeginPeriod(1);
        }

        Sleep((WORD)(seconds * 1000.0));
    }
}