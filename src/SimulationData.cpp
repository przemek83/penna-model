#include "SimulationData.h"

#include "Config.h"

SimulationData::SimulationData(std::size_t years) : ResultsData(years)
{
    ageDistribution_.resize(Config::Params::bits_, 0);
    basicMetrics_.resize(getYears());
}

void SimulationData::setBasicMetrics(
    std::vector<BasicMetrics<int>> basicBasicMetrics)
{
    basicMetrics_ = std::move(basicBasicMetrics);
}

const SimulationData::BasicMetrics<int>& SimulationData::getBasicMetrics(
    std::size_t year) const
{
    return basicMetrics_[year];
}

const std::vector<int>& SimulationData::getAgeDistribution() const
{
    return ageDistribution_;
}

void SimulationData::setAgeDistribution(const std::vector<int>& ageDistribution)
{
    for (std::size_t i{0}; i < Config::Params::bits_; i++)
        ageDistribution_[i] = ageDistribution[i];
}

void SimulationData::setBitDistribution(
    const std::vector<int>& bitsDistribution, int populationCount)
{
    for (std::size_t i{0}; i < Config::Params::bits_; i++)
    {
        const float value{static_cast<float>(bitsDistribution[i]) /
                          static_cast<float>(populationCount)};
        setBitsDistributionValue(i, value);
    }
}

void SimulationData::setDeathDistribution(
    const std::vector<int>& gompertzDeathsDistribution,
    const std::vector<int>& gompertzAgeDistribution)
{
    for (std::size_t i{0}; i < Config::Params::bits_; i++)
    {
        float value{1};
        if (gompertzAgeDistribution[i] > 0)
            value = static_cast<float>(gompertzDeathsDistribution[i]) /
                    static_cast<float>(gompertzAgeDistribution[i]);

        setDeathsDistributionValue(i, value);
    }
}

void SimulationData::saveFamilies(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Families" << std::endl;
    for (size_t year{0}; year < static_cast<std::size_t>(getYears()); ++year)
    {
        const BasicMetrics<int>& basicMetrics{getBasicMetrics(year)};
        if (basicMetrics.families_ > 1)
            stream << year << separator << basicMetrics.families_ << std::endl;
    }
}

void SimulationData::writeLifeRelatedMetricData(std::ostream& stream,
                                                std::size_t year,
                                                char separator) const
{
    basicMetrics_[year].serializeLifeRelatedData(stream, separator);
}

void SimulationData::saveAgeDistibution(std::ostream& stream,
                                        char separator) const
{
    stream << "Bit" << separator << "Count" << std::endl;
    for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
        stream << bit << separator << ageDistribution_[bit] << std::endl;
}
