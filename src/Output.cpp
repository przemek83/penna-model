#include "Output.h"

#include <filesystem>
#include <iostream>

Output::Output(int years) : years_{years} {}

void Output::saveSimulationData(const SingleSimulationData& data)
{
    saveData<SingleSimulationData>(
        data, {{FAMILIES, 0}, {STATISTICS, 0}, {AGE_DISTRIBUTION, 0}});
}

void Output::saveAverages(const SimulationAverages& data)
{
    const int floatPrecision{6};
    saveData<SimulationAverages>(data, {{FAMILIES, floatPrecision},
                                        {STATISTICS, floatPrecision},
                                        {AGE_DISTRIBUTION, floatPrecision}});
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
    *file << "Id" << separator_ << "Ancestor" << separator_ << "Age"
          << separator_ << "Survived_mutations" << separator_
          << "Genome_as_number" << separator_ << "Genome" << std::endl;
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
