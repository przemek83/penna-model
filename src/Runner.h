#pragma once

#include "Simulation.h"

class Runner
{
public:
    void addSimulation(Simulation simulation);

    [[maybe_unused]] std::vector<SimulationData> runSequential();

    std::vector<SimulationData> runParallel();

private:
    std::vector<Simulation> simulations_;
};
