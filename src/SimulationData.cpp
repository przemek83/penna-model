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
    for (std::size_t i{0}; i < ageDistribution.size(); i++)
        ageDistribution_[i] = ageDistribution[i];
}

void SimulationData::setBitDistribution(
    const std::vector<int>& bitsDistribution, int populationCount)
{
    for (std::size_t i{0}; i < bitsDistribution.size(); i++)
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
    for (std::size_t i{0}; i < gompertzAgeDistribution.size(); i++)
    {
        float value{1};
        if (gompertzAgeDistribution[i] > 0)
            value = static_cast<float>(gompertzDeathsDistribution[i]) /
                    static_cast<float>(gompertzAgeDistribution[i]);

        setDeathsDistributionValue(i, value);
    }
}

void SimulationData::writeLifeRelatedMetricData(std::ostream& stream,
                                                std::size_t year,
                                                char separator) const
{
    basicMetrics_[year].serializeLifeRelatedData(stream, separator);
}

void SimulationData::writeFamiliesMetricData(std::ostream& stream,
                                             std::size_t year) const
{
    basicMetrics_[year].serializeFamily(stream);
}

void SimulationData::writeBitDistributionData(std::ostream& stream,
                                              std::size_t bit) const
{
    stream << ageDistribution_[bit];
}

bool SimulationData::isSingleFamily(std::size_t year) const
{
    return basicMetrics_[year].families_ <= 1;
}
