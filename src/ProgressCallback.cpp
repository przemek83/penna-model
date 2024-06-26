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

bool shouldAddProgress(int currentValue, int maxValue)
{
    const int progressLineLength{50};
    return (currentValue % (maxValue / progressLineLength)) == 0;
}

bool isEnding(int currentValue, int maxValue)
{
    return currentValue >= maxValue;
}

bool shouldCalculateProgress(int currentYear, int maxYears)
{
    const int sensitivity{maxYears / 100};
    return (currentYear + 1) % sensitivity == 0;
}

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

        if (shouldAddProgress(year, maxYears))
            std::cout << progressLineMarker;

        if (isEnding(year + 1, maxYears))
            std::cout << progressLineSuffix << std::endl;

        std::cout.flush();
    };
}

[[maybe_unused]] std::function<void(int)> createOverallProgressCallback(
    int sim, const Config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            simCount = params.simulationsCount_](int year)
    {
        if (!shouldCalculateProgress(year, maxYears))
            return;

        static std::mutex mutex;
        mutex.lock();

        static std::vector<int> progresses{getProgressVector(simCount + 1)};
        const int sensitivity{maxYears / 100};
        progresses[static_cast<std::size_t>(simNumber)] =
            (year + 1) / sensitivity;
        const int currentSum{std::reduce(progresses.begin(), progresses.end())};
        mutex.unlock();

        const int totalSum{simCount * 100};
        if (currentSum == 1)
            std::cout << progressLinePreffix;
        if (shouldAddProgress(currentSum, totalSum))
            std::cout << progressLineMarker;
        if (isEnding(currentSum, totalSum))
            std::cout << progressLineSuffix << std::endl;

        std::cout.flush();
    };
}
}  // namespace ProgressCallback
