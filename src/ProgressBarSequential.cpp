#include "ProgressBarSequential.h"

#include <string>

void ProgressBarSequential::update(int year, int sim)
{
    if (year == 0)
    {
        printProgress(std::to_string(sim + 1) + "/" +
                      std::to_string(getSimCount()) + " " + getPreffix());
        return;
    }

    if (shouldAddProgressMarker(year + 1, getMaxYear()))
        printProgress(getMarker());

    if (isEnding(year + 1, getMaxYear()))
        printProgress(getSuffix() + "\n");
}
