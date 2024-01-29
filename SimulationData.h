#pragma once

#include <array>
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
        deathsDistribution_.fill(0);
        bitsDistribution_.fill(0);
        ageDistribution_.fill(0);
        basicData_.resize(static_cast<std::size_t>(years_));
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

    const std::array<float, Config::bits_>& getDeathsDistribution() const
    {
        return deathsDistribution_;
    }

    const std::array<float, Config::bits_>& getBitsDistribution() const
    {
        return bitsDistribution_;
    }

    const std::array<T, Config::bits_>& getAgeDistribution() const
    {
        return ageDistribution_;
    }

    void setDistributions(
        const std::array<int, Config::bits_>& ageDistribution,
        const std::array<int, Config::bits_>& bitsDistribution,
        const std::array<int, Config::bits_>& gompertzDeathsDistribution,
        const std::array<int, Config::bits_>& gompertzAgeDistribution,
        int populationCount)
    {
        for (std::size_t i{0}; i < Config::bits_; i++)
        {
            bitsDistribution_[i] = static_cast<float>(bitsDistribution[i]) /
                                   static_cast<float>(populationCount);
            ageDistribution_[i] = ageDistribution[i];
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

    std::array<float, Config::bits_> deathsDistribution_;
    std::array<float, Config::bits_> bitsDistribution_;
    std::array<T, Config::bits_> ageDistribution_;

    std::vector<BasicData> basicData_;

    int simulations_{0};
    std::size_t years_;
};
