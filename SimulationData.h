#pragma once

#include <array>

#include "Common.h"

struct SimulationData
{
public:
    std::array<float, WIELKOSC * INT_W> gompertz;
    std::array<float, MAX_POP_LAT> rodziny;
    std::array<float, WIELKOSC * INT_W> bity;
    std::array<float, WIELKOSC * INT_W> wiek;
    float stat[MAX_POP_LAT][4];
};
