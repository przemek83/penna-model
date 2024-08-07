#include "ProgressCallback.h"

#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <vector>

#include "Logger.h"

namespace
{
std::vector<int> getProgressVector(int simulations)
{
    std::vector<int> progresses(static_cast<std::size_t>(simulations), 0);
    return progresses;
}

bool shouldAddProgress(int currentValue, int maxValue)
{
    const int progressLineLength{progress_callback::getLineLength()};
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

namespace progress_callback
{

int getLineLength() { return 50; }

[[maybe_unused]] std::function<void(int)> getSequentialProgressCallback(
    int sim, const config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            maxSim = params.simulationsCount_](int year)
    {
        if (year == 0)
        {
            Logger().log(std::to_string(simNumber) + "/" +
                         std::to_string(maxSim) + " " + progressLinePreffix);
            return;
        }

        if (shouldAddProgress(year, maxYears))
            Logger().log(std::string(1, progressLineMarker));

        if (isEnding(year + 1, maxYears))
            Logger().log(std::string(1, progressLineSuffix) + "\n");
    };
}

[[maybe_unused]] std::function<void(int)> getOverallProgressCallback(
    int sim, const config::Params& params)
{
    return [maxYears = params.years_, simNumber = sim,
            simCount = params.simulationsCount_](int year)
    {
        if (!shouldCalculateProgress(year, maxYears))
            return;

        static std::mutex mutex;
        int currentSum{0};
        {
            std::scoped_lock<std::mutex> lock(mutex);
            static std::vector<int> progresses{getProgressVector(simCount + 1)};
            const int sensitivity{maxYears / 100};
            progresses[static_cast<std::size_t>(simNumber)] =
                (year + 1) / sensitivity;
            currentSum = std::reduce(progresses.begin(), progresses.end());
        }

        if (currentSum == 1)
        {
            Logger().log({progressLinePreffix});
            return;
        }

        const int totalSum{simCount * 100};
        if (shouldAddProgress(currentSum, totalSum))
            Logger().log(std::string(1, progressLineMarker));

        if (isEnding(currentSum, totalSum))
            Logger().log(std::string(1, progressLineSuffix) + "\n");
    };
}
}  // namespace progress_callback
