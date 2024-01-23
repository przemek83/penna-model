#pragma once

#include <array>
#include <vector>

#include "Config.h"

namespace SimulationData
{
struct AvgData
{
public:
    std::array<float, Config::bits_> gompertz;
    std::vector<float> rodziny;
    std::array<float, Config::bits_> bity;
    std::array<float, Config::bits_> wiek;

    std::vector<float> livingAtStart_;
    std::vector<float> births_;
    std::vector<float> livingAtEnd_;
    std::vector<float> deaths_;
};

AvgData prepareAvgData(int years);

void prepareFinalResults(float simulationCount, int maxPopulationAge,
                         SimulationData::AvgData& simulationData);
};  // namespace SimulationData
