#pragma once

#include <vector>

class ResultsData
{
public:
    explicit ResultsData(std::size_t years);

    template <typename T>
    struct BasicMetrics
    {
        inline T getLivingAtEnd() const
        {
            return livingAtStart_ - deaths_ + births_;
        }

        T families_{0};
        T livingAtStart_{0};
        T births_{0};
        T deaths_{0};
    };

    float getDeathsDistributionValue(std::size_t index) const;
    float getBitsDistributionValue(std::size_t index) const;

protected:
    void setDeathsDistributionValue(std::size_t index, float value);
    void setBitsDistributionValue(std::size_t index, float value);

    std::size_t getYears() const { return years_; }

private:
    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;

    std::size_t years_;
};
