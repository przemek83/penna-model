#include "Timer.h"

#include <iostream>

Timer::~Timer()
{
    const auto end{std::chrono::steady_clock::now()};
    const auto timeElapsed{end - start_};
    std::cout
        << "Execution time: "
        << std::chrono::duration_cast<std::chrono::seconds>(timeElapsed).count()
        << "s." << std::endl;
}
