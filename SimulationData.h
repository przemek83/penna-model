#pragma once

#include <array>
#include <vector>

#include "Config.h"

template <typename T>
struct SimulationData
{
public:
    std::array<T, Config::bits_> gompertz;
    std::vector<T> rodziny;
    std::array<T, Config::bits_> bity;
    std::array<T, Config::bits_> wiek;

    std::vector<T> livingAtStart_;
    std::vector<T> births_;
    std::vector<T> livingAtEnd_;
    std::vector<T> deaths_;
};

using SimulationAverages = SimulationData<float>;

SimulationAverages prepareSimulationAverages(int years);

void prepareFinalResults(float simulationCount, int maxPopulationAge,
                         SimulationAverages& simulationData);
