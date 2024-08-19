#include "AvgResults.h"

#include "Config.h"

AvgResults::AvgResults(const std::vector<SimResults>& simulationsData,
                       std::size_t years, int bits)
    : Results(years, bits)
{
    ageDistribution_.resize(getBits(), 0);
    basicMetrics_.resize(getYears());

    for (const auto& data : simulationsData)
        integrate(data);

    finalize();
}

void AvgResults::integrate(const SimResults& data)
{
    integrateBasicMetrics(data);
    integrateDistributions(data);
    ++simulations_;
}

void AvgResults::finalize()
{
    finalizeBasicMetrics();
    finalizeDistributions();
}

void AvgResults::writeLifeRelatedMetricData(std::ostream& stream,
                                            std::size_t year,
                                            char separator) const
{
    basicMetrics_[year].serializeLifeRelatedData(stream, separator);
}

void AvgResults::writeFamiliesMetricData(std::ostream& stream,
                                         std::size_t year) const
{
    getBasicMetrics(year).serializeFamily(stream);
}

void AvgResults::writeBitDistributionData(std::ostream& stream,
                                          std::size_t bit) const
{
    stream << ageDistribution_[bit];
}

bool AvgResults::isSingleFamily(std::size_t year) const
{
    return getBasicMetrics(year).families_ <= 1;
}

void AvgResults::integrateBasicMetrics(const SimResults& data)
{
    const std::size_t years{getYears()};
    for (std::size_t i{0}; i < years; ++i)
    {
        const SimResults::BasicMetrics<int>& other{data.getBasicMetrics(i)};

        basicMetrics_[i].families_ += static_cast<float>(other.families_);
        basicMetrics_[i].livingAtStart_ +=
            static_cast<float>(other.livingAtStart_);
        basicMetrics_[i].births_ += static_cast<float>(other.births_);
        basicMetrics_[i].deaths_ += static_cast<float>(other.deaths_);
    }
}

void AvgResults::integrateDistributions(const SimResults& data)
{
    const std::vector<int>& ageDistribution{data.getAgeDistribution()};

    const std::size_t bits{getBits()};
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

void AvgResults::finalizeBasicMetrics()
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

void AvgResults::finalizeDistributions()
{
    const float simulationsAsFloat{static_cast<float>(simulations_)};
    const std::size_t bits{getBits()};
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

const AvgResults::BasicMetrics<float>& AvgResults::getBasicMetrics(
    std::size_t year) const
{
    return basicMetrics_[year];
}
