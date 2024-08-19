#pragma once

#include <ostream>
#include <vector>

class Results
{
public:
    Results(std::size_t years, int bits);
    virtual ~Results() = default;

    Results(const Results&) = default;
    Results(Results&&) = default;
    Results& operator=(const Results&) = default;
    Results& operator=(Results&&) = default;

    template <typename T>
    struct BasicMetrics
    {
        T getLivingAtEnd() const { return livingAtStart_ - deaths_ + births_; }

        void serializeLifeRelatedData(std::ostream& stream,
                                      char separator) const
        {
            stream << livingAtStart_ << separator << births_ << separator
                   << getLivingAtEnd() << separator << deaths_;
        }

        void serializeFamily(std::ostream& stream) const
        {
            stream << families_;
        }

        T families_{0};
        T livingAtStart_{0};
        T births_{0};
        T deaths_{0};
    };

    void saveBitsDistibution(std::ostream& stream, char separator) const;
    void saveDeathsDistibution(std::ostream& stream, char separator) const;
    void saveBasicMetrics(std::ostream& stream, char separator) const;
    void saveFamilies(std::ostream& stream, char separator) const;
    void saveAgeDistibution(std::ostream& stream, char separator) const;

    float getDeathsDistributionValue(std::size_t index) const;
    float getBitsDistributionValue(std::size_t index) const;

protected:
    virtual void writeLifeRelatedMetricData(std::ostream& stream,
                                            std::size_t year,
                                            char separator) const = 0;

    virtual void writeFamiliesMetricData(std::ostream& stream,
                                         std::size_t year) const = 0;

    virtual void writeBitDistributionData(std::ostream& stream,
                                          std::size_t bit) const = 0;

    void setDeathsDistributionValue(std::size_t index, float value);
    void setBitsDistributionValue(std::size_t index, float value);

    std::size_t getYears() const;

    std::size_t getBits() const;

    virtual bool isSingleFamily(std::size_t year) const = 0;

private:
    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;

    std::size_t years_;
    std::size_t bits_;
};
