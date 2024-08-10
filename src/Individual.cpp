#include "Individual.h"

#include <cstdlib>
#include <cstring>
#include <set>

#include "Generator.h"

Individual::Individual(int ancestor) : ancestor_{ancestor} {}

void Individual::ageByOneYear()
{
    if (genome_[static_cast<std::size_t>(age_)])
        ++survivedMutations_;
    ++age_;
}

std::string Individual::asBitString() const { return genome_.to_string(); }

Individual Individual::offspring(Generator& generator, int mutations) const
{
    Individual offspring{*this};
    offspring.age_ = 0;
    offspring.survivedMutations_ = 0;

    for (int i{0}; i < mutations; ++i)
        offspring.applyMutation(generator);

    return offspring;
}

void Individual::applyMutation(Generator& generator)
{
    const std::size_t mutationPosition{generator.getBitPosition()};
    genome_[mutationPosition] = true;
}

void Individual::assignRandomBits(Generator& generator, int startingMutations)
{
    std::set<std::size_t> mutationPositions;
    while (mutationPositions.size() <
           static_cast<std::size_t>(startingMutations))
        mutationPositions.insert(generator.getBitPosition());

    for (const auto mutationPosition : mutationPositions)
        genome_[mutationPosition] = true;
}

bool Individual::shouldDie(int lethalMutations) const
{
    return (getSurvivedMutations() >= lethalMutations) ||
           (getAge() >= config::Params::bits_);
}
