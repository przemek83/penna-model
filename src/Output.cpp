#include "Output.h"

#include <iostream>

#include "AvgResults.h"

void Output::saveSimulationData(const SimResults& data)
{
    saveData<SimResults>(data, {{OutputType::FAMILIES, 0},
                                {OutputType::STATISTICS, 0},
                                {OutputType::AGE_DISTRIBUTION, 0}});
}

void Output::saveAverages(const AvgResults& data)
{
    const int floatPrecision{6};
    saveData<AvgResults>(data,
                         {{OutputType::FAMILIES, floatPrecision},
                          {OutputType::STATISTICS, floatPrecision},
                          {OutputType::AGE_DISTRIBUTION, floatPrecision}});
}

void Output::saveInitialPopulation(
    const std::list<Individual>& individuals) const
{
    int counter{0};
    const std::shared_ptr<std::ostream> file{
        getStream(OutputType::INITIAL_POPULATION)};
    *file << "Id" << separator_ << "Genome\n";

    for (const auto& individual : individuals)
    {
        *file << counter << separator_ << individual.asBitString() << '\n';
        ++counter;
    }
}

void Output::saveFinalPopulation(const std::list<Individual>& individuals) const
{
    int counter{0};
    const std::shared_ptr<std::ostream> file{
        getStream(OutputType::FINAL_POPULATION)};
    *file << "Id" << separator_ << "Ancestor" << separator_ << "Age"
          << separator_ << "Survived_mutations" << separator_ << "Genome\n";

    for (const auto& individual : individuals)
    {
        *file << counter << separator_ << individual.getAncestor() << separator_
              << individual.getAge() << separator_
              << individual.getSurvivedMutations() << separator_
              << individual.asBitString() << '\n';
        ++counter;
    }
}
