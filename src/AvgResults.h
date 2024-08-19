#pragma once

#include <ostream>
#include <vector>

#include "Results.h"
#include "SimResults.h"

class AvgResults : public Results
{
public:
    AvgResults(const std::vector<SimResults>& simulationsData,
               std::size_t years, int bits);

protected:
    void writeLifeRelatedMetricData(std::ostream& stream, std::size_t year,
                                    char separator) const override;

    void writeFamiliesMetricData(std::ostream& stream,
                                 std::size_t year) const override;

    void writeBitDistributionData(std::ostream& stream,
                                  std::size_t bit) const override;

    bool isSingleFamily(std::size_t year) const override;

private:
    void integrate(const SimResults& data);
    void finalize();

    void integrateBasicMetrics(const SimResults& data);
    void integrateDistributions(const SimResults& data);

    void finalizeBasicMetrics();
    void finalizeDistributions();

    const BasicMetrics<float>& getBasicMetrics(std::size_t year) const;

    int simulations_{0};

    std::vector<BasicMetrics<float>> basicMetrics_;

    std::vector<float> ageDistribution_;
};
