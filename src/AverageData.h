#pragma once

#include <ostream>
#include <vector>

#include "SimulationData.h"

class AverageData
{
public:
    explicit AverageData(std::size_t years);
    struct BasicMetrics
    {
        inline float getLivingAtEnd() const
        {
            return livingAtStart_ - deaths_ + births_;
        }

        float families_{0};
        float livingAtStart_{0};
        float births_{0};
        float deaths_{0};
    };

    void saveFamilies(std::ostream& stream, char separator) const;
    void saveBasicMetrics(std::ostream& stream, char separator) const;
    void saveBitsDistibution(std::ostream& stream, char separator) const;
    void saveAgeDistibution(std::ostream& stream, char separator) const;
    void saveDeathsDistibution(std::ostream& stream, char separator) const;

    void integrateData(const SimulationData& data);

    void finalize();

private:
    void integrateBasicMetrics(const SimulationData& data);
    void integrateDistributions(const SimulationData& data);

    void finalizeBasicMetrics();
    void finalizeDistributions();

    const BasicMetrics& getBasicMetrics(std::size_t year) const;

    int simulations_{0};

    std::vector<BasicMetrics> basicMetrics_;

    std::size_t years_;

    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;
    std::vector<float> ageDistribution_;
};
