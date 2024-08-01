#pragma once

#include <bitset>
#include <string>

#include "Config.h"

class Generator;

class Individual
{
public:
    explicit Individual(int ancestor);

    inline int getAge() const { return age_; }

    inline int getSurvivedMutations() const { return survivedMutations_; }

    inline int getAncestor() const { return ancestor_; }

    void ageByOneYear();

    std::string asBitString() const;

    Individual offspring(Generator& generator, int mutations) const;

    void assignRandomBits(Generator& generator, int startingMutations);

    inline bool getGenomeBit(std::size_t position) const
    {
        return genome_[position];
    }

    void setGenome(const std::bitset<config::Params::bits_>& genome);

private:
    void applyMutation(Generator& generator);

    std::bitset<config::Params::bits_> genome_;

    int age_{0};

    int survivedMutations_{0};

    int ancestor_;
};
