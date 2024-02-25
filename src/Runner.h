#pragma once

#include "Simulation.h"

class Runner
{
public:
    void addSimulation(Simulation simulation);

    std::vector<SingleSimulationData> runSequential();

    std::vector<SingleSimulationData> runParallel();

private:
    std::vector<Simulation> simulations_;
};
