#include "ResultsData.h"

#include "Config.h"

ResultsData::ResultsData(std::size_t years) : years_(years)
{
    deathsDistribution_.resize(Config::Params::bits_, 0);
    bitsDistribution_.resize(Config::Params::bits_, 0);
}

void ResultsData::saveBitsDistibution(std::ostream& stream,
                                      char separator) const
{
    stream << "Bit" << separator << "Percent" << std::endl;
    for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
        stream << bit << separator << getBitsDistributionValue(bit)
               << std::endl;
}

void ResultsData::saveDeathsDistibution(std::ostream& stream,
                                        char separator) const
{
    stream << "Bit" << separator << "Percent" << std::endl;
    for (std::size_t bit{0}; bit < Config::Params::bits_; bit++)
        stream << bit << separator << getDeathsDistributionValue(bit)
               << std::endl;
}

void ResultsData::saveBasicMetrics(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Living_start" << separator << "Births"
           << separator << "Living_end" << separator << "Deaths" << std::endl;
    for (size_t year{0}; year < getYears(); ++year)
    {
        stream << year << separator;
        writeLifeRelatedMetricData(stream, year, separator);
        stream << std::endl;
    }
}

void ResultsData::saveFamilies(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Families" << std::endl;
    for (size_t year{0}; year < static_cast<std::size_t>(getYears()); ++year)
    {
        if (isSingleFamily(year))
            return;
        stream << year << separator;
        writeFamiliesMetricData(stream, year);
        stream << std::endl;
    }
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
