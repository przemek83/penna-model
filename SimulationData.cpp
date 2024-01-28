#include "SimulationData.h"

void prepareFinalResults(float simulationCount, int maxPopulationAge,
                         SimulationAverages& simulationData)
{
    for (std::size_t v = 0; v < Config::bits_; v++)
    {
        simulationData.deathsDistribution_[v] /= simulationCount;
        simulationData.bitsDistribution_[v] /= simulationCount;
        simulationData.ageDistribution_[v] /= simulationCount;
    }

    for (std::size_t v = 0; v < maxPopulationAge; v++)
        simulationData.families_[v] /= simulationCount;

    for (std::size_t v = 0; v < maxPopulationAge; v++)
    {
        simulationData.livingAtStart_[v] /= simulationCount;
        simulationData.births_[v] /= simulationCount;
        simulationData.livingAtEnd_[v] /= simulationCount;
        simulationData.deaths_[v] /= simulationCount;
    }
}

void integrateData(SimulationAverages& simulationAverages,
                   const SingleSimulationData& singleSimulationData)
{
    for (std::size_t i{0}; i < simulationAverages.births_.size(); ++i)
    {
        simulationAverages.families_[i] += singleSimulationData.families_[i];
        simulationAverages.livingAtStart_[i] +=
            singleSimulationData.livingAtStart_[i];
        simulationAverages.births_[i] += singleSimulationData.births_[i];
        simulationAverages.livingAtEnd_[i] +=
            singleSimulationData.livingAtEnd_[i];
        simulationAverages.deaths_[i] += singleSimulationData.deaths_[i];
    }

    for (std::size_t i{0}; i < Config::bits_; i++)
    {
        simulationAverages.bitsDistribution_[i] += singleSimulationData.bitsDistribution_[i];
        simulationAverages.ageDistribution_[i] += singleSimulationData.ageDistribution_[i];
        simulationAverages.deathsDistribution_[i] += singleSimulationData.deathsDistribution_[i];
    }
}
