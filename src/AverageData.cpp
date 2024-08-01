#include "AverageData.h"

#include "Config.h"

AverageData::AverageData(const std::vector<SimulationData>& simulationsData,
                         std::size_t years, int bits)
    : ResultsData(years, bits)
{
    ageDistribution_.resize(getBits(), 0);
    basicMetrics_.resize(getYears());

    for (const auto& data : simulationsData)
        integrate(data);

    finalize();
}

void AverageData::integrate(const SimulationData& data)
{
    integrateBasicMetrics(data);
    integrateDistributions(data);
    ++simulations_;
}

void AverageData::finalize()
{
    finalizeBasicMetrics();
    finalizeDistributions();
}

void AverageData::writeLifeRelatedMetricData(std::ostream& stream,
                                             std::size_t year,
                                             char separator) const
{
    basicMetrics_[year].serializeLifeRelatedData(stream, separator);
}

void AverageData::writeFamiliesMetricData(std::ostream& stream,
                                          std::size_t year) const
{
    getBasicMetrics(year).serializeFamily(stream);
}

void AverageData::writeBitDistributionData(std::ostream& stream,
                                           std::size_t bit) const
{
    stream << ageDistribution_[bit];
}

bool AverageData::isSingleFamily(std::size_t year) const
{
    return getBasicMetrics(year).families_ <= 1;
}

void AverageData::integrateBasicMetrics(const SimulationData& data)
{
    const std::size_t years{getYears()};
    for (std::size_t i{0}; i < years; ++i)
    {
        const SimulationData::BasicMetrics<int>& other{data.getBasicMetrics(i)};

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

    const int bits{getBits()};
    for (std::size_t i{0}; i < bits; ++i)
    {
        const float newBitsValue{getBitsDistributionValue(i) +
                                 data.getBitsDistributionValue(i)};
        setBitsDistributionValue(i, newBitsValue);

        ageDistribution_[i] += static_cast<float>(ageDistribution[i]);

        const float newDeathsValue{getDeathsDistributionValue(i) +
                                   data.getDeathsDistributionValue(i)};
        setDeathsDistributionValue(i, newDeathsValue);
    }
}

void AverageData::finalizeBasicMetrics()
{
    const float simulationsAsFloat{static_cast<float>(simulations_)};
    const std::size_t years{getYears()};
    for (std::size_t i{0}; i < years; ++i)
        basicMetrics_[i].families_ /= simulationsAsFloat;

    for (std::size_t i{0}; i < years; ++i)
    {
        basicMetrics_[i].livingAtStart_ /= simulationsAsFloat;
        basicMetrics_[i].births_ /= simulationsAsFloat;
        basicMetrics_[i].deaths_ /= simulationsAsFloat;
    }
}

void AverageData::finalizeDistributions()
{
    const float simulationsAsFloat{static_cast<float>(simulations_)};
    const int bits{getBits()};
    for (std::size_t i{0}; i < bits; ++i)
    {
        const float newDeathsValue{getDeathsDistributionValue(i) /
                                   simulationsAsFloat};
        setDeathsDistributionValue(i, newDeathsValue);
        const float newBitsValue{getBitsDistributionValue(i) /
                                 simulationsAsFloat};
        setBitsDistributionValue(i, newBitsValue);
        ageDistribution_[i] /= simulationsAsFloat;
    }
}

const AverageData::BasicMetrics<float>& AverageData::getBasicMetrics(
    std::size_t year) const
{
    return basicMetrics_[year];
}
