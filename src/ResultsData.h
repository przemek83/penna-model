#pragma once

#include <ostream>
#include <vector>

class ResultsData
{
public:
    explicit ResultsData(std::size_t years);
    virtual ~ResultsData() = default;

    template <typename T>
    struct BasicMetrics
    {
        inline T getLivingAtEnd() const
        {
            return livingAtStart_ - deaths_ + births_;
        }

        void serializeLifeRelatedData(std::ostream& stream,
                                      char separator) const
        {
            stream << livingAtStart_ << separator << births_ << separator
                   << getLivingAtEnd() << separator << deaths_;
        }

        void serializeFamily(std::ostream& stream, char separator) const
        {
            stream << livingAtStart_ << separator << births_ << separator
                   << getLivingAtEnd() << separator << deaths_;
        }

        T families_{0};
        T livingAtStart_{0};
        T births_{0};
        T deaths_{0};
    };

    void saveBitsDistibution(std::ostream& stream, char separator) const;
    void saveDeathsDistibution(std::ostream& stream, char separator) const;
    void saveBasicMetrics(std::ostream& stream, char separator) const;

    float getDeathsDistributionValue(std::size_t index) const;
    float getBitsDistributionValue(std::size_t index) const;

protected:
    virtual void writeLifeRelatedMetricData(std::ostream& stream,
                                            std::size_t year,
                                            char separator) const = 0;

    void setDeathsDistributionValue(std::size_t index, float value);
    void setBitsDistributionValue(std::size_t index, float value);

    std::size_t getYears() const { return years_; }

private:
    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;

    std::size_t years_;
};
