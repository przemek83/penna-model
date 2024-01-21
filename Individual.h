#pragma once

#include <array>
#include <bitset>
#include <cstdio>
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

    std::bitset<Config::bits_> genome_;

    void ageByOneYear();

    std::string asBitString() const;

    Individual offspring() const;

    void applyMutation(Generator& generator);

    void assignRandomBits(Generator& generator, int startingMutations);

private:
    int age_{0};

    int survivedMutations_{0};

    int ancestor_;
};
