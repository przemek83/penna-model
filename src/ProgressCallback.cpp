#include "ProgressCallback.h"

#include <iostream>
#include <mutex>
#include <numeric>
#include <vector>

namespace
{
std::vector<int> getProgressVector(int simulations)
{
    std::vector<int> progresses(static_cast<std::size_t>(simulations), 0);
    return progresses;
}

const int progressLineLength{50};
const char progressLinePreffix{'['};
const char progressLineSuffix{']'};
const char progressLineMarker{'*'};
}  // namespace

namespace ProgressCallback
{

[[maybe_unused]] std::function<void(int)> createSequentialProgressCallback(
    int sim, const Config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            maxSim = params.simulationsCount_](int year)
    {
        if (year == 0)
            std::cout << simNumber << "/" << maxSim << " "
                      << progressLinePreffix;

        if ((year % (maxYears / progressLineLength)) == 0)
            std::cout << progressLineMarker;

        if (year + 1 == maxYears)
            std::cout << progressLineSuffix << std::endl;
    };
}

[[maybe_unused]] std::function<void(int)> createOverallProgressCallback(
    int sim, const Config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            simCount = params.simulationsCount_](int year)
    {
        const int sensitivity{maxYears / 100};
        if ((year + 1) % sensitivity != 0)
            return;

        static std::mutex mutex;
        mutex.lock();

        static std::vector<int> progresses{getProgressVector(simCount + 1)};
        progresses[static_cast<std::size_t>(simNumber)] =
            (year + 1) / sensitivity;
        const int currentSum{std::reduce(progresses.begin(), progresses.end())};
        mutex.unlock();

        const int totalSum{simCount * 100};
        if (currentSum == 1)
            std::cout << progressLinePreffix;
        if (currentSum % (totalSum / progressLineLength) == 0)
            std::cout << progressLineMarker;
        if (currentSum >= totalSum)
            std::cout << progressLineSuffix << std::endl;
    };
}
}  // namespace ProgressCallback
