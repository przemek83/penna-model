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

    void integrateData(const SingleSimulationData& data)
    {
        integrateBasicData(data);
        integrateDistributions(data);
        simulations_++;
    }

    void finalize()
    {
        finalizeBasicData();
        finalizeDistributions();
    }

private:
    void integrateBasicData(const SingleSimulationData& data)
    {
        for (std::size_t i{0}; i < years_; ++i)
        {
            const SingleSimulationData::BasicData& other{data.getBasicData(i)};

            basicData_[i].families_ += static_cast<float>(other.families_);
            basicData_[i].livingAtStart_ +=
                static_cast<float>(other.livingAtStart_);
            basicData_[i].births_ += static_cast<float>(other.births_);
            basicData_[i].livingAtEnd_ +=
                static_cast<float>(other.livingAtEnd_);
            basicData_[i].deaths_ += static_cast<float>(other.deaths_);
        }
    }

    void integrateDistributions(const SingleSimulationData& data)
    {
        const std::vector<float>& deathsDistribution{
            data.getDeathsDistribution()};
        const std::vector<float>& bitsDistribution{data.getBitsDistribution()};
        const std::vector<int>& ageDistribution{data.getAgeDistribution()};

        for (std::size_t i{0}; i < Config::bits_; i++)
        {
            bitsDistribution_[i] += bitsDistribution[i];
            ageDistribution_[i] += static_cast<float>(ageDistribution[i]);
            deathsDistribution_[i] += deathsDistribution[i];
        }
    }

    void finalizeBasicData()
    {
        const float simulationsAsFloat{static_cast<float>(simulations_)};
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

    void finalizeDistributions()
    {
        const float simulationsAsFloat{static_cast<float>(simulations_)};
        for (std::size_t v = 0; v < Config::bits_; v++)
        {
            deathsDistribution_[v] /= simulationsAsFloat;
            bitsDistribution_[v] /= simulationsAsFloat;
            ageDistribution_[v] /= simulationsAsFloat;
        }
    }

    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;
    std::vector<T> ageDistribution_;

    std::vector<BasicData> basicData_;

    int simulations_{0};
    std::size_t years_;
};
