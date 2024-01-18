#pragma once

#include <array>
#include <cstdio>

#include "Config.h"

class Individual
{
public:
    int wiek;
    int ilosc_1;
    int przodek;

    std::array<int, Config::intCount_> ciag;

    void czy1(unsigned int miejsce);

    void itob(FILE* plik);

    void inicjuj(unsigned int a, std::array<int, Config::intCount_>& bity);
};
