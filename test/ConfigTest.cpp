#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "src/Config.h"

namespace
{
bool operator==(const Config::Catching& left, const Config::Catching& right)
{
    return left.percent_ == right.percent_ &&
           left.fromYear_ == right.fromYear_ && left.fromAge_ == right.fromAge_;
}

bool operator==(const Config::Population& left, const Config::Population& right)
{
    return left.max_ == right.max_ && left.initial_ == right.initial_;
}

bool operator==(const Config::Mutations& left, const Config::Mutations& right)
{
    return left.added_ == right.added_ && left.lethal_ == right.lethal_ &&
           left.initial_ == right.initial_;
}

bool operator==(const Config::Offspring& left, const Config::Offspring& right)
{
    return left.chance_ == right.chance_ && left.count_ == right.count_;
}

bool operator==(const Config::Params& left, const Config::Params& right)
{
    return left.population_ == right.population_ &&
           left.years_ == right.years_ && left.mutations_ == right.mutations_ &&
           left.reproductionAge_ == right.reproductionAge_ &&
           left.offspring_ == right.offspring_ &&
           left.simulationsCount_ == right.simulationsCount_ &&
           left.catching_ == right.catching_;
}
}  // namespace

namespace Catch
{

template <>
struct StringMaker<Config::Params>
{
    static std::string convert(const Config::Params& value)
    {
        std::ostringstream os;
        os << "{" << value.population_.max_ << "," << value.years_ << ","
           << value.population_.initial_ << "," << value.mutations_.added_
           << "," << value.mutations_.lethal_ << ","
           << value.mutations_.initial_ << "," << value.reproductionAge_ << ","
           << value.offspring_.chance_ << "," << value.offspring_.count_ << ","
           << value.simulationsCount_ << "," << value.catching_.percent_ << ","
           << value.catching_.fromYear_ << "," << value.catching_.fromAge_
           << "}";
        return os.str();
    }
};
}  // namespace Catch

TEST_CASE("Config", "[penna]")
{
    SECTION("empty config")
    {
        const Config::Params defaultParams;

        std::istringstream emptyCofigString("");
        const Config::Params configParams{Config::loadConfig(emptyCofigString)};
        REQUIRE(defaultParams == configParams);
    }

    SECTION("valid config")
    {
        const Config::Params expectedParams{2000, 100000, 1000, 2,  6,    6, 4,
                                            50,   2,      4,    20, 2000, 5};

        std::istringstream configString(R"(
population:
  initial: 2000
  max: 100000
years: 1000
mutations:
  added: 2
  lethal: 6
  initial: 6
reproductionAge: 4
offspring:
  chance: 50
  count: 2
simulations: 4
catching:
  percent: 20
  fromYear: 2000
  fromAge: 5)");
        const Config::Params configParams{Config::loadConfig(configString)};
        REQUIRE(configParams == expectedParams);
    }

    SECTION("invalid config")
    {
        std::istringstream invalidConfigString(R"(
mutations:
  lethal: aaaa36
)");
        REQUIRE_THROWS(Config::loadConfig(invalidConfigString));
    }

    SECTION("partial config")
    {
        Config::Params expectedParams;
        expectedParams.population_.max_ = 100000;
        expectedParams.simulationsCount_ = 4;

        std::istringstream configString(R"(
population:
  max: 100000
simulations: 4)");
        const Config::Params configParams{Config::loadConfig(configString)};
        REQUIRE(configParams == expectedParams);
    }
}
