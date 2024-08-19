#pragma once

#include "Simulation.h"

class Runner
{
public:
    void prepareSimulations(const config::Params& params, long int seed);

    [[maybe_unused]] std::vector<SimResults> runSequential();

    std::vector<SimResults> runParallel();

private:
    std::vector<Simulation> simulations_;
};
