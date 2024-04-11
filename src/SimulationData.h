#pragma once

#include <ostream>
#include <vector>

#include "ResultsData.h"

class SimulationData : public ResultsData
{
public:
    explicit SimulationData(std::size_t years);

    const BasicMetrics<int>& getBasicBasicMetrics(std::size_t year) const;
    void setBasicMetrics(std::vector<BasicMetrics<int>> basicBasicMetrics);

    void setDeathDistribution(
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution);

    void setBitDistribution(const std::vector<int>& bitsDistribution,
                            int populationCount);

    const std::vector<int>& getAgeDistribution() const;
    void setAgeDistribution(const std::vector<int>& ageDistribution);

    void saveFamilies(std::ostream& stream, char separator) const;
    void saveAgeDistibution(std::ostream& stream, char separator) const;

protected:
    void serializeLifeRelatedMetricData(std::size_t year, std::ostream& stream,
                                        char separator) const override;

private:
    std::vector<int> ageDistribution_;
    std::vector<BasicMetrics<int>> basicMetrics_;
};
