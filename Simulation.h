#pragma once

#include <random>

#include "Common.h"
#include "Individual.h"

class Output;
class Generator;
class SimulationData;

class Simulation
{
public:
    Simulation(int number, float step);

    void run(Output& output, Generator& generator, SimulationData& simulationDataAvg);

private:
    int losuj_populacje(Output& wyjscie, Generator& generator);

    Individual individuals_[MAX_POP];

    const int number_;
    const float step_;
};
