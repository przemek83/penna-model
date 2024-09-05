#include "ProgressBarOverall.h"

void ProgressBarOverall::update([[maybe_unused]] int year,
                                [[maybe_unused]] int sim)
{
    const int currentSum{++progress_};
    if (currentSum == 1)
    {
        printProgress({getPreffix()});
        return;
    }

    const int totalSum{getSimCount() * getMaxYear()};
    if (shouldAddProgressMarker(currentSum, totalSum))
        printProgress(getMarker());

    if (isEnding(currentSum, totalSum))
        printProgress(getSuffix() + "\n");
}
