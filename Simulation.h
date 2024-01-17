#pragma once

#include <random>
#include <vector>

#include "Config.h"
#include "Individual.h"
#include "SimulationData.h"

class Output;
class Generator;

class Simulation
{
public:
    Simulation(const Config& config, int number, float step);

    void run(Output& output, Generator& generator,
             SimulationData::AvgData& simulationDataAvg);

private:
    int losuj_populacje(Output& wyjscie, Generator& generator);

    std::vector<Individual> individuals_;

    const Config& config_;
    const int number_;
    const float step_;
};
