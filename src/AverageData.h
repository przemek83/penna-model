#pragma once

#include <ostream>
#include <vector>

#include "ResultsData.h"
#include "SimulationData.h"

class AverageData : public ResultsData
{
public:
    explicit AverageData(std::size_t years);

    void integrateData(const SimulationData& data);

    void finalize();

protected:
    void writeLifeRelatedMetricData(std::ostream& stream, std::size_t year,
                                    char separator) const override;

    void writeFamiliesMetricData(std::ostream& stream,
                                 std::size_t year) const override;

    void writeBitDistributionData(std::ostream& stream,
                                  std::size_t bit) const override;

    bool isSingleFamily(std::size_t year) const override;

private:
    void integrateBasicMetrics(const SimulationData& data);
    void integrateDistributions(const SimulationData& data);

    void finalizeBasicMetrics();
    void finalizeDistributions();

    const BasicMetrics<float>& getBasicMetrics(std::size_t year) const;

    int simulations_{0};

    std::vector<BasicMetrics<float>> basicMetrics_;

    std::vector<float> ageDistribution_;
};
