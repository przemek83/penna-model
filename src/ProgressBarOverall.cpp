#include "ProgressBarOverall.h"

#include "Logger.h"

ProgressBarOverall::ProgressBarOverall(int maxYear, int simCount)
    : ProgressBar(maxYear, simCount)
{
}

void ProgressBarOverall::update([[maybe_unused]] int year,
                                [[maybe_unused]] int sim)
{
    const int currentSum{++progress_};
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
