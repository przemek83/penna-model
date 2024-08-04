#include "ResultsData.h"

ResultsData::ResultsData(std::size_t years, int bits)
    : years_(years), bits_(bits)
{
    deathsDistribution_.resize(bits_, 0);
    bitsDistribution_.resize(bits_, 0);
}

void ResultsData::saveBitsDistibution(std::ostream& stream,
                                      char separator) const
{
    stream << "Bit" << separator << "Percent\n";
    for (std::size_t bit{0}; bit < bits_; ++bit)
        stream << bit << separator << getBitsDistributionValue(bit) << '\n';
}

void ResultsData::saveDeathsDistibution(std::ostream& stream,
                                        char separator) const
{
    stream << "Bit" << separator << "Percent\n";
    for (std::size_t bit{0}; bit < bits_; ++bit)
        stream << bit << separator << getDeathsDistributionValue(bit) << '\n';
}

void ResultsData::saveBasicMetrics(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Living_start" << separator << "Births"
           << separator << "Living_end" << separator << "Deaths\n";
    for (size_t year{0}; year < years_; ++year)
    {
        stream << year << separator;
        writeLifeRelatedMetricData(stream, year, separator);
        stream << '\n';
    }
}

void ResultsData::saveFamilies(std::ostream& stream, char separator) const
{
    stream << "Year" << separator << "Families\n";
    for (size_t year{0}; year < years_; ++year)
    {
        if (isSingleFamily(year))
            return;
        stream << year << separator;
        writeFamiliesMetricData(stream, year);
        stream << '\n';
    }
}

void ResultsData::saveAgeDistibution(std::ostream& stream, char separator) const
{
    stream << "Age" << separator << "Count\n";
    for (std::size_t bit{0}; bit < bits_; ++bit)
    {
        stream << bit << separator;
        writeBitDistributionData(stream, bit);
        stream << '\n';
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

std::size_t ResultsData::getYears() const { return years_; }

std::size_t ResultsData::getBits() const { return bits_; }
