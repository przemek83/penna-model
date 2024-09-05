#include "ProgressBar.h"

#include <iostream>

#include "Config.h"

ProgressBar::ProgressBar(int maxYear, int simCount)
    : maxYear_{maxYear}, simCount_{simCount}
{
}

bool ProgressBar::shouldAddProgressMarker(int currentValue, int maxValue) const
{
    return (currentValue % (maxValue / config::progressBarLength)) == 0;
}

bool ProgressBar::isEnding(int currentValue, int maxValue) const
{
    return currentValue >= maxValue;
}

void ProgressBar::printProgress(const std::string& msg) const
{
    std::cout << msg;
    std::cout.flush();
}
