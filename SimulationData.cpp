#include "SimulationData.h"

SimulationData<float> prepareAvgData(int years)
{
    SimulationData<float> avgData;
    avgData.rodziny.resize(static_cast<std::size_t>(years));
    avgData.livingAtStart_.resize(static_cast<std::size_t>(years));
    avgData.births_.resize(static_cast<std::size_t>(years));
    avgData.livingAtEnd_.resize(static_cast<std::size_t>(years));
    avgData.deaths_.resize(static_cast<std::size_t>(years));

    return avgData;
}

void prepareFinalResults(float simulationCount, int maxPopulationAge,
                         SimulationData<float>& simulationData)
{
    for (std::size_t v = 0; v < Config::bits_; v++)
    {
        simulationData.gompertz[v] /= simulationCount;
        simulationData.bity[v] /= simulationCount;
        simulationData.wiek[v] /= simulationCount;
    }

    for (std::size_t v = 0; v < maxPopulationAge; v++)
        simulationData.rodziny[v] /= simulationCount;

    for (std::size_t v = 0; v < maxPopulationAge; v++)
    {
        simulationData.livingAtStart_[v] /= simulationCount;
        simulationData.births_[v] /= simulationCount;
        simulationData.livingAtEnd_[v] /= simulationCount;
        simulationData.deaths_[v] /= simulationCount;
    }
}
