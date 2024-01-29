#include "SimulationData.h"

template <>
void SimulationAverages::integrateData(const SingleSimulationData& data)
{
    for (std::size_t i{0}; i < years_; ++i)
    {
        const SingleSimulationData::BasicData& other{data.getBasicData(i)};

        basicData_[i].families_ += static_cast<float>(other.families_);
        basicData_[i].livingAtStart_ +=
            static_cast<float>(other.livingAtStart_);
        basicData_[i].births_ += static_cast<float>(other.births_);
        basicData_[i].livingAtEnd_ += static_cast<float>(other.livingAtEnd_);
        basicData_[i].deaths_ += static_cast<float>(other.deaths_);
    }

    const std::array<float, Config::bits_>& deathsDistribution{
        data.getDeathsDistribution()};
    const std::array<float, Config::bits_> bitsDistribution{
        data.getBitsDistribution()};
    const std::array<int, Config::bits_> ageDistribution{
        data.getAgeDistribution()};

    for (std::size_t i{0}; i < Config::bits_; i++)
    {
        bitsDistribution_[i] += bitsDistribution[i];
        ageDistribution_[i] += static_cast<float>(ageDistribution[i]);
        deathsDistribution_[i] += deathsDistribution[i];
    }

    simulations_++;
}

template <>
void SimulationAverages::finalize()
{
    const float simulationsAsFloat{static_cast<float>(simulations_)};
    for (std::size_t v = 0; v < Config::bits_; v++)
    {
        deathsDistribution_[v] /= simulationsAsFloat;
        bitsDistribution_[v] /= simulationsAsFloat;
        ageDistribution_[v] /= simulationsAsFloat;
    }

    for (std::size_t i{0}; i < years_; i++)
        basicData_[i].families_ /= simulationsAsFloat;

    for (std::size_t i{0}; i < years_; i++)
    {
        basicData_[i].livingAtStart_ /= simulationsAsFloat;
        basicData_[i].births_ /= simulationsAsFloat;
        basicData_[i].livingAtEnd_ /= simulationsAsFloat;
        basicData_[i].deaths_ /= simulationsAsFloat;
    }
}
