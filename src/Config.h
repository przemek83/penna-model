#pragma once

#include <istream>

namespace Config
{
struct Mutations
{
    int added_{1};
    int lethal_{4};
    int initial_{4};
};

struct Offspring
{
    int chance_{100};
    int count_{3};
};

struct Population
{
    int initial_{3000};
    int max_{50000};
};

struct Catching
{
    int percent_{0};
    int fromYear_{1000};
    int fromAge_{4};
};

struct Params
{
    Population population_;
    int years_{5000};
    Mutations mutations_;
    int reproductionAge_{10};
    Offspring offspring_;
    int simulationsCount_{2};
    static constexpr int bits_{64};
    Catching catching_;
};

static_assert(Params::bits_ > 0);

Config::Params loadConfig(std::istream& stream);

bool isValid(const Params& params);

std::string getConfigFileName(int argc, char* argv[]);

Config::Params getParams(const std::string& configFileName);

}  // namespace Config
