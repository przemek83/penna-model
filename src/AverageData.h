#pragma once

#include <ostream>
#include <vector>

#include "ResultsData.h"
#include "SimulationData.h"

class AverageData : public ResultsData
{
public:
    explicit AverageData(std::size_t years);

    void saveFamilies(std::ostream& stream, char separator) const;
    void saveAgeDistibution(std::ostream& stream, char separator) const;

    void integrateData(const SimulationData& data);

    void finalize();

protected:
    void serializeLifeRelatedMetricData(std::size_t year, std::ostream& stream,
                                        char separator) const override;

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
