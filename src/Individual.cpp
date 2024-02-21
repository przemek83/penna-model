#include "Individual.h"

#include <cstdlib>
#include <cstring>
#include <set>

#include "Generator.h"

Individual::Individual(int ancestor) : ancestor_{ancestor} {}

void Individual::ageByOneYear()
{
    if (genome_[age_])
        survivedMutations_++;
    age_++;
}

std::string Individual::asBitString() const { return genome_.to_string(); }

Individual Individual::offspring(Generator& generator, int mutations) const
{
    Individual offspring{*this};
    offspring.age_ = 0;
    offspring.survivedMutations_ = 0;

    for (int i{0}; i < mutations; i++)
        offspring.applyMutation(generator);

    return offspring;
}

void Individual::applyMutation(Generator& generator)
{
    const int mutationPosition{generator.getBitPosition()};
    genome_[mutationPosition] = true;
}

void Individual::assignRandomBits(Generator& generator, int startingMutations)
{
    std::set<int> mutationPositions;
    while (mutationPositions.size() < startingMutations)
        mutationPositions.insert(generator.getBitPosition());

    for (const auto mutationPosition : mutationPositions)
        genome_[mutationPosition] = true;
}

unsigned long long Individual::getGenomeAsNumber() const
{
    return genome_.to_ullong();
}

void Individual::setGenome(const std::bitset<Config::Params::bits_>& genome)
{
    genome_ = genome;
}
