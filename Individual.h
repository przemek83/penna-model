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
    int age_{0};
    int survivedOnes_{0};
    int ancestor_{0};

    std::bitset<Config::bits_> genome_;

    void ageByOneYear();

    std::string asBitString() const;

    Individual offspring() const;

    void applyMutation(Generator& generator);

    void assignRandomBits(Generator& generator, int startingMutations);
};
