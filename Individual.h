#pragma once

#include <array>
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

    std::array<int, Config::intCount_> genome_{0, 0};

    void ageByOneYear(unsigned int miejsce);

    std::string asBitString() const;

    Individual offspring() const;

    void assignRandomBits(Generator& generator, int startingMutations);
};
