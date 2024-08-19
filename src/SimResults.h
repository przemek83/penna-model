#pragma once

#include <ostream>
#include <vector>

#include "Results.h"

class SimResults : public Results
{
public:
    SimResults(std::size_t years, int bits);

    const BasicMetrics<int>& getBasicMetrics(std::size_t year) const;
    void setBasicMetrics(std::vector<BasicMetrics<int>> basicBasicMetrics);

    void setDeathDistribution(
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution);

    void setBitDistribution(const std::vector<int>& bitsDistribution,
                            int populationCount);

    const std::vector<int>& getAgeDistribution() const;
    void setAgeDistribution(const std::vector<int>& ageDistribution);

protected:
    void writeLifeRelatedMetricData(std::ostream& stream, std::size_t year,
                                    char separator) const override;

    void writeFamiliesMetricData(std::ostream& stream,
                                 std::size_t year) const override;

    void writeBitDistributionData(std::ostream& stream,
                                  std::size_t bit) const override;

    bool isSingleFamily(std::size_t year) const override;

private:
    float calculateBitDistributionValue(int populationCount,
                                        int bitsDistributionValue) const;

    std::vector<int> ageDistribution_;
    std::vector<BasicMetrics<int>> basicMetrics_;
};
