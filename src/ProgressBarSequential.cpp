#include "ProgressBarSequential.h"

#include "Logger.h"

void ProgressBarSequential::update(int year, int sim)
{
    if (year == 0)
    {
        Logger().log(std::to_string(sim + 1) + "/" +
                     std::to_string(getSimCount()) + " " + getPreffix());
        return;
    }

    if (shouldAddProgress(year, getMaxYear()))
        Logger().log(getMarker());

    if (isEnding(year + 1, getMaxYear()))
        Logger().log(getSuffix() + "\n");
}
