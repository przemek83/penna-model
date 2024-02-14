#pragma once

#include <memory>
#include <ostream>
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
        basicMetrics_.resize(years_);
    }

    struct BasicMetrics
    {
        inline T getLivingAtEnd() const
        {
            return livingAtStart_ - deaths_ + births_;
        }

        T families_{0};
        T livingAtStart_{0};
        T births_{0};
        T deaths_{0};
    };

    void setBasicMetrics(std::vector<BasicMetrics> basicBasicMetrics)
    {
        basicMetrics_ = std::move(basicBasicMetrics);
    }

    const BasicMetrics& getBasicBasicMetrics(std::size_t year) const
    {
        return basicMetrics_[year];
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
        integrateBasicMetrics(data);
        integrateDistributions(data);
        simulations_++;
    }

    void finalize()
    {
        finalizeBasicMetrics();
        finalizeDistributions();
    }

    void saveFamilies(std::shared_ptr<std::ostream> stream,
                      char separator) const
    {
        *stream << "Year" << separator << "Families" << std::endl;
        for (size_t year{0}; year < static_cast<std::size_t>(years_); ++year)
        {
            const BasicMetrics& basicMetrics{getBasicBasicMetrics(year)};
            if (basicMetrics.families_ > 1)
                *stream << year << separator << basicMetrics.families_
                        << std::endl;
        }
    }

private:
    void integrateBasicMetrics(const SingleSimulationData& data)
    {
        for (std::size_t i{0}; i < years_; ++i)
        {
            const SingleSimulationData::BasicMetrics& other{
                data.getBasicBasicMetrics(i)};

            basicMetrics_[i].families_ += static_cast<float>(other.families_);
            basicMetrics_[i].livingAtStart_ +=
                static_cast<float>(other.livingAtStart_);
            basicMetrics_[i].births_ += static_cast<float>(other.births_);
            basicMetrics_[i].deaths_ += static_cast<float>(other.deaths_);
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

    void finalizeBasicMetrics()
    {
        const float simulationsAsFloat{static_cast<float>(simulations_)};
        for (std::size_t i{0}; i < years_; i++)
            basicMetrics_[i].families_ /= simulationsAsFloat;

        for (std::size_t i{0}; i < years_; i++)
        {
            basicMetrics_[i].livingAtStart_ /= simulationsAsFloat;
            basicMetrics_[i].births_ /= simulationsAsFloat;
            basicMetrics_[i].deaths_ /= simulationsAsFloat;
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

    std::vector<BasicMetrics> basicMetrics_;

    int simulations_{0};
    std::size_t years_;
};
