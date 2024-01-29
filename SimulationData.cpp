#include "SimulationData.h"

template <>
void SimulationAverages::integrateData(const SingleSimulationData& data)
{
    for (std::size_t i{0}; i < years_; ++i)
    {
        const SingleSimulationData::BasicData& basicData{data.getBasicData(i)};

        basicData_[i].families_ += basicData.families_;
        basicData_[i].livingAtStart_ += basicData.livingAtStart_;
        basicData_[i].births_ += basicData.births_;
        basicData_[i].livingAtEnd_ += basicData.livingAtEnd_;
        basicData_[i].deaths_ += basicData.deaths_;
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
        ageDistribution_[i] += ageDistribution[i];
        deathsDistribution_[i] += deathsDistribution[i];
    }

    simulations_++;
}

template <>
void SimulationAverages::finalize()
{
    for (std::size_t v = 0; v < Config::bits_; v++)
    {
        deathsDistribution_[v] /= simulations_;
        bitsDistribution_[v] /= simulations_;
        ageDistribution_[v] /= simulations_;
    }

    for (std::size_t i{0}; i < years_; i++)
        basicData_[i].families_ /= simulations_;

    for (std::size_t i{0}; i < years_; i++)
    {
        basicData_[i].livingAtStart_ /= simulations_;
        basicData_[i].births_ /= simulations_;
        basicData_[i].livingAtEnd_ /= simulations_;
        basicData_[i].deaths_ /= simulations_;
    }
}
