#include "ProgressBar.h"

#include "Logger.h"

ProgressBar::ProgressBar(int maxYear, int simCount)
    : maxYear_{maxYear}, simCount_{simCount}
{
}

bool ProgressBar::shouldAddProgress(int currentValue, int maxValue) const
{
    return (currentValue % (maxValue / length_)) == 0;
}

bool ProgressBar::isEnding(int currentValue, int maxValue)
{
    return currentValue >= maxValue;
}