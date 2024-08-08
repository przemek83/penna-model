#include "ProgressBarOverall.h"

#include <mutex>
#include <numeric>
#include <vector>

#include "Logger.h"

namespace
{
std::vector<int> getProgressVector(int simulations)
{
    std::vector<int> progresses(static_cast<std::size_t>(simulations), 0);
    return progresses;
}

// int getSensitivity(int years) { return years / 100; }

bool shouldCalculateProgress(int currentYear, int maxYears)
{
    return (currentYear + 1) % ProgressBarOverall::getSensitivity(maxYears) ==
           0;
}

}  // namespace

void ProgressBarOverall::update(int year, int sim)
{
    if (!shouldCalculateProgress(year, getMaxYear()))
        return;

    static std::mutex mutex;
    int currentSum{0};
    {
        std::scoped_lock<std::mutex> lock(mutex);
        static std::vector<int> progresses{
            getProgressVector(getSimCount() + 1)};
        progresses[static_cast<std::size_t>(sim)] =
            (year + 1) / getSensitivity(getMaxYear());
        currentSum = std::reduce(progresses.begin(), progresses.end());
    }

    if (currentSum == 1)
    {
        Logger().log({getPreffix()});
        return;
    }

    const int totalSum{getSimCount() * 100};
    if (shouldAddProgress(currentSum, totalSum))
        Logger().log(getMarker());

    if (isEnding(currentSum, totalSum))
        Logger().log(getSuffix() + "\n");
}