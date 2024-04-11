#pragma once

#include <ostream>
#include <vector>

#include "Config.h"

template <typename T>
class SimulationData;

using SingleSimulationData = SimulationData<int>;

template <typename T>
class SimulationData
{
public:
    explicit SimulationData(std::size_t years) : years_(years)
    {
        deathsDistribution_.resize(Config::Params::bits_, 0);
        bitsDistribution_.resize(Config::Params::bits_, 0);
        ageDistribution_.resize(Config::Params::bits_, 0);
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
        for (std::size_t i{0}; i < Config::Params::bits_; i++)
            ageDistribution_[i] = ageDistribution[i];
    }

    void setBitDistribution(const std::vector<int>& bitsDistribution,
                            int populationCount)
    {
        for (std::size_t i{0}; i < Config::Params::bits_; i++)
            bitsDistribution_[i] = static_cast<float>(bitsDistribution[i]) /
                                   static_cast<float>(populationCount);
    }

    void setDeathDistribution(
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution)
    {
        for (std::size_t i{0}; i < Config::Params::bits_; i++)
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

    void saveFamilies(std::ostream& stream, char separator) const
    {
        stream << "Year" << separator << "Families" << std::endl;
        for (size_t year{0}; year < static_cast<std::size_t>(years_); ++year)
        {
            const BasicMetrics& basicMetrics{getBasicBasicMetrics(year)};
            if (basicMetrics.families_ > 1)
                stream << year << separator << basicMetrics.families_
                       << std::endl;
        }
    }

    void saveBasicMetrics(std::ostream& stream, char separator) const
    {
        stream << "Year" << separator << "Living_start" << separator << "Births"
               << separator << "Living_end" << separator << "Deaths"
               << std::endl;
        for (size_t year{0}; year < static_cast<std::size_t>(years_); ++year)
        {
            const BasicMetrics& basicMetrics{getBasicBasicMetrics(year)};
            stream << year << separator << basicMetrics.livingAtStart_
                   << separator << basicMetrics.births_ << separator
                   << basicMetrics.getLivingAtEnd() << separator
                   << basicMetrics.deaths_ << std::endl;
        }
    }

    void saveBitsDistibution(std::ostream& stream, char separator) const
    {
        stream << "Bit" << separator << "Percent" << std::endl;
        for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
            stream << bit << separator << bitsDistribution_[bit] << std::endl;
    }

    void saveAgeDistibution(std::ostream& stream, char separator) const
    {
        stream << "Bit" << separator << "Count" << std::endl;
        for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
            stream << bit << separator << ageDistribution_[bit] << std::endl;
    }

    void saveDeathsDistibution(std::ostream& stream, char separator) const
    {
        stream << "Bit" << separator << "Percent" << std::endl;
        for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
            stream << bit << separator << deathsDistribution_[bit] << std::endl;
    }

private:
    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;
    std::vector<T> ageDistribution_;

    std::vector<BasicMetrics> basicMetrics_;

    int simulations_{0};
    std::size_t years_;
};
