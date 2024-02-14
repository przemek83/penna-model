#include "Output.h"

#include <filesystem>
#include <iomanip>
#include <iostream>

Output::Output(float simulationStep, int maxPopulationAge, int run)
    : simulationStep_{simulationStep},
      maxPopulationAge_{maxPopulationAge},
      run_{run}
{
}

void Output::saveSimulationData(const SingleSimulationData& data)
{
    const std::shared_ptr<std::ostream> families{getStream(FAMILIES)};
    data.saveFamilies(*families, separator_);

    const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};
    data.saveBasicMetrics(*stats, separator_);

    const std::shared_ptr<std::ostream> bits{getStream(BITS_DISTRIBUTION)};
    *bits << std::setprecision(2) << std::fixed;
    data.saveBitsDistibution(*bits, separator_);

    const std::shared_ptr<std::ostream> age{getStream(AGE_DISTRIBUTION)};
    data.saveAgeDistibution(*age, separator_);

    const std::shared_ptr<std::ostream> deaths{getStream(DEATHS_DISTRIBUTION)};
    *deaths << std::setprecision(3) << std::fixed;
    data.saveDeathsDistibution(*deaths, separator_);
}

void Output::saveAverages(const SimulationAverages& data)
{
    const std::shared_ptr<std::ostream> families{getStream(FAMILIES)};
    *families << std::setprecision(6) << std::fixed;
    data.saveFamilies(*families, separator_);

    const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};
    *stats << std::setprecision(6) << std::fixed;
    data.saveBasicMetrics(*stats, separator_);

    const std::shared_ptr<std::ostream> bits{getStream(BITS_DISTRIBUTION)};
    *bits << std::setprecision(2) << std::fixed;
    data.saveBitsDistibution(*bits, separator_);

    const std::shared_ptr<std::ostream> age{getStream(AGE_DISTRIBUTION)};
    *age << std::setprecision(6) << std::fixed;
    data.saveAgeDistibution(*age, separator_);

    const std::shared_ptr<std::ostream> deaths{getStream(DEATHS_DISTRIBUTION)};
    *deaths << std::setprecision(3) << std::fixed;
    data.saveDeathsDistibution(*deaths, separator_);
}

void Output::saveInitialPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    const std::shared_ptr<std::ostream> file{getStream(INITIAL_POPULATION)};
    *file << "Id" << separator_ << "Genome" << std::endl;
    for (const auto& individual : individuals)
    {
        *file << counter << separator_ << individual.asBitString() << std::endl;
        counter++;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals)
{
    int counter{0};
    const std::shared_ptr<std::ostream> file{getStream(FINAL_POPULATION)};
    for (const auto& individual : individuals)
    {
        *file << counter << separator_ << individual.getAncestor() << separator_
              << individual.getAge() << separator_
              << individual.getSurvivedMutations() << separator_
              << individual.getGenomeAsNumber() << separator_
              << individual.asBitString() << std::endl;
        counter++;
    }
}

int Output::getRunNumber() const { return run_; }
