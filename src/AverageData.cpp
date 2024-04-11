#include "AverageData.h"

#include "Config.h"

AverageData::AverageData(std::size_t years) : ResultsData(years)
{
    ageDistribution_.resize(Config::Params::bits_, 0);
    basicMetrics_.resize(getYears());
}

void AverageData::saveFamilies(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Families" << std::endl;
    for (size_t year{0}; year < static_cast<std::size_t>(getYears()); ++year)
    {
        const BasicMetrics<float>& basicMetrics{getBasicMetrics(year)};
        if (basicMetrics.families_ > 1)
            stream << year << separator << basicMetrics.families_ << std::endl;
    }
}

void AverageData::saveBasicMetrics(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Living_start" << separator << "Births"
           << separator << "Living_end" << separator << "Deaths" << std::endl;
    for (size_t year{0}; year < static_cast<std::size_t>(getYears()); ++year)
    {
        const BasicMetrics<float>& basicMetrics{getBasicMetrics(year)};
        stream << year << separator << basicMetrics.livingAtStart_ << separator
               << basicMetrics.births_ << separator
               << basicMetrics.getLivingAtEnd() << separator
               << basicMetrics.deaths_ << std::endl;
    }
}

void AverageData::saveAgeDistibution(std::ostream& stream, char separator) const
{
    stream << "Bit" << separator << "Count" << std::endl;
    for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
        stream << bit << separator << ageDistribution_[bit] << std::endl;
}

void AverageData::integrateData(const SimulationData& data)
{
    integrateBasicMetrics(data);
    integrateDistributions(data);
    simulations_++;
}

void AverageData::finalize()
{
    finalizeBasicMetrics();
    finalizeDistributions();
}

void AverageData::integrateBasicMetrics(const SimulationData& data)
{
    for (std::size_t i{0}; i < getYears(); ++i)
    {
        const SimulationData::BasicMetrics<int>& other{
            data.getBasicBasicMetrics(i)};

        basicMetrics_[i].families_ += static_cast<float>(other.families_);
        basicMetrics_[i].livingAtStart_ +=
            static_cast<float>(other.livingAtStart_);
        basicMetrics_[i].births_ += static_cast<float>(other.births_);
        basicMetrics_[i].deaths_ += static_cast<float>(other.deaths_);
    }
}

void AverageData::integrateDistributions(const SimulationData& data)
{
    const std::vector<int>& ageDistribution{data.getAgeDistribution()};

    for (std::size_t i{0}; i < Config::Params::bits_; i++)
    {
        setBitsDistributionValue(
            i, getBitsDistributionValue(i) + data.getBitsDistributionValue(i));
        ageDistribution_[i] += static_cast<float>(ageDistribution[i]);
        setDeathsDistributionValue(i, getDeathsDistributionValue(i) +
                                          data.getDeathsDistributionValue(i));
    }
}

void AverageData::finalizeBasicMetrics()
{
    const float simulationsAsFloat{static_cast<float>(simulations_)};
    for (std::size_t i{0}; i < getYears(); i++)
        basicMetrics_[i].families_ /= simulationsAsFloat;

    for (std::size_t i{0}; i < getYears(); i++)
    {
        basicMetrics_[i].livingAtStart_ /= simulationsAsFloat;
        basicMetrics_[i].births_ /= simulationsAsFloat;
        basicMetrics_[i].deaths_ /= simulationsAsFloat;
    }
}

void AverageData::finalizeDistributions()
{
    const float simulationsAsFloat{static_cast<float>(simulations_)};
    for (std::size_t v = 0; v < Config::Params::bits_; v++)
    {
        setDeathsDistributionValue(
            v, getDeathsDistributionValue(v) / simulationsAsFloat);
        setBitsDistributionValue(
            v, getBitsDistributionValue(v) / simulationsAsFloat);
        ageDistribution_[v] /= simulationsAsFloat;
    }
}

const AverageData::BasicMetrics<float>& AverageData::getBasicMetrics(
    std::size_t year) const
{
    return basicMetrics_[year];
}
