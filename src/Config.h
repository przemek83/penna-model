#pragma once

#include <istream>

namespace config
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
    int initial_{3'000};
    int max_{50'000};
};

struct Catching
{
    int percent_{0};
    int fromYear_{1'000};
    int fromAge_{4};
};

struct Params
{
    Population population_;
    int years_{5'000};
    Mutations mutations_;
    int reproductionAge_{10};
    Offspring offspring_;
    int simulationsCount_{2};
    static constexpr int bits_{
#ifdef GENOME_BITS
        GENOME_BITS
#else
        64
#endif
    };
    Catching catching_;
};

static_assert(Params::bits_ > 0);

config::Params loadConfig(std::istream& stream);

bool isValid(const Params& params);

std::tuple<bool, std::string, std::string> getAppArguments(int argc,
                                                           char* argv[]);

std::pair<bool, config::Params> getParams(const std::string& configFileName);

}  // namespace config
