#pragma once

#include <array>
#include <cstdio>
#include <string>

#include "Config.h"

class Generator;

class Individual
{
public:
    int wiek{0};
    int ilosc_1{0};
    int przodek{0};

    std::array<int, Config::intCount_> ciag{0, 0};

    void ageByOneYear(unsigned int miejsce);

    std::string asBitString();

    Individual offspring();

    void assignRandomBits(Generator& generator, int startingMutations);
};
