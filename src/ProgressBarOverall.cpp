#include "ProgressBarOverall.h"

#include <numeric>

#include "Logger.h"

ProgressBarOverall::ProgressBarOverall(int maxYear, int simCount)
    : ProgressBar(maxYear, simCount), progresses_{simCount, 0}
{
}

void ProgressBarOverall::update(int year, int sim)
{
    if ((year != 0) && (!shouldCalculateProgress(year, getMaxYear())))
        return;

    int currentSum{0};
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        progresses_[static_cast<std::size_t>(sim)] = year + 1;
        currentSum = std::reduce(progresses_.begin(), progresses_.end());
    }

    if (currentSum == 1)
    {
        Logger().log({getPreffix()});
        return;
    }

    const int totalSum{getSimCount() * getMaxYear()};
    if (shouldAddProgressMarker(currentSum, totalSum))
        Logger().log(getMarker());

    if (isEnding(currentSum, totalSum))
        Logger().log(getSuffix() + "\n");
}

bool ProgressBarOverall::shouldCalculateProgress(int year, int maxYears) const
{
    const int sensitivity{maxYears / 100};
    return (year + 1) % sensitivity == 0;
}
