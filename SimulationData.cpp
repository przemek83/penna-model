#include "SimulationData.h"

SimulationAverages prepareSimulationAverages(int years)
{
    SimulationData<float> data;
    data.gompertz.fill(0);
    data.bity.fill(0);
    data.wiek.fill(0);
    data.rodziny.resize(static_cast<std::size_t>(years), 0);
    data.livingAtStart_.resize(static_cast<std::size_t>(years), 0);
    data.births_.resize(static_cast<std::size_t>(years), 0);
    data.livingAtEnd_.resize(static_cast<std::size_t>(years), 0);
    data.deaths_.resize(static_cast<std::size_t>(years), 0);

    return data;
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
