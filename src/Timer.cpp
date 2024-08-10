#include "Timer.h"

#include <string>

#include "Logger.h"

Timer::~Timer()
{
    const auto end{std::chrono::steady_clock::now()};
    const auto timeElapsed{end - start_};
    const auto executionTime{
        std::chrono::duration_cast<std::chrono::seconds>(timeElapsed).count()};
    Logger().log("Execution time: " + std::to_string(executionTime) + "s.\n");
}
