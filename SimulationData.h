#pragma once

#include <array>
#include <vector>

#include "Config.h"

struct SimulationData
{
public:
    std::array<float, Config::bits_> gompertz;
    std::vector<float> rodziny;
    std::array<float, Config::bits_> bity;
    std::array<float, Config::bits_> wiek;

    std::vector<float> livingAtStart_;  // 0
    std::vector<float> births_;         // 1
    std::vector<float> livingAtEnd_;    // ilosc_osobnikow - zgon; //2
    std::vector<float> deaths_;         // 3

    //    std::vector<std::vector<float>> stat;
};
