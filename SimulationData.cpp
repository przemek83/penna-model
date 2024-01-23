#include "SimulationData.h"

SimulationAverages prepareSimulationAverages(int years)
{
    SimulationData<float> simulationAverages;
    simulationAverages.rodziny.resize(static_cast<std::size_t>(years));
    simulationAverages.livingAtStart_.resize(static_cast<std::size_t>(years));
    simulationAverages.births_.resize(static_cast<std::size_t>(years));
    simulationAverages.livingAtEnd_.resize(static_cast<std::size_t>(years));
    simulationAverages.deaths_.resize(static_cast<std::size_t>(years));

    return simulationAverages;
}

void prepareFinalResults(float simulationCount, int maxPopulationAge,
                         SimulationAverages& simulationData)
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
