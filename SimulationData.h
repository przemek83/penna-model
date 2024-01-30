#pragma once

#include <vector>

#include "Config.h"

template <typename T>
class SimulationData;

using SimulationAverages = SimulationData<float>;
using SingleSimulationData = SimulationData<int>;

template <typename T>
class SimulationData
{
public:
    explicit SimulationData(std::size_t years) : years_(years)
    {
        deathsDistribution_.resize(years_, 0);
        bitsDistribution_.resize(years_, 0);
        ageDistribution_.resize(years_, 0);
        basicData_.resize(years_);
    }

    void integrateData(const SingleSimulationData& data);

    void finalize();

    struct BasicData
    {
        T families_{0};
        T livingAtStart_{0};
        T births_{0};
        T livingAtEnd_{0};
        T deaths_{0};
    };

    void setBasicData(std::vector<BasicData> basicData)
    {
        basicData_ = std::move(basicData);
    }

    const BasicData& getBasicData(std::size_t year) const
    {
        return basicData_[year];
    }

    const std::vector<float>& getDeathsDistribution() const
    {
        return deathsDistribution_;
    }

    const std::vector<float>& getBitsDistribution() const
    {
        return bitsDistribution_;
    }

    const std::vector<T>& getAgeDistribution() const
    {
        return ageDistribution_;
    }

    void setAgeDistribution(const std::vector<int>& ageDistribution)
    {
        for (std::size_t i{0}; i < Config::bits_; i++)
            ageDistribution_[i] = ageDistribution[i];
    }

    void setBitDistribution(const std::vector<int>& bitsDistribution,
                            int populationCount)
    {
        for (std::size_t i{0}; i < Config::bits_; i++)
            bitsDistribution_[i] = static_cast<float>(bitsDistribution[i]) /
                                   static_cast<float>(populationCount);
    }

    void setDeathDistribution(
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution)
    {
        for (std::size_t i{0}; i < Config::bits_; i++)
        {
            if (gompertzAgeDistribution[i] > 0)
            {
                deathsDistribution_[i] =
                    static_cast<float>(gompertzDeathsDistribution[i]) /
                    static_cast<float>(gompertzAgeDistribution[i]);
            }
            else
            {
                deathsDistribution_[i] = 1;
            }
        }
    }

private:
    void integrateBasicData(const SingleSimulationData& data);

    void integrateDistributions(const SingleSimulationData& data);

    void finalizeBasicData();

    void finalizeDistributions();

    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;
    std::vector<T> ageDistribution_;

    std::vector<BasicData> basicData_;

    int simulations_{0};
    std::size_t years_;
};
