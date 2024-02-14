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
    data.saveFamilies(families, separator_);

    const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};
    data.saveBasicMetrics(stats, separator_);

    saveDeathsDistribution(data.getDeathsDistribution());
    saveBitsDistribution(data.getBitsDistribution());
    saveAgeDistribution(data.getAgeDistribution());
}

void Output::saveAverages(const SimulationAverages& data)
{
    const std::shared_ptr<std::ostream> families{getStream(FAMILIES)};
    *families << std::setprecision(6) << std::fixed;
    data.saveFamilies(families, separator_);

    const std::shared_ptr<std::ostream> stats{getStream(STATISTICS)};
    *stats << std::setprecision(6) << std::fixed;
    data.saveBasicMetrics(stats, separator_);

    const std::shared_ptr<std::ostream> ages{getStream(AGE_DISTRIBUTION)};
    *ages << std::setprecision(6) << std::fixed;
    const std::shared_ptr<std::ostream> bits{getStream(BITS_DISTRIBUTION)};
    *bits << std::setprecision(2) << std::fixed;
    const std::shared_ptr<std::ostream> deaths{getStream(DEATHS_DISTRIBUTION)};
    *deaths << std::setprecision(3) << std::fixed;

    const std::vector<float>& deathsDistribution{data.getDeathsDistribution()};
    const std::vector<float>& bitsDistribution{data.getBitsDistribution()};
    const std::vector<float>& ageDistribution{data.getAgeDistribution()};

    for (size_t i{0}; i < Config::bits_; i++)
    {
        *ages << i << separator_ << ageDistribution[i] << std::endl;
        *bits << i << separator_ << bitsDistribution[i] << std::endl;

        if (deathsDistribution[i] > 0)
            *deaths << i << separator_ << deathsDistribution[i] << std::endl;
        else
            *deaths << i << separator_ << 1 << std::endl;
    }
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

void Output::saveBitsDistribution(const std::vector<float>& bitsDistribution)
{
    const std::shared_ptr<std::ostream> file{getStream(BITS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        *file << i << separator_ << std::setprecision(2) << std::fixed
              << bitsDistribution[i] << std::endl;
}

void Output::saveAgeDistribution(const std::vector<int>& ageDistribution)
{
    const std::shared_ptr<std::ostream> file{getStream(AGE_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
        *file << i << separator_ << ageDistribution[i] << std::endl;
}

void Output::saveDeathsDistribution(const std::vector<float>& deaths)
{
    const std::shared_ptr<std::ostream> file{getStream(DEATHS_DISTRIBUTION)};
    for (std::size_t i{0}; i < Config::bits_; i++)
    {
        *file << i << separator_ << std::setprecision(3) << std::fixed
              << deaths[i] << std::endl;
    }
}
