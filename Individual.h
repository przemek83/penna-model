#pragma once

#include <array>
#include <cstdio>

#include "Config.h"

class Generator;

class Individual
{
public:
    int wiek{0};
    int ilosc_1{0};
    int przodek{0};

    std::array<int, Config::intCount_> ciag{0, 0};

    void czy1(unsigned int miejsce);

    void itob(FILE* plik);

    void inicjuj(unsigned int a, std::array<int, Config::intCount_>& bity);

    void assignRandomBits(Generator& generator, int startingMutations);
};
