#include "Output.h"

#include <filesystem>
#include <iostream>

#include "AverageData.h"

void Output::saveSimulationData(const SimulationData& data)
{
    saveData<SimulationData>(
        data, {{FAMILIES, 0}, {STATISTICS, 0}, {AGE_DISTRIBUTION, 0}});
}

void Output::saveAverages(const AverageData& data)
{
    const int floatPrecision{6};
    saveData<AverageData>(data, {{FAMILIES, floatPrecision},
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
          << separator_ << "Survived_mutations" << separator_ << "Genome"
          << std::endl;
    for (const auto& individual : individuals)
    {
        *file << counter << separator_ << individual.getAncestor() << separator_
              << individual.getAge() << separator_
              << individual.getSurvivedMutations() << separator_
              << individual.asBitString() << std::endl;
        counter++;
    }
}
