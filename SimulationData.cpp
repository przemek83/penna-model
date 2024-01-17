#include "SimulationData.h"

namespace SimulationData
{
AvgData prepareAvgData(int years)
{
    AvgData avgData;
    avgData.rodziny.resize(static_cast<std::size_t>(years));
    avgData.livingAtStart_.resize(static_cast<std::size_t>(years));
    avgData.births_.resize(static_cast<std::size_t>(years));
    avgData.livingAtEnd_.resize(static_cast<std::size_t>(years));
    avgData.deaths_.resize(static_cast<std::size_t>(years));

    return avgData;
}

};  // namespace SimulationData
