#pragma once

#include <array>
#include <vector>

#include "Config.h"

template <typename T>
struct SimulationData
{
public:
    std::array<float, Config::bits_> gompertz;
    std::array<float, Config::bits_> bity;
    std::array<T, Config::bits_> wiek;

    std::vector<T> rodziny;
    std::vector<T> livingAtStart_;
    std::vector<T> births_;
    std::vector<T> livingAtEnd_;
    std::vector<T> deaths_;
};

using SimulationAverages = SimulationData<float>;
using SingleSimulationData = SimulationData<int>;

template <typename T>
SimulationData<T> prepareSimulationData(int years)
{
    SimulationData<T> data;
    data.gompertz.fill(0);
    data.bity.fill(0);
    data.wiek.fill(0);
    data.rodziny.resize(static_cast<std::size_t>(years), 0);
    data.livingAtStart_.resize(static_cast<std::size_t>(years), 0);
    data.births_.resize(static_cast<std::size_t>(years), 0);
    data.livingAtEnd_.resize(static_cast<std::size_t>(years), 0);
    data.deaths_.resize(static_cast<std::size_t>(years), 0);

    return data;
}

void prepareFinalResults(float simulationCount, int maxPopulationAge,
                         SimulationAverages& simulationData);

void integrateData(SimulationAverages& simulationAverages,
                   const SingleSimulationData& singleSimulationData);
