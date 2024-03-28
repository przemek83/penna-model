#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "src/Config.h"

namespace
{
bool operator==(const Config::Params& left, const Config::Params& right)
{
    return left.maxPopulation_ == right.maxPopulation_ &&
           left.years_ == right.years_ &&
           left.livesOnStart_ == right.livesOnStart_ &&
           left.mutations_.added_ == right.mutations_.added_ &&
           left.mutations_.lethal_ == right.mutations_.lethal_ &&
           left.mutations_.initial_ == right.mutations_.initial_ &&
           left.reproductionAge_ == right.reproductionAge_ &&
           left.offspring_.chance_ == right.offspring_.chance_ &&
           left.offspring_.count_ == right.offspring_.count_ &&
           left.simulationsCount_ == right.simulationsCount_;
}
}  // namespace

namespace Catch
{
template <>
struct StringMaker<Config::Params>
{
    static std::string convert(Config::Params const& value)
    {
        std::ostringstream os;
        os << "{" << value.maxPopulation_ << "," << value.years_ << ","
           << value.livesOnStart_ << "," << value.mutations_.added_ << ","
           << value.mutations_.lethal_ << "," << value.mutations_.initial_
           << "," << value.reproductionAge_ << "," << value.offspring_.chance_
           << "," << value.offspring_.count_ << "," << value.simulationsCount_
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
        const Config::Params expectedParams{100000, 1000, 2000, 2, 6,
                                            6,      4,    50,   2, 4};

        std::istringstream configString(R"(
maxPopulation: 100000
years: 1000
livesOnStart: 2000
mutations:
  added: 2
  lethal: 6
  initial: 6
reproductionAge: 4
offspring:
  chance: 50
  count: 2
simulations: 4)");
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
        expectedParams.maxPopulation_ = 100000;
        expectedParams.simulationsCount_ = 4;

        std::istringstream configString(R"(
maxPopulation: 100000
simulations: 4)");
        const Config::Params configParams{Config::loadConfig(configString)};
        REQUIRE(configParams == expectedParams);
    }
}
