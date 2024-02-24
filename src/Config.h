#pragma once

#include <istream>

namespace Config
{
struct Params
{
    int maxPopulation_{50000};
    int years_{5000};
    int livesOnStart_{3000};
    int mutationsDelta_{1};
    int maxMutations_{4};
    int startingMutations_{4};
    int reproductionAge_{10};
    int chanceForOffspring_{100};
    int offspringCount_{3};
    int simulationsCount_{2};
    static constexpr int bits_{64};
};

static_assert(Params::bits_ > 0);

Config::Params loadConfig(std::istream& stream);
}  // namespace Config
