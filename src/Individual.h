#pragma once

#include <bitset>
#include <string>

#include "Config.h"

class Generator;

class Individual
{
public:
    explicit Individual(int ancestor);

    int getAge() const { return age_; }

    int getSurvivedMutations() const { return survivedMutations_; }

    int getAncestor() const { return ancestor_; }

    void ageByOneYear();

    std::string asBitString() const;

    Individual offspring(Generator& generator, int mutations) const;

    void assignRandomBits(Generator& generator, int startingMutations);

    bool getGenomeBit(std::size_t position) const { return genome_[position]; }

    bool shouldDie(int lethalMutations) const;

private:
    void applyMutation(Generator& generator);

    std::bitset<config::Params::bits_> genome_;

    int age_{0};

    int survivedMutations_{0};

    int ancestor_;
};
