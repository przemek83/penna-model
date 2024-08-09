#include "SimulationData.h"

SimulationData::SimulationData(std::size_t years, int bits)
    : ResultsData(years, bits)
{
    ageDistribution_.resize(getBits(), 0);
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
    const std::size_t bits{getBits()};
    for (std::size_t i{0}; i < bits; ++i)
        ageDistribution_[i] = ageDistribution[i];
}

float SimulationData::calculateBitDistributionValue(
    int populationCount, int bitsDistributionValue) const
{
    if (populationCount <= 0)
        return 1.F;

    return static_cast<float>(bitsDistributionValue) /
           static_cast<float>(populationCount);
}

void SimulationData::setBitDistribution(
    const std::vector<int>& bitsDistribution, int populationCount)
{
    const std::size_t bits{getBits()};
    for (std::size_t i{0}; i < bits; ++i)
    {
        float value{calculateBitDistributionValue(populationCount,
                                                  bitsDistribution[i])};
        setBitsDistributionValue(i, value);
    }
}

void SimulationData::setDeathDistribution(
    const std::vector<int>& gompertzDeathsDistribution,
    const std::vector<int>& gompertzAgeDistribution)
{
    const std::size_t bits{getBits()};
    for (std::size_t i{0}; i < bits; ++i)
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
