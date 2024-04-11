#include "ResultsData.h"

#include "Config.h"

ResultsData::ResultsData(std::size_t years) : years_(years)
{
    deathsDistribution_.resize(Config::Params::bits_, 0);
    bitsDistribution_.resize(Config::Params::bits_, 0);
}

float ResultsData::getDeathsDistributionValue(std::size_t index) const
{
    return deathsDistribution_[index];
}

void ResultsData::setDeathsDistributionValue(std::size_t index, float value)
{
    deathsDistribution_[index] = value;
}

float ResultsData::getBitsDistributionValue(std::size_t index) const
{
    return bitsDistribution_[index];
}

void ResultsData::setBitsDistributionValue(std::size_t index, float value)
{
    bitsDistribution_[index] = value;
}
