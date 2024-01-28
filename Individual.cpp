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

Individual Individual::offspring() const
{
    Individual offspring{*this};
    offspring.age_ = 0;
    offspring.survivedMutations_ = 0;
    return offspring;
}

void Individual::applyMutation(Generator& generator)
{
    const int mutationPosition{generator.getInt(0, Config::bits_ - 1)};
    genome_[mutationPosition] = true;
}

void Individual::assignRandomBits(Generator& generator, int startingMutations)
{
    std::set<int> mutationPositions;
    while (mutationPositions.size() < startingMutations)
        mutationPositions.insert(generator.getInt(0, Config::bits_ - 1));

    for (const auto mutationPosition : mutationPositions)
        genome_[mutationPosition] = true;
}

unsigned long long Individual::getGenomeAsNumber() const
{
    return genome_.to_ullong();
}

void Individual::setGenome(const std::bitset<Config::bits_>& genome)
{
    genome_ = genome;
}
